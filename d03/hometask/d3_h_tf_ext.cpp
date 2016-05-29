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
typedef int key_type;
typedef vector<key_type> ivec;
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
bool isSorted( typename T::const_iterator aBegin
             , typename T::const_iterator aEnd)
{
   //assert( (aBegin != aEnd) &&  "isSorted: Container is empty");
   
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
bool isSorted(const T& aCont)
{
   //assert((!aCont.empty()) &&  "isSorted: Container is empty");
   
   return isSorted<T>(aCont.cbegin(), aCont.cend());
}            
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T>
ostream& operator<<(ostream& o, const vector<T>& c)
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
   
   template <typename argument_type>
   friend ostream& operator<< (ostream& o, const exact_verifier<argument_type>& v); 

private:
   const argument_type& mKey;
};

template <typename T>
ostream& operator<< (ostream& o, const exact_verifier<T>& v)
{
   o << "Equal to " << v.mKey << endl;
   return o;
}

template <typename T>
class range_verifier
{
public:
   typedef T    argument_type;
   typedef bool result_type;
   
   range_verifier( const argument_type& aLeftBound
                 , const argument_type& aRightBound)
   : mLeftBound(aLeftBound)
   , mRightBound(aRightBound)
   {
      assert(  (  (mLeftBound < mRightBound) 
               || (  !(mLeftBound < mRightBound) 
                  && !(mRightBound < mLeftBound))) 
            && "range_verifier: wrong range");
   }
   
   result_type operator() (const argument_type& key) const
   {
      return (    (  (  !(key < mLeftBound) 
                     && !(mLeftBound < key)) 
                  || (mLeftBound < key)) 
               && (key < mRightBound));
   }
   
   template <typename argument_type>
   friend ostream& operator<< (ostream& o, const range_verifier<argument_type>& v);

private:
   const argument_type& mLeftBound;
   const argument_type& mRightBound;
};

template <typename T>
ostream& operator<< (ostream& o, const range_verifier<T>& v)
{
   o << "In range [" << v.mLeftBound << ", " << v.mRightBound << ")" << endl;
   return o;
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
   
   result_type operator() (const argument_type& key) const
   {
      return (linear_search<vector<argument_type>>(mKeys.cbegin(), mKeys.cend(), key) != mKeys.cend());   
   }
   
   template <typename argument_type>
   friend ostream& operator<< (ostream& o, const one_of_verifier<argument_type>& v);
   
private:
   const vector<argument_type>& mKeys;
};

