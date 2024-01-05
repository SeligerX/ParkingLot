#include <cmath>
#include <iostream>
#include <map>

#include "bus.h"
#include "car.h"
#include "money.h"
#include "motorcycle.h"
#include "parking.h"
#include "privateAccessor.h"

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace utils;

using testFunc = int (*)();

int EXPECT_TRUE(bool expr)
{
    if (!expr)
        return 1;

    return 0;
}

int EXPECT_FALSE(bool expr)
{
    if (expr)
        return 1;

    return 0;
}

namespace money
{

// friend class to Money to access private data due test
class PrivateAccessMoney : public PrivateMemberAccessor<Money>
{
public:
    using Money::presision_;
    using Money::rawData_;
};

int testDefaultConstruct()
{
    auto mon = Money();

    const PrivateAccessMoney pam(mon);

    return EXPECT_TRUE(pam.rawData_ == 0ll);
}

int testConstruct()
{
    auto mon = Money(3, 25);

    const PrivateAccessMoney pam(mon);

    std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
              << " pam.presision_ = " << pam.presision_ << std::endl;

    return EXPECT_TRUE(pam.rawData_ == 325ll * pam.presision_);
}

int testConstructWithDouble()
{
    auto mon = Money(3.25);

    const PrivateAccessMoney pam(mon);

    std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
              << " pam.presision_ = " << pam.presision_ << std::endl;

    return EXPECT_TRUE(pam.rawData_ == 325ll * pam.presision_);
}

int testConstructNegativePapers()
{
    auto mon = Money(-3, 25);

    const PrivateAccessMoney pam(mon);

    std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
              << " pam.presision_ = " << pam.presision_ << std::endl;

    return EXPECT_TRUE(pam.rawData_ == -275ll * pam.presision_);
}

int testConstructNegativeCoins()
{
    auto mon = Money(3, -25);

    const PrivateAccessMoney pam(mon);

    std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
              << " pam.presision_ = " << pam.presision_ << std::endl;

    return EXPECT_TRUE(pam.rawData_ == 275ll * pam.presision_);
}

int testPlusAssign()
{
    auto mon1 = Money(3);
    auto mon2 = Money(0, 25);

    mon1 += mon2;

    const PrivateAccessMoney pam(mon1);

    std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
              << " pam.presision_ = " << pam.presision_ << std::endl;

    return EXPECT_TRUE(pam.rawData_ == 325ll * pam.presision_);
}

int testPlus()
{
    auto mon1 = Money(3);
    auto mon2 = Money(0, 25);

    auto mon = mon1 + mon2;

    const PrivateAccessMoney pam(mon);

    std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
              << " pam.presision_ = " << pam.presision_ << std::endl;

    return EXPECT_TRUE(pam.rawData_ == 325ll * pam.presision_);
}

int testMulScalarAssign()
{
    auto mon = Money(3, 25);

    mon *= 3;

    const PrivateAccessMoney pam(mon);

    std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
              << " pam.presision_ = " << pam.presision_ << std::endl;

    return EXPECT_TRUE(pam.rawData_ == 975ll * pam.presision_);
}

int testMulScalar()
{
    auto mon1 = Money(3, 25);

    auto mon = mon1 * .5;

    const PrivateAccessMoney pam(mon);

    std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
              << " pam.presision_ = " << pam.presision_ << std::endl;

    return EXPECT_TRUE(pam.rawData_ == 162.5 * pam.presision_);
}

int testArithmetics()
{
    // auto mon = Money(3) * .5 + Money(0.5) * 21;
    auto mon1 = Money(3);
    mon1 *= .5;
    auto mon2 = Money(0.5);
    mon2 *= 21;
    auto mon = mon1 + mon2;

    const PrivateAccessMoney pam(mon);

    std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
              << " pam.presision_ = " << pam.presision_ << std::endl;

    return EXPECT_TRUE(pam.rawData_ == 1200 * pam.presision_);
}

int testGetPapers()
{
    auto checkRoundByPresision = [](const Money& mon, double benchmark) -> bool
    {
        const PrivateAccessMoney pam(mon);

        std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
                  << " pam.presision_ = " << pam.presision_ << std::endl;

        return mon.getPapers() == std::round(benchmark);
    };

    const auto mon = Money(1) * .1;  // 1/10 fraction of 1 paper

    for (size_t i = 0; i < 10; ++i)
    {
        if (!checkRoundByPresision(Money(mon) * i, 0.1 * i))
            return -1;
    }

    return 0;
}

int testGetCoins()
{
    auto checkRoundByPresision = [](const Money& mon, double benchmark) -> bool
    {
        const PrivateAccessMoney pam(mon);

        std::osyncstream(std::cout) << "pam.rawData_ = " << pam.rawData_
                  << " pam.presision_ = " << pam.presision_ << std::endl;

        return mon.getCoins() == std::round(benchmark);
    };

    const auto mon = Money(0, 01) * .1;  // 1/10 fraction of 1 coin

    for (size_t i = 0; i < 10; ++i)
    {
        if (!checkRoundByPresision(Money(mon) * i, 0.1 * i))
            return -1;
    }

    return 0;
}

}  // namespace money

