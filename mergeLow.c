#include <stdio.h>
#include <stdlib.h>

void insert (int *arr, int size, int ele)
{
	size = size - 2 ;
	while (size >= 0 && arr[size] > ele)
	{
		arr[size+1] = arr[size] ;
		size-- ;
	}
	arr[size+1] = ele ;
}

void getLowest (int* A, int Asize, int *B, int Bsize)
{
	int j = 0 ;
	int count = 1 ;
	while (j < Bsize && count <= Asize && B[j] < A[Asize-1])
	{
		insert (A,  Asize, B[j]) ;

		/*
		printf ("After merging %d, j = %d,\nthe array is - ", B[j], j) ;
		for (int i = 0 ; i < Asize ; i++)
			printf ("%d, ", A[i]) ;
		printf ("\n") ;
		*/
		
		j++ ;
		count++ ;
	}
}



int main ()
{
	//int *A, *B ;
	//A = (int *) malloc (5*sizeof(int)) ;
	//B = (int *) malloc (4*sizeof(int)) ;
	int A[5] = {1,3,5,7,10} ;
	int B[5] = {2,4,6,8,9} ;
	int Asize, Bsize ;

	Asize = 5 ;
	Bsize = 5 ;

	getLowest (A, Asize, B, Bsize) ;

	printf ("Array A after merging - \n") ;
	for (int i = 0 ; i < Asize ; i++)
		printf ("A[%d] = %d\n" , i , A[i]) ;
	

	return 0 ;
}