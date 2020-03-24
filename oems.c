#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int compare (const void* a, const void* b)
{
	return (*(int *)a - *(int *)b) ;
}

 ;

int main ()
{
	int rank, np, i, j, k ;	
	//int arr[10] = {7,9,1,5,10,2,6,4,8,3} ;
	int *local_arr, *recv_arr ;
	local_arr = (int *) malloc (5*sizeof(int)) ;
	recv_arr = (int *) malloc (5*sizeof(int)) ;

	MPI_Init (NULL, NULL) ;
	MPI_Comm_size (MPI_COMM_WORLD, &np) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;

	int *arr ;
	arr = (int *) malloc (10 * sizeof(int)) ;
	arr[0] = 7 ;
	arr[1] = 3 ;
	arr[2] = 1 ;
	arr[3] = 5 ;
	arr[4] = 10 ;
	arr[5] = 2 ;
	arr[6] = 8 ;
	arr[7] = 4 ;
	arr[8] = 6 ;
	arr[9] = 9 ;


	if (rank == 0)
	{		
		MPI_Scatter (arr, 5, MPI_INT, local_arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;
		//free (arr) ;
		qsort (local_arr , 5 , sizeof(int), compare) ;

		/*
		printf ("From process 0 - \n") ;
		for (i = 0 ; i < 5 ; i++)
			printf ("local_arr0[%d] = %d\n", i, local_arr[i]) ;
		*/
		
		
		MPI_Recv (recv_arr, 5, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
		MPI_Send (local_arr, 5, MPI_INT, 1, 0, MPI_COMM_WORLD) ;

		int *lowest = (int *) malloc (sizeof(int)*5) ;
		i = j = k = 0 ;
		for (k = 0 ; k < 5 ; k++)
		{
			if (local_arr[i] < recv_arr[j])
			{
				lowest[k] = local_arr[i] ;
				i++ ;
			}
			else if (recv_arr[j] < local_arr[i])
			{
				lowest[k] = recv_arr[j] ;
				j++ ;
			}
			else
			{
				lowest[k] = local_arr[i] ;
				i++ ;
				j++ ;
			}
		}

		for (k = 0 ; k < 5 ; k++)
			local_arr[k] = lowest[k] ;
		
		free (lowest) ;
		MPI_Gather (local_arr, 5, MPI_INT, arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;

		printf ("From process 0 - \n") ;
		for (i = 0 ; i < 10 ; i++)
			printf ("arr[%d] = %d\n", i, arr[i]) ;


	}
	else
	{
		MPI_Scatter (arr, 5, MPI_INT, local_arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;
		qsort (local_arr , 5 , sizeof(int), compare) ;

		/*
		printf ("From process 1\n") ;
		for (int i = 0 ; i < 5 ; i++)
			printf ("local_arr1[%d] = %d\n", i, local_arr[i]) ;
		*/


		
		MPI_Send (local_arr, 5, MPI_INT, 0, 0, MPI_COMM_WORLD) ;
		MPI_Recv (recv_arr, 5, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;

		int *highest = (int *) malloc (sizeof(int)*5) ;
		i = j = k = 4 ;
		for (k = 4 ; k >= 0 ; k--)
		{
			if (local_arr[i] > recv_arr[j])
			{
				highest[k] = local_arr[i] ;
				i-- ;
			}
			else if (recv_arr[j] > local_arr[i])
			{
				highest[k] = recv_arr[j] ;
				j-- ;
			}
			else
			{
				highest[k] = local_arr[i] ;
				i-- ;
				j-- ;
			}
		}

		for (k = 0 ; k < 5 ; k++)
			local_arr[k] = highest[k] ;
		free (highest) ;

		//MPI_Gather (localVec, localVecSize, MPI_INT, vec, localVecSize, MPI_INT, 0, MPI_COMM_WORLD) ;
		MPI_Gather (local_arr, 5, MPI_INT, arr, 5, MPI_INT, 0, MPI_COMM_WORLD) ;
	}

	MPI_Finalize () ;

	return 0 ;
}