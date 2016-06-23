#ifndef vs_algorithm_cpp
#define vs_algorithm_cpp

#include "vs_algorithm.hpp"

//-----------------------------------------------------------------------------
template <typename T>
bool vs::isSorted( typename T::const_iterator aBegin
                 , typename T::const_iterator aEnd)
{
   if (aBegin == aEnd) { return true; }
   
   ++aBegin;
   
   while (aBegin != aEnd)
   {
      if ( *aBegin < *(aBegin - 1) ) { return false; }
      ++aBegin;
   }
   
   return true;
}

template <typename T>
bool vs::isSorted(const T& aCont)
{
   return isSorted<T>(aCont.cbegin(), aCont.cend());
}            
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T>
typename T::const_iterator vs::min_element( typename T::const_iterator aBegin
                                          , typename T::const_iterator aEnd)
{
   typename T::const_iterator min_element = aBegin++;
   for (typename T::const_iterator i = aBegin; i < aEnd; ++i) 
   {
      if ((*i) < (*min_element)) { min_element = i; }
   }

   return min_element;
}                              
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T>
typename T::const_iterator vs::linear_search(       typename T::const_iterator aBegin
                                            ,       typename T::const_iterator aEnd
                                            , const typename T::value_type&    aKey)
//-----------------------------------------------------------------------------                                        
{
   while (aBegin != aEnd)
   {
      if (*(aBegin++) == aKey) return (--aBegin);
   }
   return aEnd;
}
//-----------------------------------------------------------------------------

#endif // vs_algorithm_cpp