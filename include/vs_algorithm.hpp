namespace vs
{
   template <typename T>
   bool isSorted( typename T::const_iterator aBegin
                , typename T::const_iterator aEnd);

   template <typename T>
   bool isSorted(const T& aCont);

   template <typename T>
   typename T::const_iterator min_element( typename T::const_iterator aBegin
                                         , typename T::const_iterator aEnd);

   template <typename T>
   typename T::const_iterator linear_search(       typename T::const_iterator aBegin
                                           ,       typename T::const_iterator aEnd
                                           , const typename T::value_type&    aKey);
                                         
}

#include "vs_algorithm.cpp"             