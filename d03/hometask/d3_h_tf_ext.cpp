//-----------------------------------------------------------------------------
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
using std::ostream;
using std::cout;
using std::endl;
using std::vector;
using std::string;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
typedef int              key_type;
typedef vector<key_type> container;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T>
bool isSorted( typename T::const_iterator aBegin
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
bool isSorted(const T& aCont)
{
   return isSorted<T>(aCont.cbegin(), aCont.cend());
}            
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T>
ostream& operator<<(ostream& o, const std::vector<T>& c)
{
   o << "{";
   typename vector<T>::const_iterator b = c.cbegin();
   typename vector<T>::const_iterator e = c.cend();
   
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
   typedef vector<argument_type> _container;
   
   explicit one_of_verifier(const _container& aKeys)
   :mKeys(aKeys)
   {}
   
   result_type operator() (const argument_type& key) const
   {
      return (linear_search<_container>(mKeys.cbegin(), mKeys.cend(), key) != mKeys.cend());   
   }
   
   template <typename argument_type>
   friend ostream& operator<< (ostream& o, const one_of_verifier<argument_type>& v);
   
private:
   const _container& mKeys;
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
      aFailCounter++;
      cout << "Test #\t" << aTestNumber << "\t: error" << endl;
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
      
      if    (key < *middle) { aEnd   = middle;   }  // [begin, middle)
      else                  { aBegin = ++middle; }  // (middle, end)
   }
   
   return aBegin;
}

template <typename T>
typename T::const_iterator lower_bound(       typename T::const_iterator aBegin
                                      ,       typename T::const_iterator aEnd
                                      , const typename T::value_type&    key)
{
   assert ((isSorted<T>(aBegin, aEnd)) && "lower_bound: Container is not sorted");
   
   while (aBegin < aEnd)
   {
      typename T::const_iterator middle = aBegin + (aEnd - aBegin) / 2;
      
      if    (*middle < key) { aBegin = ++middle; } // (middle, end) 
      else                  { aEnd   = middle;   } // [begin, middle)
   }
   
   return aBegin;
}

