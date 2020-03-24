#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int compare (const void* a, const void* b)
{
	return (*(int *)a - *(int *)b) ;
}

int main ()
{
	int arr[10] = {10,9,8,7,6,5,4,3,2,1} ;
	qsort (arr+4, 6, sizeof(int) , compare) ;

	for (int i = 0 ; i < 10 ; i++)
		printf ("arr[%d] = %d\n", i, arr[i]) ;
	return 0 ;
}