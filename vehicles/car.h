#ifndef _CAR_H_
#define _CAR_H_

#include "vehicle.h"

class Car : public Vehicle
{
public:
    Car();
    Car(std::string licensePlate);

    utils::Money calculateCharge(TimePoint now) const override;
};

#endif  //_CAR_H_