namespace parking
{

int testAddVehicle()
{
    Parking parking(10);

    Car car{"KA 3456 YY"};

    bool expFalse = parking.contains("KA 3456 YY");
    if (expFalse)
    {
        std::osyncstream(std::cout) << "this vehicle should not be here" << std::endl;
    }
    parking.park(std::make_shared<Car>(car));
    bool expTrue = parking.contains("KA 3456 YY");
    return EXPECT_TRUE(expTrue);
}

int testAddVehicleTwice()
{
    Parking parking(10);

    Car car{"KA 3456 YY"};

    parking.park(std::make_shared<Car>(car));  // first park

    bool expTrue{false};
    try
    {
        parking.park(
            std::make_shared<Car>(car));  // try to add the same vehicle which
                                          // should lead to logic_error
    }
    catch (const std::logic_error& e)
    {
        auto ex = std::string(e.what());
        if (ex.contains("park") && ex.contains("twice"))
            expTrue = true;
        std::cerr << e.what() << '\n';
    }

    return EXPECT_TRUE(expTrue);
}

int testRemoveVehicleById()
{
    Parking parking(10);

    Car car{"KA 3456 YY"};

    auto id = parking.park(std::make_shared<Car>(car));
    parking.release(id);

    bool expFalse = parking.contains("KA 3456 YY");
    return EXPECT_FALSE(expFalse);
}

int testRemoveVehicleByIdTwice()
{
    Parking parking(10);

    auto id = parking.park(std::make_shared<Car>());
    parking.release(id);  // first time regular release

    bool expTrue{false};
    try
    {
        parking.release(id);  // try to remove one more time which
                              // should lead to logic_error
    }
    catch (const std::logic_error& e)
    {
        auto ex = std::string(e.what());
        if (ex.contains("contains") && ex.contains("id"))
            expTrue = true;
        std::cerr << e.what() << '\n';
    }

    return EXPECT_TRUE(expTrue);
}

int testRemoveVehicleByPlate()
{
    Parking parking(10);

    Car car{"KA 3456 YY"};

    parking.park(std::make_shared<Car>(car));
    parking.release("KA 3456 YY");

    bool expFalse = parking.contains("KA 3456 YY");
    return EXPECT_FALSE(expFalse);
}

int testRemoveVehicleByPlatedTwice()
{
    Parking parking(10);

    Car car{"KA 3456 YY"};

    parking.park(std::make_shared<Car>(car));
    parking.release("KA 3456 YY");  // first time regular release

    bool expTrue{false};
    try
    {
        parking.release("KA 3456 YY");  // try to remove one more time which
                                        // should lead to logic_error
    }
    catch (const std::logic_error& e)
    {
        auto ex = std::string(e.what());
        if (ex.contains("contains") && ex.contains("plate"))
            expTrue = true;
        std::cerr << e.what() << '\n';
    }

    return EXPECT_TRUE(expTrue);
}

int testEmptyCapacity()
{
    Parking parking(0);

    Car car{"KA 3456 YY"};

    auto sizeB = parking.size();
    try
    {
        auto id = parking.park(std::make_shared<Car>(car));
    }
    catch (const std::logic_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    auto sizeA = parking.size();

    return EXPECT_TRUE(sizeB == sizeA);
}

int testCapacityOverflow()
{
    const size_t capacity{3};
    Parking parking(capacity);

    for (size_t i = 0; i < capacity; ++i)
        parking.park(std::make_shared<Car>());

    auto sizeB = parking.size();
    try
    {
        parking.park(std::make_shared<Car>());  // try to add one more which
                                                // should overflow
    }
    catch (const std::logic_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    auto sizeA = parking.size();

    return EXPECT_TRUE(sizeB == sizeA);
}

int testSize()
{
    Parking parking(10);

    auto id = parking.park(std::make_shared<Car>());
    if (parking.size() != 1)
    {
        std::osyncstream(std::cout) << "parking size is not correct after park" << std::endl;
        return -1;
    }

    parking.release(id);
    if (parking.size() != 0)
    {
        std::osyncstream(std::cout) << "parking size is not correct after release" << std::endl;
        return -1;
    }

    auto id2 = parking.park(std::make_shared<Motorcycle>());
    auto id3 = parking.park(std::make_shared<Bus>());
    if (parking.size() != 2)
    {
        std::osyncstream(std::cout) << "parking size is not correct after park 2 times"
                  << std::endl;
        return -1;
    }
    parking.release(id2);
    parking.release(id3);
    if (parking.size() != 0)
    {
        std::osyncstream(std::cout) << "parking size is not correct after release 2 times"
                  << std::endl;
        return -1;
    }

    return 0;
}

int testUniqueTicketIdForSameVehicle()
{
    Parking parking(10);

    Car car{"KA 3456 YY"};

    try
    {
        auto id = parking.park(std::make_shared<Car>(car));
        parking.release(id);
        auto id2 = parking.park(std::make_shared<Car>(car));

        // ticket id should be unigue even for the same car
        return EXPECT_TRUE(id != id2);
    }
    catch (const std::logic_error& e)
    {
        std::cerr << e.what() << '\n';
    }

    return -1;  // fail due to logic_error
}

int testAddRemoveVehicleByPlate()
{
    Parking parking(10);

    Car car{"KA 3456 YY"};
    Motorcycle mt{"KB 7890 TL"};
    Bus bus("UB 1234 VE");

    parking.park(std::make_shared<Car>(car));
    if (parking.size() != 1)
    {
        std::osyncstream(std::cout) << "release by plate is not correct" << std::endl;
        return -1;
    }

    try
    {
        parking.release("UB 1234 VE");  // release by not existed plate
    }
    catch (const std::logic_error& e)
    {
        std::cerr << e.what() << '\n';
    }

    if (parking.size() != 1)
    {
        std::osyncstream(std::cout) << "release by not existed plate is not correct" << std::endl;
        return -1;
    }

    parking.release("KA 3456 YY");  // release by plate
    if (parking.size() != 0)
    {
        std::osyncstream(std::cout) << "release by plate is not correct" << std::endl;
        return -1;
    }

    parking.park(std::make_shared<Motorcycle>(mt));
    parking.park(std::make_shared<Bus>(bus));
    if (parking.size() != 2)
    {
        std::osyncstream(std::cout) << "release by plate is not correct" << std::endl;
        return -1;
    }
    parking.release("KB 7890 TL");  // release by plate
    parking.release("UB 1234 VE");  // release by plate
    if (parking.size() != 0)
    {
        std::osyncstream(std::cout) << "release by plate is not correct" << std::endl;
        return -1;
    }

    return 0;
}

int testFastForwardTime()
{
    using namespace std::chrono;
    Parking parking(10);

    const auto timeShift = 27min;
    const auto timeShift2 = 2h;

    auto now = std::chrono::steady_clock::now();
    auto simNow = parking.simulatedNow();
    // remember difference since time point take at not the same time
    auto diff = simNow - now;

    parking.fastForwardTime(timeShift);

    auto simNow2 = parking.simulatedNow();

    // 1 second just to round diff
    auto roundDiff = 1s;
    auto finalDiff = simNow2 - (now + diff + timeShift);
    if (finalDiff > roundDiff)
    {
        std::osyncstream(std::cout) << "simNow2 - (now + diff + timeShift) = " << finalDiff
                  << std::endl;
        std::osyncstream(std::cout) << "roundDiff = " << roundDiff << std::endl;

        return -1;
    }

    parking.fastForwardTime(timeShift2);

    auto simNow3 = parking.simulatedNow();

    auto finalDiff2 = simNow3 - (now + diff + timeShift + timeShift2);
    if (finalDiff2 > roundDiff)
    {
        std::osyncstream(std::cout) << "simNow3 - (now + diff + timeShift + timeShift2) = "
                  << finalDiff2 << std::endl;
        std::osyncstream(std::cout) << "roundDiff = " << roundDiff << std::endl;

        return -1;
    }

    return 0;
}

}  // namespace parking

namespace vehicle
{
namespace car
{

int testChargeById()
{
    // $2 for first hour and $1 for further hours
    Parking parking(10);

    Car car{};

    auto id = parking.park(std::make_shared<Car>(car));
    parking.fastForwardTime(15min);
    auto charge = parking.calculateCharge(id);

    // should be 1/4 of $2
    if (charge != Money(2.0 / 4))
    {
        std::osyncstream(std::cout) << "charge for 15 min = " << charge << std::endl;
        return -1;
    }

    parking.fastForwardTime(1h);
    auto charge2 = parking.calculateCharge(id);
    if (charge2 != Money(2 + 1.0 / 4))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 15 min = " << charge2 << std::endl;
        return -1;
    }

    return 0;
}

int testChargeByPlate()
{
    // $2 for first hour and $1 for further hours
    Parking parking(10);

    Car car{"KA 3456 YY"};

    parking.park(std::make_shared<Car>(car));
    parking.fastForwardTime(15min);
    auto charge = parking.calculateCharge("KA 3456 YY");

    // should be 1/4 of $2
    if (charge != Money(2.0 / 4))
    {
        std::osyncstream(std::cout) << "charge for 15 min = " << charge << std::endl;
        return -1;
    }

    parking.fastForwardTime(1h);
    auto charge2 = parking.calculateCharge("KA 3456 YY");
    if (charge2 != Money(2 + 1.0 / 4))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 15 min = " << charge2 << std::endl;
        return -1;
    }

    return 0;
}

int testReleaseVehicleById()
{
    // $2 for first hour and $1 for further hours
    Parking parking(10);

    Car car{};

    auto id = parking.park(std::make_shared<Car>(car));
    parking.fastForwardTime(15min);
    parking.calculateCharge(id);  // make sure some calc don't break release

    parking.fastForwardTime(1h);
    auto charge = parking.release(id);  // target release
    if (charge != Money(2 + 1.0 / 4))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 15 min = " << charge << std::endl;
        return -1;
    }

    return 0;
}

int testReleaseVehicleByPlate()
{
    // $2 for first hour and $1 for further hours
    Parking parking(10);

    Car car{"KA 3456 YY"};

    parking.park(std::make_shared<Car>(car));
    parking.fastForwardTime(15min);
    // make sure some calc don't break release
    parking.calculateCharge("KA 3456 YY");

    parking.fastForwardTime(1h);
    auto charge = parking.release("KA 3456 YY");  // target release
    if (charge != Money(2 + 1.0 / 4))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 15 min = " << charge << std::endl;
        return -1;
    }

