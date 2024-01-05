#ifndef _parking_H_
#define _parking_H_

#include <map>
#include <memory>
// #include <mutex>
#include <shared_mutex>

#include "money.h"
#include "vehicle.h"

class Parking
{
public:
    using TicketId = size_t;
    using SharedVehicle = std::shared_ptr<Vehicle>;

    Parking(size_t parkingCapacity);

    size_t capacity() const;
    size_t size() const;

    void list() const;
    void showStats() const;

    TicketId park(SharedVehicle vehicle);
    utils::Money calculateCharge(TicketId ticketId);
    utils::Money calculateCharge(std::string licensePlate);
    utils::Money release(TicketId ticketId);
    utils::Money release(std::string licensePlate);

    bool contains(std::string licensePlate) const;

    // TODO: it better to move these functionality to some separate class
    void fastForwardTime(std::chrono::minutes min);
    // FIXME: remove this if don't need
    std::chrono::minutes getCurrentTimeShift() const;
    // return "now" time point according to added time shift by fast forwsard
    std::chrono::time_point<std::chrono::steady_clock> simulatedNow() const;

private:
    // TODO: try to make getUid more abstract
    size_t getUid();

    // TODO: just for testing purposes, may be wrapped with some define
    // including only into necessary build
    std::map<Vehicle::Type, size_t> stats_;

    // additional time duration for simulating passed time for parked vehicle
    std::chrono::minutes forwardedTime_{std::chrono::minutes::zero()};

    struct MapId
    {
        TicketId ticket;
        std::string plate;

        bool operator<(const MapId& rhs) const
        {
            if (ticket == 0 || rhs.ticket == 0)
            {
                return plate < rhs.plate;
            }
            return ticket < rhs.ticket;
        }
    };
    using ParkingMap = std::map<MapId, SharedVehicle>;
    ParkingMap parking_;
    size_t parkingCapacity_{0};

    mutable std::shared_mutex timerMutex_;
    mutable std::shared_mutex parkingMutex_;

    // internal non-lock methods to avoid recursive look
    bool containsInternal(std::string licensePlate) const;

    ParkingMap::iterator findVehicle(TicketId ticketId);
    ParkingMap::iterator findVehicle(std::string licensePlate);
};

#endif  //_parking_H_