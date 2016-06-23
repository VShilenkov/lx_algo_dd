#include <vector>
#include <iostream>

namespace vs
{
   template <typename T>
   std::ostream& operator<<(std::ostream& o, const std::vector<T>& c);
}

#include "vs_iohelper.cpp"