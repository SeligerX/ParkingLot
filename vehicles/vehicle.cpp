#include "vehicle.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace utils;
using namespace std::chrono;
using namespace std::chrono_literals;

static std::string genLicensePlate(Vehicle::Type type)
{
    static std::unordered_map<Vehicle::Type, std::string> plateLetters = {
        {Vehicle::Type::CAR, "CA"},
        {Vehicle::Type::MOTORCYCLE, "MO"},
        {Vehicle::Type::BUS, "BU"}};

    static size_t numbers = 1000;  // just simple 4-digit number

    return plateLetters[type] + ' ' + std::to_string(numbers++) + ' ' +
           plateLetters[type];
}

static void printVehicle(std::string state, const Vehicle& vehicle)
{
    std::osyncstream(std::cout) << std::setw(outputWigth) << state << vehicle << std::endl;
}

std::ostream& operator<<(std::ostream& os, Vehicle::Type vehicleType)
{
    switch (vehicleType)
    {
        case Vehicle::Type::CAR:
            os << "Car";
            break;
        case Vehicle::Type::MOTORCYCLE:
            os << "Motocycle";
            break;
        case Vehicle::Type::BUS:
            os << "Bus";
            break;

        default:
            os << std::quoted("Unknown vehicle type");
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle)
{
    os << std::setw(10) << vehicle.getType()
       << ", license plate: " << vehicle.getLicensePlate();
    return os;
}

Vehicle::Vehicle(Type vehicleType)
    : Vehicle(vehicleType, genLicensePlate(vehicleType))
{
}

Vehicle::Vehicle(Type vehicleType, std::string licensePlate)
    : vehicleType_(vehicleType), licensePlate_(licensePlate)
{
    //printVehicle("Vehicle created: ", *this);
}

Vehicle::~Vehicle()
{
    //printVehicle("Vehicle destroyed: ", *this);
}

std::string Vehicle::getLicensePlate() const
{
    return licensePlate_;
}

Vehicle::Type Vehicle::getType() const
{
    return vehicleType_;
}

std::chrono::minutes Vehicle::getParkingDuration(TimePoint now) const
{
    auto endParking = now;
    return std::chrono::duration_cast<std::chrono::minutes>(endParking -
                                                            startParking_);
}

void Vehicle::startParking(TimePoint now)
{
    startParking_ = now;
}

utils::Money Vehicle::applyPrices(TimePoint now, Money priceFirstHour,
                                  Money priceSubseqHour) const
{
    // make hours representation based on double type for fractions
    using DoubleHours = duration<double, hours::period>;

    auto dur = getParkingDuration(now);
    DoubleHours minTime = std::min<DoubleHours>(dur, 1h);
    auto firstHour = priceFirstHour * minTime.count();
    auto subsequentHours = (dur - 1h) > DoubleHours::zero()
                               ? priceSubseqHour * DoubleHours(dur - 1h).count()
                               : Money();
    return firstHour + subsequentHours;
}
