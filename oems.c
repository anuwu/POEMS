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
	int rank, np, i, phase ;	
	int *local_arr, *recv_arr ;

	local_arr = (int *) malloc (5*sizeof(int)) ;
	recv_arr = (int *) malloc (5*sizeof(int)) ;

	MPI_Init (NULL, NULL) ;
	MPI_Comm_size (MPI_COMM_WORLD, &np) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;

	int *arr ;
	arr = (int *) malloc (40 * sizeof(int)) ;

	{
		arr[0] = 18 ;
		arr[1] = 2 ;
		arr[2] = 11 ;
		arr[3] = 28 ;
		arr[4] = 34 ;
		arr[5] = 10 ;
		arr[6] = 14 ;
		arr[7] = 1 ;
		arr[8] = 24 ;
		arr[9] = 27 ;
		arr[10] = 9 ;
		arr[11] = 24 ;
		arr[12] = 14 ;
		arr[13] = 7 ;
		arr[14] = 36 ;
		arr[15] = 1 ;
		arr[16] = 24 ;
		arr[17] = 29 ;
		arr[18] = 3 ;
		arr[19] = 28 ;
		arr[20] = 31 ;
		arr[21] = 36 ;
		arr[22] = 26 ;
		arr[23] = 31 ;
		arr[24] = 34 ;
		arr[25] = 40 ;
		arr[26] = 28 ;
		arr[27] = 30 ;
		arr[28] = 19 ;
		arr[29] = 15 ;
		arr[30] = 25 ;
		arr[31] = 10 ;
		arr[32] = 20 ;
		arr[33] = 9 ;
		arr[34] = 4 ;
		arr[35] = 17 ;
		arr[36] = 15 ;
		arr[37] = 19 ;
		arr[38] = 36 ;
		arr[39] = 2 ;
	}

	MPI_Scatter (arr, 5, MPI_INT, local_arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;
	qsort (local_arr , 5 , sizeof(int), compare) ;

	for (phase = 0 ; phase < np ; phase++)
	{
		if (phase % 2 == 0)
		{
			if (rank % 2 == 0)
			{
				if (rank + 1 < np)
				{
					MPI_Recv (recv_arr, 5, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
					MPI_Send (local_arr, 5, MPI_INT, rank+1, 0, MPI_COMM_WORLD) ;
					getLowest (local_arr, 5, recv_arr, 5) ;
				}
			}
			else
			{
				MPI_Send (local_arr, 5, MPI_INT, rank-1, 0, MPI_COMM_WORLD) ;
				MPI_Recv (recv_arr, 5, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
				getHighest (recv_arr, 5, local_arr, 5) ;
			}
		}
		else
		{
			if (rank % 2 == 0)
			{
				MPI_Send (local_arr, 5, MPI_INT, rank-1, 0, MPI_COMM_WORLD) ;
				MPI_Recv (recv_arr, 5, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
				getHighest (recv_arr, 5, local_arr, 5) ;
			}
			else
			{
				if (rank + 1 < np)
				{
					MPI_Recv (recv_arr, 5, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
					MPI_Send (local_arr, 5, MPI_INT, rank+1, 0, MPI_COMM_WORLD) ;
					getLowest (local_arr, 5, recv_arr, 5) ;
				}
			}
		}
	}

	MPI_Gather (local_arr, 5, MPI_INT, arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;
	if (rank == 0)
	{
		printf ("The list after sorting - \n") ;
		for (i = 0 ; i < 40 ; i++)
			printf ("arr[%d] = %d\n", i, arr[i]) ;
	}

	/*
	if (rank == 0)
	{		
		MPI_Scatter (arr, 5, MPI_INT, local_arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;
		qsort (local_arr , 5 , sizeof(int), compare) ;
		
		MPI_Recv (recv_arr, 5, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
		MPI_Send (local_arr, 5, MPI_INT, 1, 0, MPI_COMM_WORLD) ;

		getLowest (local_arr, 5, recv_arr, 5) ;

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
	*/

	MPI_Finalize () ;

	return 0 ;
}