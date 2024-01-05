#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include <chrono>
#include <string>
#include <syncstream>

#include "money.h"

constexpr auto outputWigth = 25;

class Vehicle
{
public:
    enum class Type
    {
        CAR,
        MOTORCYCLE,
        BUS
    };

    using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

    virtual ~Vehicle();

    std::string getLicensePlate() const;
    Type getType() const;
    std::chrono::minutes getParkingDuration(TimePoint now) const;

    void startParking(TimePoint now);

    virtual utils::Money calculateCharge(TimePoint now) const = 0;

protected:
    Vehicle(Type vehicleType);
    Vehicle(Type vehicleType, std::string licensePlate);

    utils::Money applyPrices(TimePoint now, utils::Money priceFirstHour,
                             utils::Money priceSubseqHour) const;

private:
    friend std::ostream& operator<<(std::ostream& os,
                                    Vehicle::Type vehicleType);

    friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);

    std::string licensePlate_;
    Type vehicleType_;

    // TODO: it is better to move out this member from vehicle to some parking
    // lot details data
    TimePoint startParking_;
};

#endif  // _VEHICLE_H_