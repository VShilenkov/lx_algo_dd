#include <iostream>

int min_1 (int A[], int size, int key)
{
	for (int i=0; i<size; ++i)
	{
		if (A[i] == key) return i;
	}
	
	return -1;
}

int min_2(int A[], int size, int key)
{
	A[size] = key;
	int i = 0;
	while (A[i] != key) i++;
	
	return (i==size) ? -1: i;
}

int min_3(int A[], int size, int key)
{
	if (size == 0) return -1;
	if (A[size-1] == key) return (size-1);
	
	int tmp = A[size - 1];
	A[size - 1] = key;
	
	int i = 0;
	while (A[i] != key) i++;
	
	A[size-1] = tmp;
	
	return (i==size-1) ? -1: i;
}

int main()
{
	int A[7] = {0, 1, 2, 3, 4, 5};
	
	std::cout << min_1(A, 6, 2) << std::endl;
	std::cout << min_2(A, 6, 2) << std::endl;
	std::cout << min_3(A, 6, 2) << std::endl;
}