#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <unordered_map>
#include <utility>

#include "bus.h"
#include "car.h"
#include "money.h"
#include "motorcycle.h"
#include "parking.h"

using namespace std::chrono;
using namespace std::chrono_literals;

// mask for Parker constructor
constexpr int CAR = 1;
constexpr int MOTORCYCLE = 2;
constexpr int BUS = 4;

class Parker
{
public:
    Parker(size_t parkerNum, Parking& parking, size_t n, int mask)
        : parkerNum_(parkerNum), parking_(parking)
    {
        startWork_ = std::chrono::steady_clock::now();
        for (size_t i = 0; i < n;)
        {
            if (mask & CAR)
            {
                vehicles_.emplace_back(std::make_shared<Car>());
                ++i;
            };
            if (mask & MOTORCYCLE)
            {
                vehicles_.emplace_back(std::make_shared<Motorcycle>());
                ++i;
            };
            if (mask & BUS)
            {
                vehicles_.emplace_back(std::make_shared<Bus>());
                ++i;
            }
        }
        std::osyncstream(std::cout) << "parker #" << parkerNum_ << " generated "
                                    << n << " vehicles" << std::endl;
    }

    void park()
    {
        while (!vehicles_.empty())
        {
            auto id = parking_.park(vehicles_.front());
            // if passed here that means everything is ok (no exceptions)
            auto tp = parking_.simulatedNow() +
                      parkingTimer_[vehicles_.front()->getType()];
            tickets_.push({tp, id});
            vehicles_.pop_front();
        }
    }

    void unpark()
    {
        while (!tickets_.empty())
        {
            auto [timePoint, ticket] = tickets_.top();
            auto now = parking_.simulatedNow();
            if (now < timePoint)
            {
                break;  // since it is sorted we break
                        // asap time is not come yet
            }
            spent_ += parking_.release(ticket);
            tickets_.pop();  // clean up used tickets
        }
    }

    void operator()(std::promise<void> finished)
    {
        while (!vehicles_.empty() || !tickets_.empty())
        {
            auto beforeParked = vehicles_.size();
            try
            {
                park();
            }
            catch (const std::logic_error& e)  // it's okey
            {
                auto ex = std::string(e.what());
                if (!ex.contains("Parking is full") &&
                    !ex.contains("Trying to park the same vehicle twice"))
                {
                    std::cerr << e.what() << '\n';
                    return;
                };
            }
            catch (const std::exception&
                       e)  // TODO: make sure this exection critical
            {
                std::cerr << e.what() << '\n';
                return;
            }

            bool hasParked{vehicles_.size() != beforeParked};
            if (hasParked)
            {
                std::osyncstream(std::cout)
                    << "parker #" << parkerNum_ << " just parked vehicles: "
                    << beforeParked - vehicles_.size() << std::endl
                    << "parker #" << parkerNum_
                    << " vehicles left: " << vehicles_.size() << std::endl
                    << std::endl;
            }

            auto beforeUnparked = tickets_.size();
            try
            {
                unpark();
            }
            catch (const std::logic_error& e)
            {
                auto ex = std::string(e.what());
                if (!ex.contains("Parking do not contains vehicle"))
                {
                    std::cerr << e.what() << '\n';
                    return;
                }
            }
            catch (const std::exception&
                       e)  // TODO: make sure this exection critical
            {
                std::cerr << e.what() << '\n';
                return;
            }

            bool hasUnparked{tickets_.size() != beforeUnparked};
            if (hasUnparked)
            {
                std::osyncstream(std::cout)
                    << "parker #" << parkerNum_ << " just released vehicles: "
                    << beforeUnparked - tickets_.size() << std::endl
                    << "parker #" << parkerNum_
                    << " tickets left: " << tickets_.size() << std::endl
                    << "parker# " << parkerNum_ << "spent money for "
                    << duration_cast<minutes>(parking_.simulatedNow() -
                                              startWork_)
                    << ": " << spent_ << std::endl;
            }
        }
        finished.set_value();
    }

private:
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
    using PriorityTicket = std::pair<TimePoint, Parking::TicketId>;

    static constexpr auto PriorityComparator =
        [](const PriorityTicket& left, const PriorityTicket& right)
    { return left.first > right.first; };

    std::deque<Parking::SharedVehicle> vehicles_;
    std::priority_queue<Parker::PriorityTicket, std::deque<PriorityTicket>,
                        decltype(PriorityComparator)>
        tickets_;  // tickets prioritized by time

    // this need just to test/play with different parking duration
    std::unordered_map<Vehicle::Type, std::chrono::minutes> parkingTimer_ = {
        {Vehicle::Type::BUS, 30min},
        {Vehicle::Type::CAR, 15min},
        {Vehicle::Type::MOTORCYCLE, 5min}};

    size_t parkerNum_;
    Parking& parking_;
    TimePoint startWork_;
    utils::Money spent_;
};

using fut = std::future<void>;
void parkingTimer(Parking& parking, fut fut1, fut fut2, fut fut3, fut fut4)
{
    auto isReady = [](const fut& f)
    { return f.wait_for(0ms) == std::future_status::ready; };
    auto threadState = [&isReady](const fut& f, int n)
    {
        if (isReady(f))
            std::osyncstream(std::cout)
                << "parker #" << n << " finished" << std::endl
                << std::endl;
    };

    std::chrono::minutes currTime{0min};
    while (!isReady(fut1) || !isReady(fut2) || !isReady(fut3) || !isReady(fut4))
    {
        parking.showStats();
        std::this_thread::sleep_for(2s);  // for simulation every n second
        parking.fastForwardTime(1min);    // move forward for 1 min
        std::osyncstream(std::cout)
            << "time past from beginning: " << currTime << std::endl
            << std::endl;
        ++currTime;

        threadState(fut1, 1);
        threadState(fut2, 2);
        threadState(fut3, 3);
        threadState(fut4, 4);
    }

    std::osyncstream(std::cout)
        << "all parkers finished their work" << std::endl
        << "time past from beginning: " << currTime << std::endl
        << std::endl;
}

int main()
{
    // TODO: write doc assumtion that you can't park the same vehicle at the
    // same time, but you can park different vehicles at the same time
    std::osyncstream(std::cout)
        << "Multithreading parking presents..." << std::endl;

    Parking parking(100);
    Parker parkerMixed(1, parking, 100, BUS | CAR | MOTORCYCLE);
    Parker parkerBus(2, parking, 20, BUS);
    Parker parkerCar(3, parking, 50, CAR);
    Parker parkerMoto(4, parking, 130, MOTORCYCLE);

    std::promise<void> p1, p2, p3, p4;

    std::jthread timerForwarder(parkingTimer, std::ref(parking),
                                p1.get_future(), p2.get_future(),
                                p3.get_future(), p4.get_future());

    std::jthread worker1(std::ref(parkerMixed), std::move(p1));
    std::jthread worker2(std::ref(parkerBus), std::move(p2));
    std::jthread worker3(std::ref(parkerCar), std::move(p3));
    std::jthread worker4(std::ref(parkerMoto), std::move(p4));

    return 0;
}