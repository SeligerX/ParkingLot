#include "car.h"

Car::Car() : Vehicle(Type::CAR)
{
}

Car::Car(std::string licensePlate) : Vehicle(Type::CAR, licensePlate)
{
}

utils::Money Car::calculateCharge(TimePoint now) const
{
    const auto priceFirstHour = utils::Money(2);
    const auto priceSubseqHour = utils::Money(1);

    return applyPrices(now, priceFirstHour, priceSubseqHour);
}