    return 0;
}

}  // namespace car

namespace motorcycle
{

int testChargeById()
{
    // $1 for first hour and $0.5 for further hours
    Parking parking(10);

    Motorcycle moto{};

    auto id = parking.park(std::make_shared<Motorcycle>(moto));
    parking.fastForwardTime(30min);
    auto charge = parking.calculateCharge(id);

    // should be 1/2 of $1
    if (charge != Money(1.0 / 2))
    {
        std::osyncstream(std::cout) << "charge for 30 min = " << charge << std::endl;
        return -1;
    }

    parking.fastForwardTime(1h);
    auto charge2 = parking.calculateCharge(id);
    if (charge2 != Money(1 + 0.5 / 2))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 15 min = " << charge2 << std::endl;
        return -1;
    }

    return 0;
}

int testChargeByPlate()
{
    // $1 for first hour and $0.5 for further hours
    Parking parking(10);

    Motorcycle moto{"KA 3456 YY"};

    parking.park(std::make_shared<Motorcycle>(moto));
    parking.fastForwardTime(30min);
    auto charge = parking.calculateCharge("KA 3456 YY");

    // should be 1/2 of $1
    if (charge != Money(1.0 / 2))
    {
        std::osyncstream(std::cout) << "charge for 30 min = " << charge << std::endl;
        return -1;
    }

    parking.fastForwardTime(1h);
    auto charge2 = parking.calculateCharge("KA 3456 YY");
    if (charge2 != Money(1 + 0.5 / 2))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 15 min = " << charge2 << std::endl;
        return -1;
    }

    return 0;
}

