//-----------------------------------------------------------------------------
#include <iostream>
#include <cassert>
#include <vector>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
typedef vector<int> ivec;
//-----------------------------------------------------------------------------

template <typename T>
bool isSorted(const T* A, size_t aSize)
{
   assert((aSize > 0) && "isSorted: Array is empty");
   
   for (size_t i = 1; i < aSize; ++i)
   {
      if (A[i] < A[i-1]) { return false; }
   }
   
   return true;
}

template <typename T>
bool isSorted(const T& aCont)
{
   assert((!aCont.empty()) &&  "isSorted: Container is empty");
   
   typename T::const_iterator b = aCont.cbegin() + 1;
   typename T::const_iterator e = aCont.cend();
   
   while (b != e)
   {
      if ( *b < *(b - 1) ) { return false; }
      ++b;
   }
   
   return true;
}



int main (int argc, char** argv)
{
   int S[] = {0, 1, 2, 3};
   int U[] = {3, 2, 1, 0};
   
   cout << ((isSorted(S, 4)) ? ("sorted") : ("not sorted")) << endl;
   cout << ((isSorted(U, 4)) ? ("sorted") : ("not sorted")) << endl;
   //cout << ((isSorted(U, 0)) ? ("sorted") : ("not sorted")) << endl;
   
   cout << ( ( isSorted(ivec({0, 1, 2, 3})) ) ? ("sorted") : ("not sorted") ) << endl;
   cout << ( ( isSorted(ivec({3, 2, 1, 0})) ) ? ("sorted") : ("not sorted") ) << endl;
   cout << ( ( isSorted(ivec({3})) ) ? ("sorted") : ("not sorted") ) << endl;
   
   return 0;
}

