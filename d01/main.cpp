#include <iostream>
#include <cassert>
#include <string>

using std::cout;
using std::endl;
using std::string;

typedef size_t (*serch_function_t)(int[], int, size_t);

void test(string test_name, size_t expected_value, serch_function_t funct, int serch_array[], int serch_key, size_t array_size)
{
	assert(funct != NULL);
	
	size_t actual_value = funct(serch_array, serch_key, array_size);
	
	if (actual_value != expected_value)
	{
		cout << "Test " + test_name + " FAILED" << endl;
	}
	else
	{
		cout << "Test " + test_name + " OK" << endl;
	}
}

bool isSorted (int A[], size_t size)
{
	for (size_t i = 0; i < (size - 1); i++)
	{
		if (A[i] > A[i+1]) return false;
	}
	
	return true;
}


size_t l_search_naive_for	(int A[], int key, size_t size)
{
	assert(0 < size);
	
	for (size_t i=0; i < size; ++i)
	{
		if (A[i] == key) return i;
	}
	
	return size;
}

size_t l_search_naive_while	(int A[], int key, size_t size)
{
	assert(0 < size);
	
	size_t result_index = 0;
	
	while ((result_index < size) && (A[result_index] != key))
	{
		result_index++;
	}
	
	return result_index;
}

size_t l_search_border_out	(int A[], int key, size_t size)
{
	assert(0 < size);
	
	A[size] = key;
	
	size_t result_index = 0;
	
	while (A[result_index] != key) result_index++;
	
	return result_index;
}

size_t l_search_border_in	(int A[], int key, size_t size)
{
	assert(0 < size);
	
	if (A[size-1] == key) return (size-1);
	
	int last_element = A[size-1];
	A[size-1] = key;
	
	size_t result_index = 0;
	
	while (A[result_index] != key) result_index++;
	
	A[size-1] = last_element;
	
	return ((result_index == (size - 1)) ? (size) : (result_index));
}

size_t b_search_iterative	(int A[], int key, size_t size)
{
	assert (0 < size);
	assert (isSorted(A, size));
	
	size_t left_index  	= 0;
	size_t right_index 	= size;
	size_t middle_index = 0;
	
	while (left_index < right_index)
	{
		middle_index = left_index + (right_index - left_index) / 2;
		
		if (A[middle_index] == key) 
		{
			return middle_index;
		}
		else if (A[middle_index] > key)
		{
			right_index = middle_index;
		}
		else
		{
			left_index = middle_index + 1;
		}
	}
	
	return size;
}

int main()
{
	bool test_assert_empty = false;
	
	int key = 7;
	
	int empty[1]	 						= {};
	int multiple_elements_unsorted[8]		= {1, 8, 5, 11, 13, 17, 23};
	 
	int single_element_positive[2] 			= {7};
	int single_element_negative[2]			= {1};
	int pair_elements_left_positive[3] 		= {7, 8};
	int pair_elements_right_positive[3] 	= {1, 7};
	int pair_elements_negative[3] 			= {1, 2};
	int multiple_elements_positive[8]		= {1, 7, 9, 11, 13, 17, 23};
	int multiple_elements_first_positive[8] = {7, 9, 11, 13, 17, 23, 27};
	int multiple_elements_last_positive[8]	= {1, 2, 3, 4, 5, 6, 7};
	int multiple_elements_negative[8] 		= {1, 8, 9, 11, 13, 17, 23};
	
	if ( test_assert_empty )
	{
		test("empty l_search_naive_for"		, 1, l_search_naive_for		, empty, key, 0);
		test("empty l_search_naive_while"	, 1, l_search_naive_while	, empty, key, 0);
		test("empty l_search_border_in"		, 1, l_search_border_in		, empty, key, 0);
		test("empty l_search_border_out"	, 1, l_search_border_out	, empty, key, 0);
		test("empty b_search_iterative"		, 1, b_search_iterative		, empty, key, 0);
	}
	
	return 0;	
}