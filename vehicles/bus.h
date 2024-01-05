#ifndef _BUS_H_
#define _BUS_H_

#include "vehicle.h"

class Bus : public Vehicle
{
public:
    Bus();
    Bus(std::string licensePlate);

    utils::Money calculateCharge(TimePoint now) const override;
};

#endif  //_BUS_H_