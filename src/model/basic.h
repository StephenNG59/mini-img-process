#ifndef BASIC_H
#define BASIC_H

#include <algorithm>
#include <assert.h>

template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    return std::max(lo, std::min(hi, v));
}

template<class T>
int getArrayLen(const T& array)
{
    if (array == nullptr)
        return 0;
    return sizeof(array) / sizeof(array[0]);
}

template<class T>
void getArraySize(const T& array, int &height, int &width)
{
    height = getArrayLen(array);
    width = getArrayLen(array[0]);
}

//template<class T>
//void conv(T *src, T *dst, T *kernel, ConvType=CONV_CONSTANT)
//{
//    int height =
//}

#endif // BASIC_H
