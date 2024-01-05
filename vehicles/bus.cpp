#include "bus.h"

Bus::Bus() : Vehicle(Type::BUS)
{
}

Bus::Bus(std::string licensePlate) : Vehicle(Type::BUS, licensePlate)
{
}

utils::Money Bus::calculateCharge(TimePoint now) const
{
    const auto priceFirstHour = utils::Money(5);
    const auto priceSubseqHour = utils::Money(3);

    return applyPrices(now, priceFirstHour, priceSubseqHour);
}