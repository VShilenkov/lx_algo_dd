#include <iostream>
#include <vector>

using namespace std;

template<typename T>
void sort(T::iterator b, T::iterator e)
{
    while (b < e)
    {
        typename T::iterator in_b;
        
        in_b = b + 1;
        
        while (in_b < b)
        {
            if (*in_b < *b)
            {
                typename T::value_type tmp = *b;
                *b = *in_b;
                *in_b = tmp;
            }
            
            in_b++;
        }
        
        b++;
    }
}

template <class T>
void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template<class TIter>
void sort_naive(TIter b, TIter e)
{
    // [b, e) = [sorted) [unsorted)
    // [b, e) = [b, pivot) [pivot, e)
    
    for (TIter i = b; i < e; ++i)
    {
        for (TIter j = i+1; j < e; ++j)
        {
            if (*j < *i)
                swap(*i, *j);
        }
    }
}

template<class TIter>
TIter void min_element(TIter b, TIter e)
{
    TIter result = b;
    while (b < e)
    {
        if ( *b < *result)
            result = b;
        ++b;
    }
    return result;
}


template<class TIter>
void sort_selection(TIter b, TIter e)
{
    for (TIter pivot = b; pivot < e; ++pivot)
    {
        auto m = min_element(pivot, e);
        if (pivot != m)
            swap (*pivot, *m);
    }
}

template<class TIter>
void sort_selection_fun(TIter b, TIter e)
{
    for (TIter pivot = b; pivot < e; ++pivot)
            swap (*pivot, *min_element(pivot, e));
}

template<typename T>
void sort_bubble(TIter b, TIter e)
{
    for (TIter i = b; b < e; ++i)
    {
        for (TIter j = i + 1; j < e; ++j )
        {
            if (*j < *(j-1))
                swap(*j, *(j-1));   
        }
    }
}

template<typename T>
void sort_bubble_dd(TIter b, TIter e)
{
    for (TIter pivot = e; pivot != b; --pivot)
    {
        for (TIter next_i = b + 1; next_i < pivot; ++next_i )
        {
            auto i = next_i - 1;
            if (*next_i < *i)
                swap(*i, *next_i);   
        }
    }
}