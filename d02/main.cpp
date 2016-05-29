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
		cout << "Test " + test_name + " Expected: " << expected_value << endl;
		cout << "Test " + test_name + " Actual: " 	<< actual_value << endl;
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

size_t b_search_recursive_imp (int A[], int key, size_t left, size_t right)
{
    if ( left == right) return right;
    
    size_t middle_index = left + (right - left) / 2;
    
    if ( A[middle_index] == key ) 
    {
        return middle_index;
    }
    else if ( A[middle_index] > key)
    {
        return b_search_recursive_imp(A, key, left, middle_index);
    }
    else
    {
        return b_search_recursive_imp(A, key, middle_index + 1, right);
    }
}

size_t b_search_recursive	(int A[], int key, size_t size)
{
	assert (0 < size);
	assert (isSorted(A, size));
	
	size_t result = b_search_recursive_imp(A, key, 0, size); 
	
	if ( result == size ) return size;
	
   return  ( (A[result] == key) ? (result) : (size) );
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
		
		test("empty b_search_recursive"		, 1, b_search_recursive		, empty, key, 0);
	}
	
	if ( test_assert_unsorted )
	{
		int multiple_elements_unsorted[8]		= {1, 8, 5, 11, 13, 17, 23};
		
		test ("unsorted b_search_recursive", 1, b_search_recursive, multiple_elements_unsorted, key, 7);
	}
	
	if ( test_single_element_positive )
	{
		int single_element_positive[2] 			= {7};
		
		size_t length = 1;
		size_t expect = 0;
		
		cout << "---------- TEST single_element_positive" << endl;
		test("single_element_positive b_search_recursive"	, expect, b_search_recursive	, single_element_positive, key, length);
	}
	
	if ( test_single_element_negative )
	{
		int single_element_negative[2]			= {1};
		
		size_t length = 1;
		size_t expect = length;
		
		cout << "---------- TEST single_element_negative" << endl;
		test("single_element_negative b_search_recursive"	, expect, b_search_recursive	, single_element_negative, key, length);
	}
	
	if ( test_pair_elements_left_positive )
	{
		int pair_elements_left_positive[3] 		= {7, 8};
		
		size_t length = 2;
		size_t expect = 0;
		
		cout << "---------- TEST pair_elements_left_positive" << endl;
		test("pair_elements_left_positive b_search_recursive"	, expect, b_search_recursive	, pair_elements_left_positive, key, length);
	}
	
	if ( test_pair_elements_right_positive )
	{
		int pair_elements_right_positive[3] 	= {1, 7};
		
		size_t length = 2;
		size_t expect = 1;
		
		cout << "---------- TEST pair_elements_right_positive" << endl;
		test("pair_elements_right_positive b_search_recursive"		, expect, b_search_recursive	, pair_elements_right_positive, key, length);
		
	}
	
	if ( test_pair_elements_negative )
	{
		int pair_elements_negative[3] 			= {1, 2};
		
		size_t length = 2;
		size_t expect = length;
		
		cout << "---------- TEST pair_elements_negative" << endl;
		test("pair_elements_negative b_search_recursive"	, expect, b_search_recursive	, pair_elements_negative, key, length);
	}
	
	if ( test_multiple_elements_positive )
	{
		int multiple_elements_positive[8]		= {1, 7, 9, 11, 13, 17, 23};
		
		size_t length = 7;
		size_t expect = 1;
		
		cout << "---------- TEST multiple_elements_positive" << endl;
		test("multiple_elements_positive b_search_recursive"	, expect, b_search_recursive	, multiple_elements_positive, key, length);	
	}
	
	if ( test_multiple_elements_first_positive )
	{
		int multiple_elements_first_positive[8] = {7, 9, 11, 13, 17, 23, 27};
		
		size_t length = 7;
		size_t expect = 0;
		
		cout << "---------- TEST multiple_elements_first_positive" << endl;
		test("multiple_elements_first_positive b_search_recursive"		, expect, b_search_recursive	, multiple_elements_first_positive, key, length);
	}
	
	if ( test_multiple_elements_last_positive )
	{
		int multiple_elements_last_positive[8]	= {1, 2, 3, 4, 5, 6, 7};
		
		size_t length = 7;
		size_t expect = 6;
		
		cout << "---------- TEST multiple_elements_last_positive" << endl;
		test("multiple_elements_last_positive b_search_recursive"		, expect, b_search_recursive	, multiple_elements_last_positive, key, length);
	}
	
	if ( test_multiple_elements_negative )
	{
		int multiple_elements_negative[8] 		= {1, 8, 9, 11, 13, 17, 23};
		
		size_t length = 7;
		size_t expect = length;
		
		cout << "---------- TEST multiple_elements_negative" << endl;
		test("multiple_elements_negative b_search_recursive"		, expect, b_search_recursive	, multiple_elements_negative, key, length);
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