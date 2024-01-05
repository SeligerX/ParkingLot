#ifndef _MONEY_H_
#define _MONEY_H_

#include <ostream>

namespace money
{
class PrivateAccessMoney;  // forward declaration friend class
}  // namespace money

namespace utils
{

class Money
{
public:
    Money(int papers = 0, short coins = 0);
    Money(double money);

    int getPapers() const;
    short getCoins() const;

    Money& operator+=(const Money& rhs);
    Money operator+(const Money& rhs) const;

    Money& operator*=(double scalar);
    Money operator*(double scalar) const;

    auto operator<=>(const Money&) const = default;

private:
    friend std::ostream& operator<<(std::ostream& os, const Money& money);
    // tests need access to private data
    friend class money::PrivateAccessMoney;

    long long int rawData_{0ll};

    static constexpr auto presision_{10};
    static constexpr auto coinsPerPaper_{100};
    static constexpr auto coin_{presision_};
    static constexpr auto paper_{coin_ * coinsPerPaper_};
};

}  // namespace utils

#endif  // _MONEY_H_