int testReleaseVehicleById()
{
    // $1 for first hour and $0.5 for further hours
    Parking parking(10);

    Motorcycle moto{};

    auto id = parking.park(std::make_shared<Motorcycle>(moto));
    parking.fastForwardTime(30min);
    parking.calculateCharge(id);  // make sure some calc don't break release

    parking.fastForwardTime(1h);
    auto charge = parking.release(id);
    if (charge != Money(1 + 0.5 / 2))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 15 min = " << charge << std::endl;
        return -1;
    }

    return 0;
}

int testReleaseVehicleByPlate()
{
    // $1 for first hour and $0.5 for further hours
    Parking parking(10);

    Motorcycle moto{"KA 3456 YY"};

    parking.park(std::make_shared<Motorcycle>(moto));
    parking.fastForwardTime(30min);
    // make sure some calc don't break release
    parking.calculateCharge("KA 3456 YY");

    parking.fastForwardTime(1h);
    auto charge = parking.release("KA 3456 YY");
    if (charge != Money(1 + 0.5 / 2))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 15 min = " << charge << std::endl;
        return -1;
    }

    return 0;
}

}  // namespace motorcycle

namespace bus
{

int testChargeById()
{
    // $5 for first hour and $3 for further hours
    Parking parking(10);

    Bus bus{};

    auto id = parking.park(std::make_shared<Bus>(bus));
    parking.fastForwardTime(20min);
    auto charge = parking.calculateCharge(id);

    // should be 1/3 of $5
    if (charge != Money(5.0 / 3))
    {
        std::osyncstream(std::cout) << "charge for 20 min = " << charge << std::endl;
        return -1;
    }

    parking.fastForwardTime(90min);
    auto charge2 = parking.calculateCharge(id);
    if (charge2 != Money(5 + 3.0 * 5 / 6))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 50 min = " << charge2 << std::endl;
        return -1;
    }

