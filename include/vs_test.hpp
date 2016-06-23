#include <iostream>
#include <vector>

namespace vs
{
   template <typename T>
   class exact_verifier
   {
   public:   
      typedef T    argument_type;
      typedef bool result_type;
      
      explicit exact_verifier(const argument_type& aKey)
      : mKey(aKey)
      {}
      
      result_type operator() (const argument_type& aKey) const { return (mKey == aKey); }
      
      template <typename argument_type>
      friend std::ostream& operator<< (std::ostream& o, const exact_verifier<argument_type>& v); 

   private:
      const argument_type mKey;
   };

   template <typename T>
   std::ostream& operator<< (std::ostream& o, const exact_verifier<T>& v);

   template <typename T>
   class range_verifier
   {
   public:
      typedef T    argument_type;
      typedef bool result_type;
      
      range_verifier( const argument_type& aLeftBound
                    , const argument_type& aRightBound);
      
      result_type operator() (const argument_type& aKey) const;
      
      template <typename argument_type>
      friend std::ostream& operator<< (std::ostream& o, const range_verifier<argument_type>& v);

   private:
      const argument_type mLeftBound;
      const argument_type mRightBound;
   };

   template <typename T>
   std::ostream& operator<< (std::ostream& o, const range_verifier<T>& v);

   template <typename T>
   class one_of_verifier
   {
   private:
      typedef std::vector<T> _container;

   public:
      typedef T    argument_type;
      typedef bool result_type;
      
      explicit one_of_verifier(const _container& aKeys)
      :mKeys(aKeys)
      {}
      
      result_type operator() (const argument_type& key) const;
      
      template <typename argument_type>
      friend std::ostream& operator<< (std::ostream& o, const one_of_verifier<argument_type>& v);
      
   private:
      const _container mKeys;
   };

   template <typename T>
   std::ostream& operator<< (std::ostream& o, const one_of_verifier<T>& v);

   template < typename verifier_t
            , typename function_t
            , typename argument_t >
   void test (       size_t&      aTestNumber
             ,       size_t&      aFailCounter
             , const verifier_t&  aVerifier
             ,       function_t   aFunction
             ,       argument_t   aArgument);

}

#include "vs_test.cpp"