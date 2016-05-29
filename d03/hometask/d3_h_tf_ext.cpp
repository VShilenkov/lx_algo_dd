//-----------------------------------------------------------------------------
#include <iostream>
//#include <iomanip>
#include <iterator>
#include <cassert>
#include <vector>
#include <algorithm>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
typedef vector<int> ivec;
//-----------------------------------------------------------------------------

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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ostream& operator<<(ostream& o, const ivec& c)
{
   o << "{";
   //copy(c.begin(), c.end(), ostream_iterator<ivec::value_type>(o, ","));
   ivec::const_iterator b = c.cbegin();
   ivec::const_iterator e = c.cend();
   
   if (b != e)
   {
      o << *(b++);
      while (b != e) { o << ", " << *(b++); }
   }
   
   o << "}";
   return o;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T>
class exact_verifier
{
public:   
   typedef T    argument_type;
   typedef bool result_type;
   
   explicit exact_verifier(const argument_type& key)
   : mKey(key)
   {}
   
   result_type operator() (const argument_type& key) const { return (mKey == key); }

private:
   const argument_type& mKey;
};

template <typename T>
class range_verifier
{
public:
   typedef T    argument_type;
   typedef bool result_type;
   
   range_verifier(const argument_type& aLeftBound, const argument_type& aRightBound)
   : mLeftBound(aLeftBound)
   , mRightBound(aRightBound)
   {
      assert(  (  (mLeftBound < mRightBound) 
               || (  !(mLeftBound < mRightBound) 
                  && !(mRightBound < mLeftBound))) 
            && "range_verifier: wrong range");
   }
   
   result_type operator() (const argument_type& key)
   {
      return (    (  (  !(key < mLeftBound) 
                     && !(mLeftBound < key)) 
                  || (mLeftBound < key)) 
               && (key < mRightBound));
   }
   
private:
   const argument_type& mLeftBound;
   const argument_type& mRightBound;
};

template <typename T>
typename T::const_iterator linear_search(       typename T::const_iterator aBegin
                                        ,       typename T::const_iterator aEnd
                                        , const typename T::value_type&    key)
{
   while (aBegin != aEnd)
   {
      if (*(aBegin++) == key) return (--aBegin);
   }
   return aEnd;
}

template <typename T>
class one_of_verifier
{
public:
   typedef T    argument_type;
   typedef bool result_type;
   
   explicit one_of_verifier(const vector<argument_type>& aKeys)
   :mKeys(aKeys)
   {}
   
   result_type operator() (const argument_type& key)
   {
      return (linear_search<vector<argument_type>>(mKeys.cbegin(), mKeys.cend(), key) != mKeys.cend());   
   }
private:
   const vector<argument_type>& mKeys;
};
//-----------------------------------------------------------------------------


int main (int argc, char** argv)
{
   //int S[] = {0, 1, 2, 3};
   //int U[] = {3, 2, 1, 0};
   
   //cout << ((isSorted(S, 4)) ? ("sorted") : ("not sorted")) << endl;
   //cout << ((isSorted(U, 4)) ? ("sorted") : ("not sorted")) << endl;
   //cout << ((isSorted(U, 0)) ? ("sorted") : ("not sorted")) << endl;
   
   //cout << ( ( isSorted(ivec({0, 1, 2, 3})) ) ? ("sorted") : ("not sorted") ) << endl;
   //cout << ( ( isSorted(ivec({3, 2, 1, 0})) ) ? ("sorted") : ("not sorted") ) << endl;
   //cout << ( ( isSorted(ivec({3})) ) ? ("sorted") : ("not sorted") ) << endl;
   
   //cout << ivec({}) << endl;
   
   //exact_verifier<int> a(7);
   
   //cout << (a(2) ? "equal" : "unequal") << endl;
   //cout << (a(7) ? "equal" : "unequal") << endl;
   
   // range_verifier<int> a(5, 4);
   // cout << (a(5) ? "equal" : "unequal") << endl;
   // cout << (a(6) ? "equal" : "unequal") << endl;
   // cout << (a(7) ? "equal" : "unequal") << endl;
   // cout << (a(8) ? "equal" : "unequal") << endl;
   
   // ivec a = {1, 2, 3, 4, 5};
   // ivec::const_iterator i = linear_search<ivec>(a.cbegin(), a.cend(), 6);
   // cout << ((i == a.cend()) ? -1 : (i - a.cbegin())) << endl;  
   
   // ivec a = {1, 2, 3, 4, 5};
   // one_of_verifier<int> v(a);
   // cout << (v(0) ? "equal" : "unequal") << endl;
   // cout << (v(1) ? "equal" : "unequal") << endl;
   // cout << (v(2) ? "equal" : "unequal") << endl;
   // cout << (v(3) ? "equal" : "unequal") << endl;
   // cout << (v(4) ? "equal" : "unequal") << endl;
   // cout << (v(5) ? "equal" : "unequal") << endl;
   // cout << (v(6) ? "equal" : "unequal") << endl;
   
   return 0;
}

