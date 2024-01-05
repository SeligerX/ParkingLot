#include "money.h"

#include <cmath>
#include <iomanip>
#include <iostream>

namespace utils
{

// FIXME: simplify output with "."
std::ostream& operator<<(std::ostream& os, const Money& money)
{
    os << std::setw(10) << "$" << money.getPapers() << '.' << money.getCoins()
       << std::endl;
    return os;
}

Money::Money(int papers, short coins)
{
    rawData_ = papers * paper_ + coins * coin_;
}

Money::Money(double money)
{
    rawData_ = money * paper_;
}

int Money::getPapers() const
{
    return std::round(1. * rawData_ / paper_);
}

short Money::getCoins() const
{
    return std::fmod(std::round(1. * rawData_ / coin_), coinsPerPaper_);
}

Money& Money::operator+=(const Money& rhs)
{
    rawData_ += rhs.rawData_;
    return *this;
}

Money Money::operator+(const Money& rhs) const
{
    Money money(*this);
    money += rhs;
    return money;
}

Money& Money::operator*=(double scalar)
{
    rawData_ *= scalar;
    return *this;
}

Money Money::operator*(double scalar) const
{
    Money money(*this);
    money *= scalar;
    return money;
}

}  // namespace utils