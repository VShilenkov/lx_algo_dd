#include <cassert>
#include <vector>
#include "..\include\vs_utility.hpp"
#include "..\include\vs_iohelper.hpp"

typedef std::vector<int> container;
using vs::operator<<;

template <typename T>
typename T::iterator pivotSelector(typename T::iterator aBegin, typename T::iterator aEnd)
{
   assert ((aBegin < aEnd) && "pivotSelector: empty container");
   return aBegin + (aEnd - aBegin) / 2U;
}

template <typename T>
typename T::iterator partitionLeft(typename T::iterator aBegin, typename T::iterator aEnd, typename T::iterator aPivot)
{
   assert ((aBegin < aEnd) && "partitionLeft: empty container");
   
   typename T::iterator originalBegin = aBegin;
   vs::swap_iterator(originalBegin, aPivot);           // [originalBegin] [b+1, e)

   // using vs::operator<<;

   // std::cout << container(originalBegin, aEnd) << std::endl;

   typename T::value_type pValue = *aBegin;           // [pivot_value] [unpartitioned)

                                                // [pivot_value]   [partitioned)[unpartitioned)
   ++aBegin;                                    // [originalBegin] [)           [b, e)

                                                // [pivot_value]   [<pivot_value)       [>pivot_value) [unpartitioned)
   typename T::iterator pivot = aBegin;         // [originalBegin] [originalBegin+1, p) [p, b)         [b, e)

   while (aBegin < aEnd)
   {
      if ((*aBegin) < pValue)
      {
         vs::swap_iterator(aBegin, pivot);
         ++pivot;
      }
      ++aBegin;
   }

   vs::swap_iterator(originalBegin, --pivot);

   return pivot;
}

template < typename T
         , typename PivotSelectorFunction
         , typename PartitionFunction>
void quickSort2( typename T::iterator  aBegin
               , typename T::iterator	aEnd
               , PivotSelectorFunction aPivotSelector
               , PartitionFunction     aPartition
               , size_t                aRecursionDepth = 0)
{
   assert ((aRecursionDepth < 60U)  && "quickSort2: recursion goes too deep");

   if (aEnd - aBegin < 2U) return;

   typename T::iterator pivot = aPivotSelector(aBegin, aEnd);
   
   pivot = aPartition(aBegin, aEnd, pivot);

   quickSort2<T>(aBegin    , pivot, aPivotSelector, aPartition, aRecursionDepth + 1U);
   quickSort2<T>(pivot + 1U, aEnd , aPivotSelector, aPartition, aRecursionDepth + 1U);
}

int main()
{
   return 0;
}