#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
	int np, rank, *arr, *local_arr, locSize, recvSize ;

	MPI_Init (NULL, NULL) ;
	MPI_Comm_size (MPI_COMM_WORLD, &np) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;

	MPI_Comm TRIM_WORLD ;

	MPI_Group world_group, final_group ;
	MPI_Comm_group (MPI_COMM_WORLD, &world_group) ;

	int ranges[1][3] = {6, 7, 1};
	MPI_Group_range_excl(world_group, 1, ranges, &final_group);
	MPI_Comm_create (MPI_COMM_WORLD, final_group, &TRIM_WORLD);


	if (rank >= 6)
	{
		MPI_Finalize () ;
		return 0 ;
	}
	else
		; //printf ("%d has survived!\n", rank) ;

	recvSize = 3 ;
	if (rank == 5)
		locSize = 1 ;
	else
		locSize = 3 ;

	local_arr = (int *) malloc (locSize * sizeof(int)) ;
	if (rank == 0)
	{
		arr = (int *) malloc (16 * sizeof(int)) ;
		for (int i = 0 ; i < 16 ; i++)
			arr[i] = i ;

		//printf ("Rank 0 has filled in array and locSize = %d\n", locSize) ;
	}
	else
		;//printf ("Rank %d ready with locSize = %d\n", rank, locSize) ;

	
	MPI_Scatter (arr, locSize, MPI_INT, local_arr, recvSize, MPI_INT, 0, TRIM_WORLD) ;

	//MPI_Scatter (arr, sendcounts, displs, MPI_INT, local_arr, 4, MPI_INT, 0, TRIM_WORLD) ;
	
	//printf ("Rank %d has local array - ", rank) ;
	for (int i = 0 ; i < locSize ; i++)
	{
		//printf ("%d, ", i) ;
		//printf ("%d, " , local_arr[i]) ;
		local_arr[i] = 2*local_arr[i] ;
	}
	//printf ("\n") ;

	MPI_Gather (local_arr, locSize, MPI_INT, arr, recvSize, MPI_INT, 0, TRIM_WORLD) ;
	if (rank == 0)
	{
		printf ("The total elements received by the root\n") ;
		for (int i = 0 ; i < 16 ; i++)
			printf ("%d\n" , arr[i]) ;
	}

	MPI_Finalize () ;
	return 0 ;
}