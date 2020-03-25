#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int compare (const void* a, const void* b)
{
	return (*(int *)a - *(int *)b) ;
}

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

void insertFront (int *arr, int size, int ele)
{
	int j = 1 ;
	while (j < size && arr[j] < ele)
	{
		arr[j-1] = arr[j] ;
		j++ ;
	}
	arr[--j] = ele ;
}

void getHighest (int* A, int Asize, int* B, int Bsize)
{
	int i = Asize-1 ;
	int count = 1 ;
	while (i >= 0 && count <= Bsize && A[i] > B[0])
	{
		insertFront (B,  Bsize, A[i]) ;

		/*
		printf ("After merging %d, \nthe array is - ", A[i]) ;
		for (int j = 0 ; j < Bsize ; j++)
			printf ("%d, ", B[j]) ;
		printf ("\n") ;
		*/
		
		i-- ;
		count++ ;
	}
}

int main ()
{
	int rank, np, i, j, k ;	
	int *local_arr, *recv_arr ;

	local_arr = (int *) malloc (5*sizeof(int)) ;
	recv_arr = (int *) malloc (5*sizeof(int)) ;

	MPI_Init (NULL, NULL) ;
	MPI_Comm_size (MPI_COMM_WORLD, &np) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;

	int *arr ;
	arr = (int *) malloc (10 * sizeof(int)) ;
	arr[0] = 5 ;
	arr[1] = 9 ;
	arr[2] = 13 ;
	arr[3] = 4 ;
	arr[4] = 0 ;
	arr[5] = 9 ;
	arr[6] = 6 ;
	arr[7] = 7 ;
	arr[8] = 5 ;
	arr[9] = 3 ;


	if (rank == 0)
	{		
		MPI_Scatter (arr, 5, MPI_INT, local_arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;
		free (arr) ;
		qsort (local_arr , 5 , sizeof(int), compare) ;
		
		
		MPI_Recv (recv_arr, 5, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
		MPI_Send (local_arr, 5, MPI_INT, 1, 0, MPI_COMM_WORLD) ;

		getLowest (local_arr, 5, recv_arr, 5) ;

		arr = (int *) malloc (10 * sizeof(int)) ;
		MPI_Gather (local_arr, 5, MPI_INT, arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;

		printf ("From process 0 after parallel sort - \n") ;
		for (i = 0 ; i < 10 ; i++)
			printf ("arr[%d] = %d\n", i, arr[i]) ;
	}
	else
	{
		MPI_Scatter (arr, 5, MPI_INT, local_arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;
		qsort (local_arr , 5 , sizeof(int), compare) ;

		
		MPI_Send (local_arr, 5, MPI_INT, 0, 0, MPI_COMM_WORLD) ;
		MPI_Recv (recv_arr, 5, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;

		getHighest (recv_arr, 5, local_arr, 5) ;

		MPI_Gather (local_arr, 5, MPI_INT, arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;
	}

	MPI_Finalize () ;

	return 0 ;
}