template <typename T>
ostream& operator<< (ostream& o, const one_of_verifier<T>& v)
{
   o << "One of " << v.mKeys << endl;
   return o;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template < typename verifier_t
         , typename function_t
         , typename argument1_t
         , typename argument2_t >
void test (       size_t&      aTestNumber
          ,       size_t&      aFailCounter
          , const verifier_t&  aVerifier
          ,       function_t   aFunction
          ,       argument1_t  aArgument1
          , const argument2_t& aArgument2)
{
   aTestNumber++;
   
   auto actual = aFunction(aArgument1, aArgument2);
   
   if (aVerifier(actual))
   {
      cout << "Test #\t" << aTestNumber << "\t: OK" << endl;
   }
   else
   {
      cout << "Test #\t" << aTestNumber << "\t: error" << endl;
      aFailCounter++;
      
      cout << "Test #\t" << aTestNumber << "\t: Expected " << aVerifier << endl;
      cout << "Test #\t" << aTestNumber << "\t: Actual   " << actual    << endl;
      cout << "Test #\t" << aTestNumber << "\t: Called as F(" << aArgument1 << ", " << aArgument2 << ")" << endl; 
   }
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T>
typename T::const_iterator upper_bound(       typename T::const_iterator aBegin
                                      ,       typename T::const_iterator aEnd
                                      , const typename T::value_type&    key)
{
   assert ((isSorted<T>(aBegin, aEnd)) && "upper_bound: Container is not sorted");
   
   while (aBegin < aEnd)
   {
      typename T::const_iterator middle = aBegin + (aEnd - aBegin) / 2;
      
      if (key < *middle)   // [begin, middle)
      {
         aEnd = middle;
      }
      else                 // (middle, end)
      {
         aBegin = ++middle;
      }
      
   }
   
   return aBegin;
}                                      
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
template <typename function_t>
void test_upper_bound(function_t bound_imp)
{
   key_type key = 26;
   size_t testCounter = 0;
   size_t failCounter = 0;
   
   auto imp_adapter = [bound_imp] (ivec& v, const key_type& k) {
      ivec::const_iterator bound = bound_imp(v.cbegin(), v.cend(), k);
      return (bound == v.cend()) ? (-1) : (bound - v.cbegin()) ; 
   };
   
   // degenerated
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>(-1), imp_adapter, ivec({}), key);
   
   // first trivial
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>(-1), imp_adapter, ivec({key-1}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>(-1), imp_adapter, ivec({key  }), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>( 0), imp_adapter, ivec({key+1}), key);
   
   // second trivial
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>(-1), imp_adapter, ivec({key-1, key-1}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>(-1), imp_adapter, ivec({key-1, key  }), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>(-1), imp_adapter, ivec({key  , key  }), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>( 1), imp_adapter, ivec({key  , key+1}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>( 0), imp_adapter, ivec({key+1, key+1}), key);
   
   // common
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>(-1), imp_adapter, ivec({  2,   6,   8,  10,  21,  23}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>(-1), imp_adapter, ivec({  2,   6,   8,  10,  21, key}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>(-1), imp_adapter, ivec({  2,   6,   8,  10, key, key}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>( 5), imp_adapter, ivec({  2,   6,   8, key, key,  27}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>( 4), imp_adapter, ivec({  2,   6, key, key,  27,  30}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>( 3), imp_adapter, ivec({  2, key, key,  27,  30,  37}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>( 2), imp_adapter, ivec({key, key,  27,  30,  37,  43}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>( 1), imp_adapter, ivec({key,  27,  30,  37,  43,  59}), key);
   test(testCounter, failCounter, exact_verifier<ivec::difference_type>( 0), imp_adapter, ivec({ 27,  30,  37,  43,  59,  95}), key);
   
   cout << "#---------- TEST SUMMARY" << endl;
   cout << "\t\t Tests" << endl;
	cout << "Total:\t" 	  << testCounter 			     << endl;
	cout << "Fault:\t" 	  << failCounter 	           << "\t(" << failCounter * 100 / testCounter << "%)" << endl;
	cout << "Success:\t"   << testCounter-failCounter << "\t(" << (testCounter-failCounter) * 100 / testCounter << "%)" << endl;
}

void test_all_bound()
{
   cout << "#---------- TEST Upper Bound" << endl;
   test_upper_bound(upper_bound<ivec>);
   cout << endl;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void run_test()
{
   cout << "#---------- TEST Bound Functions" << endl << endl;
   test_all_bound();
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int main (int argc, char** argv)
{
   //int S[] = {0, 1, 2, 3};
   //int U[] = {3, 2, 1, 0};
   
   //cout << ((isSorted(S, 4)) ? ("sorted") : ("not sorted")) << endl;
   //cout << ((isSorted(U, 4)) ? ("sorted") : ("not sorted")) << endl;
   //cout << ((isSorted(U, 0)) ? ("sorted") : ("not sorted")) << endl;
   
   // cout << ( ( isSorted(ivec({0, 1, 2, 3})) ) ? ("sorted") : ("not sorted") ) << endl;
   // cout << ( ( isSorted(ivec({3, 2, 1, 0})) ) ? ("sorted") : ("not sorted") ) << endl;
   // cout << ( ( isSorted(ivec({3})) ) ? ("sorted") : ("not sorted") ) << endl;
   
   //cout << ivec({3, 2, 1, 0}) << endl;
   
   //exact_verifier<int> a(7);
   //cout << a;
   
   //cout << (a(2) ? "equal" : "unequal") << endl;
   //cout << (a(7) ? "equal" : "unequal") << endl;
   
   //range_verifier<int> a(5, 7);
   //cout << a;
   // cout << (a(5) ? "equal" : "unequal") << endl;
   // cout << (a(6) ? "equal" : "unequal") << endl;
   // cout << (a(7) ? "equal" : "unequal") << endl;
   // cout << (a(8) ? "equal" : "unequal") << endl;
   
   // ivec a = {1, 2, 3, 4, 5};
   // ivec::const_iterator i = linear_search<ivec>(a.cbegin(), a.cend(), 6);
   // cout << ((i == a.cend()) ? -1 : (i - a.cbegin())) << endl;  
   
   // ivec a = {1, 2, 3, 4, 5};
   // one_of_verifier<int> v(a);
   // cout << v;
   // cout << (v(0) ? "equal" : "unequal") << endl;
   // cout << (v(1) ? "equal" : "unequal") << endl;
   // cout << (v(2) ? "equal" : "unequal") << endl;
   // cout << (v(3) ? "equal" : "unequal") << endl;
   // cout << (v(4) ? "equal" : "unequal") << endl;
   // cout << (v(5) ? "equal" : "unequal") << endl;
   // cout << (v(6) ? "equal" : "unequal") << endl;
   
   string test_flag = "-test_run";
	if ((argc > 1) && (argv[1] == test_flag))
	{
      cout << "#---------- TEST START" << endl << endl;
		run_test();
      cout << "#---------- TEST FINISH" << endl << endl;
	}
   
   return 0;
}
//-----------------------------------------------------------------------------
