#ifndef vs_test_cpp
#define vs_test_cpp

#include "vs_test.hpp"
#include <cassert>
#include <iostream>
#include "vs_iohelper.hpp"
#include "vs_algorithm.hpp"

template <typename T>
std::ostream& vs::operator<< (std::ostream& o, const exact_verifier<T>& v)
{
   o << "Equal to " << v.mKey << std::endl;
   return o;
}

template <typename T>
vs::range_verifier<T>::range_verifier( const vs::range_verifier<T>::argument_type& aLeftBound
                                     , const vs::range_verifier<T>::argument_type& aRightBound)
: mLeftBound (aLeftBound)
, mRightBound(aRightBound)
{
   assert(  (  (mLeftBound < mRightBound) 
            || (  !(mLeftBound < mRightBound) 
               && !(mRightBound < mLeftBound))) 
         && "range_verifier: wrong range");
}

template <typename T>
typename vs::range_verifier<T>::result_type vs::range_verifier<T>::operator() 
                           (const vs::range_verifier<T>::argument_type& aKey) const
{
   return (    (  (  !(aKey < mLeftBound) 
                  && !(mLeftBound < aKey)) 
               || (mLeftBound < aKey)) 
            && (aKey < mRightBound));
}

template <typename T>
std::ostream& vs::operator<< (std::ostream& o, const range_verifier<T>& v)
{
   o << "In range [" << v.mLeftBound << ", " << v.mRightBound << ")" << std::endl;
   return o;
}

template <typename T>
typename vs::one_of_verifier<T>::result_type vs::one_of_verifier<T>::operator() (const vs::one_of_verifier<T>::argument_type& key) const
{
   return (vs::linear_search<vs::one_of_verifier<T>::_container>(mKeys.cbegin(), mKeys.cend(), key) != mKeys.cend());   
}


template <typename T>
std::ostream& vs::operator<< (std::ostream& o, const one_of_verifier<T>& v)
{
   o << "One of " << v.mKeys << std::endl;
   return o;
}

template < typename verifier_t
         , typename function_t
         , typename argument_t >
void vs::test (       size_t&      aTestNumber
              ,       size_t&      aFailCounter
              , const verifier_t&  aVerifier
              ,       function_t   aFunction
              ,       argument_t   aArgument)
{
   aTestNumber++;
   
   auto actual = aFunction(aArgument);
   
   if (aVerifier(actual))
   {
      std::cout << "Test #\t" << aTestNumber << "\t: OK" << std::endl;
   }
   else
   {
      aFailCounter++;
      std::cout << "Test #\t" << aTestNumber << "\t: error"                            << std::endl;
      std::cout << "Test #\t" << aTestNumber << "\t: Expected "    << aVerifier        << std::endl;
      std::cout << "Test #\t" << aTestNumber << "\t: Actual   "    << actual           << std::endl;
      std::cout << "Test #\t" << aTestNumber << "\t: Called as F(" << aArgument << ")" << std::endl; 
   }
}

#endif // vs_test_cpp