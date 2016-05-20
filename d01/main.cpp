#include <iostream>
#include <cassert>
#include <string>

using std::cout;
using std::endl;
using std::string;

typedef size_t (*serch_function_t)(int[], int, size_t);

size_t test_count 			= 0;
size_t test_fault_count 	= 0;
size_t test_success_count 	= 0;

void test(string test_name, size_t expected_value, serch_function_t funct, int serch_array[], int serch_key, size_t array_size)
{
	assert(funct != NULL);
	
	test_count++;
	
	size_t actual_value = funct(serch_array, serch_key, array_size);
	
	if (actual_value != expected_value)
	{
		cout << "Test " + test_name + " FAILED" << endl;
		test_fault_count++;
	}
	else
	{
		cout << "Test " + test_name + " OK" << endl;
		test_success_count++;
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


void run_test()
{
	bool test_assert_empty    = false;
	bool test_assert_unsorted = false;
	
	bool test_single_element_positive 			= true;
	bool test_single_element_negative 			= true;
	bool test_pair_elements_left_positive 		= true;
	bool test_pair_elements_right_positive 		= true;
	bool test_pair_elements_negative 			= true;
	bool test_multiple_elements_positive 		= true;
	bool test_multiple_elements_first_positive 	= true;
	bool test_multiple_elements_last_positive 	= true;
	bool test_multiple_elements_negative 		= true;
	
	int key = 7;
		
	if ( test_assert_empty )
	{
		int empty[1]	 						= {};
		
		test("empty l_search_naive_for"		, 1, l_search_naive_for		, empty, key, 0);
		test("empty l_search_naive_while"	, 1, l_search_naive_while	, empty, key, 0);
		test("empty l_search_border_in"		, 1, l_search_border_in		, empty, key, 0);
		test("empty l_search_border_out"	, 1, l_search_border_out	, empty, key, 0);
		test("empty b_search_iterative"		, 1, b_search_iterative		, empty, key, 0);
	}
	
	if ( test_assert_unsorted )
	{
		int multiple_elements_unsorted[8]		= {1, 8, 5, 11, 13, 17, 23};
		
		test ("unsorted b_search_iterative", 1, b_search_iterative, multiple_elements_unsorted, key, 7);
	}
	
	if ( test_single_element_positive )
	{
		int single_element_positive[2] 			= {7};
		
		size_t length = 1;
		size_t expect = 0;
		
		cout << "---------- TEST single_element_positive" << endl;
		test("single_element_positive l_search_naive_for"	, expect, l_search_naive_for	, single_element_positive, key, length);
		test("single_element_positive l_search_naive_while"	, expect, l_search_naive_while	, single_element_positive, key, length);
		test("single_element_positive l_search_border_in"	, expect, l_search_border_in	, single_element_positive, key, length);
		test("single_element_positive l_search_border_out"	, expect, l_search_border_out	, single_element_positive, key, length);
		test("single_element_positive b_search_iterative"	, expect, b_search_iterative	, single_element_positive, key, length);
	}
	
	if ( test_single_element_negative )
	{
		int single_element_negative[2]			= {1};
		
		size_t length = 1;
		size_t expect = length;
		
		cout << "---------- TEST single_element_negative" << endl;
		test("single_element_negative l_search_naive_for"	, expect, l_search_naive_for	, single_element_negative, key, length);
		test("single_element_negative l_search_naive_while"	, expect, l_search_naive_while	, single_element_negative, key, length);
		test("single_element_negative l_search_border_in"	, expect, l_search_border_in	, single_element_negative, key, length);
		test("single_element_negative l_search_border_out"	, expect, l_search_border_out	, single_element_negative, key, length);
		test("single_element_negative b_search_iterative"	, expect, b_search_iterative	, single_element_negative, key, length);
	}
	
	if ( test_pair_elements_left_positive )
	{
		int pair_elements_left_positive[3] 		= {7, 8};
		
		size_t length = 2;
		size_t expect = 0;
		
		cout << "---------- TEST pair_elements_left_positive" << endl;
		test("pair_elements_left_positive l_search_naive_for"	, expect, l_search_naive_for	, pair_elements_left_positive, key, length);
		test("pair_elements_left_positive l_search_naive_while"	, expect, l_search_naive_while	, pair_elements_left_positive, key, length);
		test("pair_elements_left_positive l_search_border_in"	, expect, l_search_border_in	, pair_elements_left_positive, key, length);
		test("pair_elements_left_positive l_search_border_out"	, expect, l_search_border_out	, pair_elements_left_positive, key, length);
		test("pair_elements_left_positive b_search_iterative"	, expect, b_search_iterative	, pair_elements_left_positive, key, length);
	}
	
	if ( test_pair_elements_right_positive )
	{
		int pair_elements_right_positive[3] 	= {1, 7};
		
		size_t length = 2;
		size_t expect = 1;
		
		cout << "---------- TEST pair_elements_right_positive" << endl;
		test("pair_elements_right_positive l_search_naive_for"		, expect, l_search_naive_for	, pair_elements_right_positive, key, length);
		test("pair_elements_right_positive l_search_naive_while"	, expect, l_search_naive_while	, pair_elements_right_positive, key, length);
		test("pair_elements_right_positive l_search_border_in"		, expect, l_search_border_in	, pair_elements_right_positive, key, length);
		test("pair_elements_right_positive l_search_border_out"		, expect, l_search_border_out	, pair_elements_right_positive, key, length);
		test("pair_elements_right_positive b_search_iterative"		, expect, b_search_iterative	, pair_elements_right_positive, key, length);
		
	}
	
	if ( test_pair_elements_negative )
	{
		int pair_elements_negative[3] 			= {1, 2};
		
		size_t length = 2;
		size_t expect = length;
		
		cout << "---------- TEST pair_elements_negative" << endl;
		test("pair_elements_negative l_search_naive_for"	, expect, l_search_naive_for	, pair_elements_negative, key, length);
		test("pair_elements_negative l_search_naive_while"	, expect, l_search_naive_while	, pair_elements_negative, key, length);
		test("pair_elements_negative l_search_border_in"	, expect, l_search_border_in	, pair_elements_negative, key, length);
		test("pair_elements_negative l_search_border_out"	, expect, l_search_border_out	, pair_elements_negative, key, length);
		test("pair_elements_negative b_search_iterative"	, expect, b_search_iterative	, pair_elements_negative, key, length);
	}
	
	if ( test_multiple_elements_positive )
	{
		int multiple_elements_positive[8]		= {1, 7, 9, 11, 13, 17, 23};
		
		size_t length = 7;
		size_t expect = 1;
		
		cout << "---------- TEST multiple_elements_positive" << endl;
		test("multiple_elements_positive l_search_naive_for"	, expect, l_search_naive_for	, multiple_elements_positive, key, length);
		test("multiple_elements_positive l_search_naive_while"	, expect, l_search_naive_while	, multiple_elements_positive, key, length);
		test("multiple_elements_positive l_search_border_in"	, expect, l_search_border_in	, multiple_elements_positive, key, length);
		test("multiple_elements_positive l_search_border_out"	, expect, l_search_border_out	, multiple_elements_positive, key, length);
		test("multiple_elements_positive b_search_iterative"	, expect, b_search_iterative	, multiple_elements_positive, key, length);	
	}
	
	if ( test_multiple_elements_first_positive )
	{
		int multiple_elements_first_positive[8] = {7, 9, 11, 13, 17, 23, 27};
		
		size_t length = 7;
		size_t expect = 0;
		
		cout << "---------- TEST multiple_elements_first_positive" << endl;
		test("multiple_elements_first_positive l_search_naive_for"		, expect, l_search_naive_for	, multiple_elements_first_positive, key, length);
		test("multiple_elements_first_positive l_search_naive_while"	, expect, l_search_naive_while	, multiple_elements_first_positive, key, length);
		test("multiple_elements_first_positive l_search_border_in"		, expect, l_search_border_in	, multiple_elements_first_positive, key, length);
		test("multiple_elements_first_positive l_search_border_out"		, expect, l_search_border_out	, multiple_elements_first_positive, key, length);
		test("multiple_elements_first_positive b_search_iterative"		, expect, b_search_iterative	, multiple_elements_first_positive, key, length);
	}
	
	if ( test_multiple_elements_last_positive )
	{
		int multiple_elements_last_positive[8]	= {1, 2, 3, 4, 5, 6, 7};
		
		size_t length = 7;
		size_t expect = 6;
		
		cout << "---------- TEST multiple_elements_last_positive" << endl;
		test("multiple_elements_last_positive l_search_naive_for"		, expect, l_search_naive_for	, multiple_elements_last_positive, key, length);
		test("multiple_elements_last_positive l_search_naive_while"		, expect, l_search_naive_while	, multiple_elements_last_positive, key, length);
		test("multiple_elements_last_positive l_search_border_in"		, expect, l_search_border_in	, multiple_elements_last_positive, key, length);
		test("multiple_elements_last_positive l_search_border_out"		, expect, l_search_border_out	, multiple_elements_last_positive, key, length);
		test("multiple_elements_last_positive b_search_iterative"		, expect, b_search_iterative	, multiple_elements_last_positive, key, length);
	}
	
	if ( test_multiple_elements_negative )
	{
		int multiple_elements_negative[8] 		= {1, 8, 9, 11, 13, 17, 23};
		
		size_t length = 7;
		size_t expect = length;
		
		cout << "---------- TEST multiple_elements_negative" << endl;
		test("multiple_elements_negative l_search_naive_for"		, expect, l_search_naive_for	, multiple_elements_negative, key, length);
		test("multiple_elements_negative l_search_naive_while"		, expect, l_search_naive_while	, multiple_elements_negative, key, length);
		test("multiple_elements_negative l_search_border_in"		, expect, l_search_border_in	, multiple_elements_negative, key, length);
		test("multiple_elements_negative l_search_border_out"		, expect, l_search_border_out	, multiple_elements_negative, key, length);
		test("multiple_elements_negative b_search_iterative"		, expect, b_search_iterative	, multiple_elements_negative, key, length);
	}
	
	cout << "---------- TEST SUMMARY" << endl;
	cout << "Total test number: " 	<< test_count 			<< endl;
	cout << "Fault test number: " 	<< test_fault_count 	<< " (" << test_fault_count * 100 / test_count 	 << "%)" << endl;
	cout << "Success test number: " << test_success_count 	<< " (" << test_success_count * 100 / test_count << "%)" << endl;
}

int main(int argc, char** argv )
{
	string test_flag = "-test_run";
	if ((argc > 1) && (argv[1] == test_flag))
	{
		run_test();
	} 
	
		
	return 0;	
}