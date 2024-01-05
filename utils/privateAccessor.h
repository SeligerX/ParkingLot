#ifndef _PRIVATE_TESTER_H_
#define _PRIVATE_TESTER_H_

namespace utils
{

template <typename ToBeAccessed>
class PrivateMemberAccessor : public ToBeAccessed
{
public:
    // consume object that need to be accessed
    PrivateMemberAccessor(const ToBeAccessed& toBeAccessed)
        : ToBeAccessed(toBeAccessed)
    {
    }
};

}  // namespace utils

#endif  // _PRIVATE_TESTER_H_