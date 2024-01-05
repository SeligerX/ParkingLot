#ifndef _MORORCYCLE_H_
#define _MORORCYCLE_H_

#include "vehicle.h"

class Motorcycle : public Vehicle
{
public:
    Motorcycle();
    Motorcycle(std::string licensePlate);

    utils::Money calculateCharge(TimePoint now) const override;
};

#endif  // _MORORCYCLE_H_