    return 0;
}

int testChargeByPlate()
{
    // $5 for first hour and $3 for further hours
    Parking parking(10);

    Bus bus{"KA 3456 YY"};

    parking.park(std::make_shared<Bus>(bus));
    parking.fastForwardTime(20min);
    auto charge = parking.calculateCharge("KA 3456 YY");

    // should be 1/3 of $5
    if (charge != Money(5.0 / 3))
    {
        std::osyncstream(std::cout) << "charge for 20 min = " << charge << std::endl;
        return -1;
    }

    parking.fastForwardTime(90min);
    auto charge2 = parking.calculateCharge("KA 3456 YY");
    if (charge2 != Money(5 + 3.0 * 5 / 6))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 50 min = " << charge2 << std::endl;
        return -1;
    }

    return 0;
}

int testReleaseVehicleById()
{
    // $5 for first hour and $3 for further hours
    Parking parking(10);

    Bus bus{};

    auto id = parking.park(std::make_shared<Bus>(bus));
    parking.fastForwardTime(20min);
    parking.calculateCharge(id);  // make sure some calc don't break release

    parking.fastForwardTime(90min);
    auto charge = parking.release(id);
    if (charge != Money(5 + 3.0 * 5 / 6))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 50 min = " << charge << std::endl;
        return -1;
    }

    return 0;
}

