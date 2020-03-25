#include <stdio.h>
#include <stdlib.h>

void insertFront (int *arr, int size, int ele)
{
	int j = 0 ;
	while (j < size && arr[j+1] < ele)
	{
		arr[j] = arr[j+1] ;
		j++ ;
	}
	arr[j] = ele ;
}

void getHighest (int* A, int Asize, int* B, int Bsize)
{
	int i = Asize-1 ;
	int count = 1 ;
	while (i >= 0 && count <= Bsize && A[i] > B[0])
	{
		insertFront (B,  Bsize, A[i]) ;

		/*
		printf ("After merging %d, j = %d,\nthe array is - ", B[j], j) ;
		for (int i = 0 ; i < Asize ; i++)
			printf ("%d, ", A[i]) ;
		printf ("\n") ;
		*/
		
		i-- ;
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

	getHighest (A, Asize, B, Bsize) ;

	
	printf ("Array B after merging - \n") ;
	for (int j = 0 ; j < Bsize ; j++)
		printf ("B[%d] = %d\n" , j , B[j]) ;

	return 0 ;
}