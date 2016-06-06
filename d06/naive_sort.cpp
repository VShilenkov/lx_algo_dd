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


//-----------------------------------------------------------------------------
template < typename verifier_t
         , typename function_t
         , typename argument_t >
void test (       size_t&      aTestNumber
          ,       size_t&      aFailCounter
          , const verifier_t&  aVerifier
          ,       function_t   aFunction
          ,       argument_t   aArgument)
{
   aTestNumber++;
   
   auto actual = aFunction(aArgument);
   
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
      cout << "Test #\t" << aTestNumber << "\t: Called as F(" << aArgument << ")" << endl; 
   }
}
//-----------------------------------------------------------------------------

template <typename Iterator_t>
Iterator_t minElement(Iterator_t aB, Iterator_t aE)
{
    Iterator_t result = aB;
    
    for (Iterator_t i = aB; i < aE; ++i) if ( (*i) < (*result) ) result = i;
    
    return result;
}

template <typename Iterator_t>
void swap(Iterator_t& aL, Iterator_t& aR)
{
    Iterator_t t = aL;
    aL = aR;
    aR = t;
}


template <typename Iterator_t>
void sortOneToOne(Iterator_t aB, Iterator_t aE)
{
    if (aE == aB) return;
    
    for (Iterator_t i = aB; i < aE - 1; ++i)
    {
        for (Iterator_t j = i + 1; j < aE; ++j)
        {
            if (*j < *i) swap(*i, *j);
        }
    }
}

template <typename Iterator_t>
void sortSelect(Iterator_t aB, Iterator_t aE)
{
    for (Iterator_t i = aB; i < aE; ++i)
    {
        auto min = minElement(i, aE); 
        if (i != min) swap(*i, *min);
    }
}

template <typename Iterator_t>
void sortBuble(Iterator_t aB, Iterator_t aE)
{
    if (aE == aB) return;
    
    for (Iterator_t i = aB; i < aE - 1; ++i)
    {
        for (Iterator_t j = aE - 1; i < j; --j )
        {
            if (*j < *(j-1)) swap(*j, *(j-1));
        }
    }
}

template <typename Iterator_t>
void sortInsert(Iterator_t aB, Iterator_t aE)
{
    if (aE == aB) return;
    
    for (Iterator_t i = aB + 1; i < aE; ++i)
    {
        for (Iterator_t j = i; (aB < j) && (*j < *(j-1)); --j) swap (*(j-1), *j);
    }
}

template <typename Iterator_t>
void merge(Iterator_t aLB, Iterator_t aLE, Iterator_t aRB, Iterator_t aRE, Iterator_t aOB)
{
    while ((aLB < aLE) || (aRB < aRE))
    {
        if (aLB != aLE)
        {
            if (aRB != aRE)
            {
                if (*aRB < *aLB) { *(aOB++) = *(aRB++); }
                else             { *(aOB++) = *(aLB++); }
            }
            else                 { *(aOB++) = *(aLB++); }
            
        }
        else
        {
            if (aRB != aRE)      { *(aOB++) = *(aRB++); }
        }
    }
}

template <typename Iterator_t>
void sortMerge(Iterator_t aB, Iterator_t aE)
{
    if (aB < aE)
    {
        Iterator_t = aB + (aE - aB) / 2;
    }
    else
    {
        return;
    }
    
    
}

template <typename function_t>
void test_sort(function_t sort_imp)
{
   size_t testCounter = 0;
   size_t failCounter = 0;
   
   auto imp_adapter = [sort_imp] (container c) {
       sort_imp(c.begin(), c.end());
       return c;
   };
   
   // degenerated
   test(testCounter, failCounter, exact_verifier<container>({}), imp_adapter, container({}));
   test(testCounter, failCounter, exact_verifier<container>({1}), imp_adapter, container({1}));
   
   test(testCounter, failCounter, exact_verifier<container>({1, 1}), imp_adapter, container({1, 1}));
   test(testCounter, failCounter, exact_verifier<container>({1, 2}), imp_adapter, container({1, 2}));
   test(testCounter, failCounter, exact_verifier<container>({1, 2}), imp_adapter, container({2, 1}));
   
   test(testCounter, failCounter, exact_verifier<container>({1, 2, 3, 4, 5, 6}), imp_adapter, container({1, 2, 3, 4, 5, 6}));
   test(testCounter, failCounter, exact_verifier<container>({1, 2, 3, 4, 5, 6}), imp_adapter, container({6, 5, 4, 3, 2, 1}));
   test(testCounter, failCounter, exact_verifier<container>({1, 1, 1, 1, 1, 1}), imp_adapter, container({1, 1, 1, 1, 1, 1}));
   test(testCounter, failCounter, exact_verifier<container>({1, 1, 2, 2, 2, 2}), imp_adapter, container({1, 1, 2, 2, 2, 2}));
   test(testCounter, failCounter, exact_verifier<container>({1, 1, 2, 2, 2, 2}), imp_adapter, container({2, 2, 2, 2, 1, 1}));
   
   test(testCounter, failCounter, exact_verifier<container>({1, 2, 3, 4, 5, 6}), imp_adapter, container({5, 3, 1, 2, 4, 6}));
   test(testCounter, failCounter, exact_verifier<container>({1, 2, 3, 4, 5, 6}), imp_adapter, container({6, 4, 2, 1, 3, 5}));
   
    cout << "#---------- TEST SUMMARY" << endl;
    cout << "\t\t Tests" << endl;
	cout << "Total:\t" 	  << testCounter 			     << endl;
	cout << "Fault:\t" 	  << failCounter 	           << "\t(" << failCounter * 100 / testCounter << "%)" << endl;
	cout << "Success:\t"   << testCounter-failCounter << "\t(" << (testCounter-failCounter) * 100 / testCounter << "%)" << endl;
}

void test_all_sort()
{
   cout << "#---------- TEST Sorting One to one" << endl;
   test_sort(sortOneToOne<container::iterator>);
   cout << "#---------- TEST Sorting selection" << endl;
   test_sort(sortSelect<container::iterator>);
   cout << "#---------- TEST Sorting buble" << endl;
   test_sort(sortBuble<container::iterator>);
   cout << "#---------- TEST Sorting insetion" << endl;
   test_sort(sortInsert<container::iterator>);
   cout << endl;
}

//-----------------------------------------------------------------------------
void run_test()
{
   cout << "#---------- TEST Sort Functions" << endl << endl;
   test_all_sort();
   cout << endl;
}

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