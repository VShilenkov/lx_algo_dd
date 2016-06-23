#ifndef vs_iohelper_cpp
#define vs_iohelper_cpp

#include "vs_iohelper.hpp"

template <typename T>
std::ostream& vs::operator<<(std::ostream& o, const std::vector<T>& c)
{
   o << "{";
   typename std::vector<T>::const_iterator b = c.cbegin();
   typename std::vector<T>::const_iterator e = c.cend();
   
   if (b != e)
   {
      o << *(b++);
      while (b != e) { o << ", " << *(b++); }
   }
   
   o << "}";
   return o;
}

#endif // vs_iohelper_cpp