int testReleaseVehicleByPlate()
{
    // $5 for first hour and $3 for further hours
    Parking parking(10);

    Bus bus{"KA 3456 YY"};

    parking.park(std::make_shared<Bus>(bus));
    parking.fastForwardTime(20min);
    // make sure some calc don't break release
    parking.calculateCharge("KA 3456 YY");

    parking.fastForwardTime(90min);
    auto charge = parking.release("KA 3456 YY");
    if (charge != Money(5 + 3.0 * 5 / 6))
    {
        std::osyncstream(std::cout) << "charge for 1 hour 50 min = " << charge << std::endl;
        return -1;
    }

    return 0;
}

}  // namespace bus
}  // namespace vehicle

int main(int argc, char* argv[])
{
    std::osyncstream(std::cout) << "Testing ..." << std::endl;

    std::map<std::string, testFunc> tests = {
        {"money::testDefaultConstruct", money::testDefaultConstruct},
        {"money::testConstruct", money::testConstruct},
        {"money::testConstructWithDouble", money::testConstructWithDouble},

        {"money::testConstructNegativePapers",
         money::testConstructNegativePapers},
        {"money::testConstructNegativeCoins",
         money::testConstructNegativeCoins},
        {"money::testPlusAssign", money::testPlusAssign},
        {"money::testPlus", money::testPlus},
        {"money::testMulScalarAssign", money::testMulScalarAssign},
        {"money::testMulScalar", money::testMulScalar},
        {"money::testArithmetics", money::testArithmetics},
        {"money::testGetPapers", money::testGetPapers},
        {"money::testGetCoins", money::testGetCoins},

        {"parking::testAddVehicle", parking::testAddVehicle},
        {"parking::testAddVehicleTwice", parking::testAddVehicleTwice},
        {"parking::testRemoveVehicleById", parking::testRemoveVehicleById},

        {"parking::testRemoveVehicleByIdTwice",
         parking::testRemoveVehicleByIdTwice},
        {"parking::testRemoveVehicleByPlate",
         parking::testRemoveVehicleByPlate},

        {"parking::testRemoveVehicleByPlatedTwice",
         parking::testRemoveVehicleByPlatedTwice},
        {"parking::testEmptyCapacity", parking::testEmptyCapacity},
        {"parking::testCapacityOverflow", parking::testCapacityOverflow},
        {"parking::testSize", parking::testSize},

        {"parking::testUniqueTicketIdForSameVehicle",
         parking::testUniqueTicketIdForSameVehicle},

        {"parking::testAddRemoveVehicleByPlate",
         parking::testAddRemoveVehicleByPlate},
        {"parking::testFastForwardTime", parking::testFastForwardTime},

        {"vehicle::car::testChargeById", vehicle::car::testChargeById},
        {"vehicle::car::testChargeByPlate", vehicle::car::testChargeByPlate},
        {"vehicle::car::testReleaseVehicleById",
         vehicle::car::testReleaseVehicleById},
        {"vehicle::car::testReleaseVehicleByPlate",
         vehicle::car::testReleaseVehicleByPlate},

        {"vehicle::motorcycle::testChargeById",
         vehicle::motorcycle::testChargeById},
        {"vehicle::motorcycle::testChargeByPlate",
         vehicle::motorcycle::testChargeByPlate},
        {"vehicle::motorcycle::testReleaseVehicleById",
         vehicle::motorcycle::testReleaseVehicleById},
        {"vehicle::motorcycle::testReleaseVehicleByPlate",
         vehicle::motorcycle::testReleaseVehicleByPlate},

        {"vehicle::bus::testChargeById", vehicle::bus::testChargeById},
        {"vehicle::bus::testChargeByPlate", vehicle::bus::testChargeByPlate},
        {"vehicle::bus::testReleaseVehicleById",
         vehicle::bus::testReleaseVehicleById},
        {"vehicle::bus::testReleaseVehicleByPlate",
         vehicle::bus::testReleaseVehicleByPlate},
    };

    if (argc < 2)
    {
        std::osyncstream(std::cout) << "missed testcase argument ..." << std::endl;
        return -1;
    }

    return tests[argv[1]]();
}