template <typename T>
typename T::difference_type equal_range_length(       typename T::const_iterator aBegin
                                              ,       typename T::const_iterator aEnd
                                              , const typename T::value_type&    key)
{
   assert ((isSorted<T>(aBegin, aEnd)) && "equal_range_length: Container is not sorted");
   
   typename T::const_iterator lower = lower_bound<T>(aBegin, aEnd, key);
   
   return (upper_bound<T>(lower, aEnd, key) - lower); 
}                                    
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename function_t>
void test_upper_bound(function_t bound_imp)
{
   key_type key = 26;
   size_t testCounter = 0;
   size_t failCounter = 0;
   
   auto imp_adapter = [bound_imp] (container& v, const key_type& k) {
      container::const_iterator bound = bound_imp(v.cbegin(), v.cend(), k);
      return (bound == v.cend()) ? (-1) : (bound - v.cbegin()) ; 
   };
   
   // degenerated
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({}), key);
   
   // first trivial
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({key-1}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({key  }), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({key+1}), key);
   
   // second trivial
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({key-1, key-1}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({key-1, key  }), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({key  , key  }), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 1), imp_adapter, container({key  , key+1}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({key+1, key+1}), key);
   
   // common
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({  2,   6,   8,  10,  21,  23}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({  2,   6,   8,  10,  21, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({  2,   6,   8,  10, key, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 5), imp_adapter, container({  2,   6,   8, key, key,  27}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 4), imp_adapter, container({  2,   6, key, key,  27,  30}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 3), imp_adapter, container({  2, key, key,  27,  30,  37}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 2), imp_adapter, container({key, key,  27,  30,  37,  43}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 1), imp_adapter, container({key,  27,  30,  37,  43,  59}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({ 27,  30,  37,  43,  59,  95}), key);
   
   cout << "#---------- TEST SUMMARY" << endl;
   cout << "\t\t Tests" << endl;
	cout << "Total:\t" 	  << testCounter 			     << endl;
	cout << "Fault:\t" 	  << failCounter 	           << "\t(" << failCounter * 100 / testCounter << "%)" << endl;
	cout << "Success:\t"   << testCounter-failCounter << "\t(" << (testCounter-failCounter) * 100 / testCounter << "%)" << endl;
}

template <typename function_t>
void test_lower_bound(function_t bound_imp)
{
   key_type key = 26;
   size_t testCounter = 0;
   size_t failCounter = 0;
   
   auto imp_adapter = [bound_imp] (container& v, const key_type& k) {
      container::const_iterator bound = bound_imp(v.cbegin(), v.cend(), k);
      return (bound == v.cend()) ? (-1) : (bound - v.cbegin()) ; 
   };
   
   // degenerated
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({}), key);
   
   // first trivial
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({key-1}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({key  }), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({key+1}), key);
   
   // second trivial
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({key-1, key-1}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 1), imp_adapter, container({key-1, key  }), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({key  , key  }), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({key  , key+1}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({key+1, key+1}), key);
   
   // common
   test(testCounter, failCounter, exact_verifier<container::difference_type>(-1), imp_adapter, container({  2,   6,   8,  10,  21,  23}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 5), imp_adapter, container({  2,   6,   8,  10,  21, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 4), imp_adapter, container({  2,   6,   8,  10, key, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 3), imp_adapter, container({  2,   6,   8, key, key,  27}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 2), imp_adapter, container({  2,   6, key, key,  27,  30}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 1), imp_adapter, container({  2, key, key,  27,  30,  37}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({key, key,  27,  30,  37,  43}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({key,  27,  30,  37,  43,  59}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>( 0), imp_adapter, container({ 27,  30,  37,  43,  59,  95}), key);
   
   cout << "#---------- TEST SUMMARY" << endl;
   cout << "\t\t Tests" << endl;
	cout << "Total:\t" 	  << testCounter 			     << endl;
	cout << "Fault:\t" 	  << failCounter 	           << "\t(" << failCounter * 100 / testCounter << "%)" << endl;
	cout << "Success:\t"   << testCounter-failCounter << "\t(" << (testCounter-failCounter) * 100 / testCounter << "%)" << endl;
}

void test_all_bound()
{
   cout << "#---------- TEST Upper Bound" << endl;
   test_upper_bound(upper_bound<container>);
   cout << endl;
   
   cout << "#---------- TEST Lower Bound" << endl;
   test_lower_bound(lower_bound<container>);
   cout << endl;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename function_t>
void test_equal_range_length(function_t equal_range_length_imp)
{
   key_type key = 26;
   size_t testCounter = 0;
   size_t failCounter = 0;
   
   auto imp_adapter = [equal_range_length_imp] (container& v, const key_type& k) {
      return equal_range_length_imp(v.cbegin(), v.cend(), k); 
   };
   
   // degenerated
   test(testCounter, failCounter, exact_verifier<container::difference_type>(0), imp_adapter, container({}), key);
   
   // first trivial
   test(testCounter, failCounter, exact_verifier<container::difference_type>(0), imp_adapter, container({key-1}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(1), imp_adapter, container({key  }), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(0), imp_adapter, container({key+1}), key);
   
   // second trivial
   test(testCounter, failCounter, exact_verifier<container::difference_type>(0), imp_adapter, container({key-1, key-1}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(1), imp_adapter, container({key-1, key  }), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(2), imp_adapter, container({key  , key  }), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(1), imp_adapter, container({key  , key+1}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(0), imp_adapter, container({key+1, key+1}), key);
   
   // common
   test(testCounter, failCounter, exact_verifier<container::difference_type>(0), imp_adapter, container({  2,   6,   8,  10,  21,  23}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(1), imp_adapter, container({  2,   6,   8,  10,  21, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(2), imp_adapter, container({  2,   6,   8,  10, key, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(2), imp_adapter, container({  2,   6,   8, key, key,  27}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(2), imp_adapter, container({  2,   6, key, key,  27,  30}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(2), imp_adapter, container({  2, key, key,  27,  30,  37}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(2), imp_adapter, container({key, key,  27,  30,  37,  43}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(1), imp_adapter, container({key,  27,  30,  37,  43,  59}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(0), imp_adapter, container({ 27,  30,  37,  43,  59,  95}), key);
   
   test(testCounter, failCounter, exact_verifier<container::difference_type>(0), imp_adapter, container({  2,   6,   8,  10,  21,  23}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(1), imp_adapter, container({  2,   6,   8,  10,  21, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(2), imp_adapter, container({  2,   6,   8,  10, key, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(3), imp_adapter, container({  2,   6,   8, key, key, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(4), imp_adapter, container({  2,   6, key, key, key, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(5), imp_adapter, container({  2, key, key, key, key, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(6), imp_adapter, container({key, key, key, key, key, key}), key);
   
   test(testCounter, failCounter, exact_verifier<container::difference_type>(6), imp_adapter, container({key, key, key, key, key, key}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(5), imp_adapter, container({key, key, key, key, key,  27}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(4), imp_adapter, container({key, key, key, key,  27,  30}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(3), imp_adapter, container({key, key, key,  27,  30,  37}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(2), imp_adapter, container({key, key,  27,  30,  37,  43}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(1), imp_adapter, container({key,  27,  30,  37,  43,  59}), key);
   test(testCounter, failCounter, exact_verifier<container::difference_type>(0), imp_adapter, container({ 27,  30,  37,  43,  59,  95}), key);
   
   cout << "#---------- TEST SUMMARY" << endl;
   cout << "\t\t Tests" << endl;
	cout << "Total:\t" 	  << testCounter 			     << endl;
	cout << "Fault:\t" 	  << failCounter 	           << "\t(" << failCounter * 100 / testCounter << "%)" << endl;
	cout << "Success:\t"   << testCounter-failCounter << "\t(" << (testCounter-failCounter) * 100 / testCounter << "%)" << endl;
}

void test_all_equal_range_length()
{
   cout << "#---------- TEST Equal Range Length" << endl;
   test_equal_range_length(equal_range_length<container>);
   cout << endl;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void run_test()
{
   cout << "#---------- TEST Bound Functions" << endl << endl;
   test_all_bound();
   cout << endl;
   
   cout << "#---------- TEST Equal Range Length Functions" << endl << endl;
   test_all_equal_range_length();
   cout << endl;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int main (int argc, char** argv)
{
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