#include <iostream>

#include "bus.h"
#include "car.h"
#include "money.h"
#include "motorcycle.h"
#include "parking.h"

int main()
{
    std::osyncstream(std::cout) << "Parking presents..." << std::endl;

    Parking parking(50);

    Car car{"KA 3456 YY"};
    Motorcycle mt{"KB 7890 TL"};
    Bus bus("UB 1234 VE");

    std::cout << "Before parking something" << std::endl;
    parking.list();

    auto id = parking.park(std::make_shared<Car>(car));
    parking.release(id);

    parking.park(std::make_shared<Motorcycle>(mt));
    auto id2 = parking.park(std::make_shared<Car>());
    parking.park(std::make_shared<Bus>(bus));

    std::cout << "Parked 4 vehicles, release 1 vehicles" << std::endl;
    parking.list();

    parking.release(id2);
    parking.release("UB 1234 VE");

    std::cout << "Released 2 more vehicles" << std::endl;
    parking.list();

    return 0;
}