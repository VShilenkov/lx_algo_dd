#ifndef vs_utility_cpp
#define vs_utility_cpp

#include "vs_utility.hpp"

//-----------------------------------------------------------------------------
template <typename T>
void vs::swap(T& aLeft, T& aRight)
{
   T tmp  = aLeft;
   aLeft  = aRight;
   aRight = tmp;
}

template <typename T>
void vs::swap_iterator(T aLeft, T aRight)
{
   swap(*aLeft, *aRight);
}
//-----------------------------------------------------------------------------

#endif // vs_utility_cpp