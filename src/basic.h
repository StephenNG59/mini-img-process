#ifndef BASIC_H
#define BASIC_H

#include <algorithm>

template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    return std::max(lo, std::min(hi, v));
}



#endif // BASIC_H
