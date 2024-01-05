#include "parking.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

using namespace utils;

Parking::Parking(size_t parkingCapacity) : parkingCapacity_(parkingCapacity)
{
}

size_t Parking::capacity() const
{
    return parkingCapacity_;
}

size_t Parking::size() const
{
    std::shared_lock readLock(parkingMutex_);
    return parking_.size();
}

void Parking::list() const
{
    std::shared_lock readLock(parkingMutex_);

    std::osyncstream(std::cout) << "List of parking vehicles:" << std::endl;
    for (const auto& [key, vehicle] : parking_)
    {
        std::osyncstream(std::cout)
            << std::setw(outputWigth) << "Parked: " << *vehicle
            << " Ticket id is: " << key.ticket << std::endl;
    }
    std::osyncstream(std::cout)
        << "End of vehicle list. Total = " << parking_.size() << std::endl;
}

void Parking::showStats() const
{
    std::shared_lock readLock(parkingMutex_);

    std::osyncstream(std::cout)
        << "Parking capacity: " << parkingCapacity_
        << ", used park slots: " << parking_.size()
        << ", free slots: " << parkingCapacity_ - parking_.size() << std::endl;

    for (auto [type, count] : stats_)
    {
        std::osyncstream(std::cout)
            << std::setw(outputWigth) << "{type: " << type
            << ", count: " << count << "}" << std::endl;
    }
    std::osyncstream(std::cout) << std::endl;
}

Parking::TicketId Parking::park(SharedVehicle vehicle)
{
    std::lock_guard writeLock(parkingMutex_);

    if (parking_.size() + 1 > parkingCapacity_)
    {
        throw std::logic_error("Parking is full");
    }

    // non-locking contains
    if (containsInternal(vehicle->getLicensePlate()))
    {
        throw std::logic_error("Trying to park the same vehicle twice");
    }

    size_t uid = getUid();
    // std::osyncstream(std::cout)
    //     << std::setw(outputWigth) << "Parking: " << *vehicle
    //     << " Ticket id is: " << uid << std::endl;

    vehicle->startParking(simulatedNow());
    parking_[MapId{uid, vehicle->getLicensePlate()}] = vehicle;
    ++stats_[vehicle->getType()];

    return uid;
}

utils::Money Parking::calculateCharge(TicketId ticketId)
{
    std::shared_lock readLock(parkingMutex_);

    auto found = findVehicle(ticketId);
    auto [uid, vehicle] = *found;
    return vehicle->calculateCharge(simulatedNow());
}

utils::Money Parking::calculateCharge(std::string licensePlate)
{
    std::shared_lock readLock(parkingMutex_);

    auto found = findVehicle(licensePlate);
    auto [uid, vehicle] = *found;
    return vehicle->calculateCharge(simulatedNow());
}

utils::Money Parking::release(Parking::TicketId ticketId)
{
    std::lock_guard writeLock(parkingMutex_);

    auto found = findVehicle(ticketId);
    auto [uid, vehicle] = *found;
    parking_.erase(found);
    --stats_[vehicle->getType()];

    // std::osyncstream(std::cout)
    //     << std::setw(outputWigth) << "Unparking (by ticket): " << *vehicle
    //     << " Ticket id is: " << ticketId << std::endl;
    return vehicle->calculateCharge(simulatedNow());
}

utils::Money Parking::release(std::string licensePlate)
{
    std::lock_guard writeLock(parkingMutex_);

    auto found = findVehicle(licensePlate);
    auto [uid, vehicle] = *found;
    parking_.erase(found);
    --stats_[vehicle->getType()];

    // std::osyncstream(std::cout)
    //     << std::setw(outputWigth) << "Unparking (by plate): " << *vehicle
    //     << " Ticket id is: " << uid.ticket << std::endl;
    return vehicle->calculateCharge(simulatedNow());
}

bool Parking::contains(std::string licensePlate) const
{
    std::shared_lock readLock(parkingMutex_);
    return containsInternal(licensePlate);
}

void Parking::fastForwardTime(std::chrono::minutes min)
{
    std::lock_guard writeLock(timerMutex_);
    forwardedTime_ += min;
}

std::chrono::minutes Parking::getCurrentTimeShift() const
{
    std::shared_lock readLock(timerMutex_);
    return forwardedTime_;
}

std::chrono::time_point<std::chrono::steady_clock> Parking::simulatedNow() const
{
    std::shared_lock readLock(timerMutex_);
    return std::chrono::steady_clock::now() + forwardedTime_;
}

size_t Parking::getUid()
{
    static size_t uid = 1;
    return uid++;
}

bool Parking::containsInternal(std::string licensePlate) const
{
    return parking_.contains({{}, licensePlate});
}

Parking::ParkingMap::iterator Parking::findVehicle(TicketId ticketId)
{
    auto found = parking_.find(MapId{ticketId, {}});
    if (found == parking_.end())
    {
        throw std::logic_error(
            "Parking do not contains vehicle with such id: " + ticketId);
    }

    return found;
}

Parking::ParkingMap::iterator Parking::findVehicle(std::string licensePlate)
{
    auto found = parking_.find(MapId{{}, licensePlate});
    if (found == parking_.end())
    {
        throw std::logic_error(
            "Parking do not contains vehicle with such license plate: " +
            licensePlate);
    }

    return found;
}
