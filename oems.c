#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int* readTestCase (char *filename, int line)
{
	FILE *fp ;
	char buf[20] ;
	int i ;
	int *arr ;

	fp = fopen (filename , "r") ;
	arr = (int *) malloc (line * sizeof(int)) ;

	i = 0 ;
	while (fgets (buf, 20, fp) != NULL)
	{
		buf[strlen(buf)-1] = '\0' ;
		arr[i] = atoi (buf) ;
		i++ ;
	}
	fclose (fp) ;

	return arr ;
}

int readTestLines (char *filename)
{
	FILE *fp ;
	char buf[20] ;
	int line = 0 ;

	fp = fopen (filename , "r") ;
	while (fgets (buf, 20, fp) != NULL)
		line++ ;

	return line ;

	fclose (fp) ;
}

int main (int argc, char **argv)
{
	int rank, np, i, phase, N ;	
	int *local_arr, *recv_arr, *arr ;
	int locSize = 5 , recvSize = 5 , recvCount ;

	MPI_Init (NULL, NULL) ;
	MPI_Comm_size (MPI_COMM_WORLD, &np) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;
	MPI_Status status ;

	if (rank == 0)
	{
		N = readTestLines (argv[1]) ;
		printf ("N = %d\n", N) ;
		arr = readTestCase (argv[1], N) ;
		/*
		printf ("The array elements are - \n") ;
		for (int i = 0 ; i < N ; i++)
			printf ("arr[%d] = %d\n", i, arr[i]) ;
		*/
	}

	MPI_Bcast (&N, 	1, MPI_INT, 0, MPI_COMM_WORLD) ;
	//printf ("Got N as %d by %d\n", N, rank) ;

	if (N % np == 0)
	{
		locSize = N/np ;
		recvSize = N/np ;
	}
	else
	{
		int rem, ceilN ;
		rem = N % np ;
		ceilN = N + (np - rem) ;

		if (rank == np - 1)
		{
			printf ("UNDIVISIBLE!\n") ;
			locSize = N - (rank*ceilN/np) ;
			printf ("localSize = %d\n", locSize) ;
		}
		else
			locSize = ceilN/np ;
			
		recvSize = ceilN/np ;
	}

	local_arr = (int *) malloc (locSize*sizeof(int)) ;
	recv_arr = (int *) malloc (recvSize*sizeof(int)) ;


	MPI_Scatter (arr, locSize, MPI_INT, local_arr, locSize, MPI_INT, 0, MPI_COMM_WORLD) ;
	qsort (local_arr , locSize , sizeof(int), compare) ;

	for (phase = 0 ; phase < np ; phase++)
	{
		if (phase % 2 == 0)
		{
			if (rank % 2 == 0)
			{
				if (rank + 1 < np)
				{
					printf ("Phase %d = %d <---> %d", phase, rank, rank+1) ;
					MPI_Recv (recv_arr, recvSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status) ;
					MPI_Get_count (&status, MPI_INT, &recvCount) ;

					MPI_Send (local_arr, locSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD) ;
					getLowest (local_arr, locSize, recv_arr, recvCount) ;
					printf (" Successful %d = %d <---> %d\n", phase, rank, rank+1) ;
				}
			}
			else
			{
				MPI_Send (local_arr, locSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD) ;
				MPI_Recv (recv_arr, recvSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status) ;
				MPI_Get_count (&status, MPI_INT, &recvCount) ;

				getHighest (recv_arr, recvCount, local_arr, locSize) ;
			}
		}
		else
		{
			if (rank % 2 == 0)
			{
				if (rank - 1 >= 0)
				{
					MPI_Send (local_arr, locSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD) ;
					MPI_Recv (recv_arr, recvSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status) ;
					MPI_Get_count (&status, MPI_INT, &recvCount) ;

					getHighest (recv_arr, recvCount, local_arr, locSize) ;
				}
			}
			else
			{
				if (rank + 1 < np)
				{
					//printf ("Phase %d = %d <---> %d", phase, rank, rank+1) ;
					MPI_Recv (recv_arr, recvSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status) ;
					MPI_Get_count (&status, MPI_INT, &recvCount) ;

					MPI_Send (local_arr, locSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD) ;
					getLowest (local_arr, locSize, recv_arr, recvCount) ;
					printf (" Successful %d = %d <---> %d\n\n", phase, rank, rank+1) ;
				}
			}
		}
	}

	MPI_Gather (local_arr, locSize, MPI_INT, arr, locSize, MPI_INT, 0, MPI_COMM_WORLD) ;

	if (rank == 0)
	{
		printf ("The list after sorting - \n") ;
		for (i = 0 ; i < N ; i++)
			printf ("arr[%d] = %d\n", i, arr[i]) ;
	}

	MPI_Finalize () ;

	return 0 ;
}