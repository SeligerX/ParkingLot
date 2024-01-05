#include "motorcycle.h"

Motorcycle::Motorcycle() : Vehicle(Type::MOTORCYCLE)
{
}

Motorcycle::Motorcycle(std::string licensePlate)
    : Vehicle(Type::MOTORCYCLE, licensePlate)
{
}

utils::Money Motorcycle::calculateCharge(TimePoint now) const
{
    const auto priceFirstHour = utils::Money(1);
    const auto priceSubseqHour = utils::Money(0.5);

    return applyPrices(now, priceFirstHour, priceSubseqHour);
}