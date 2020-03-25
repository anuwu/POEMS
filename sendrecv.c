#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	int rank, np, i, phase, N, lastrank ;	
	int *local_arr, *recv_arr, *arr ;
	int locSize , recvSize , recvCount ;

	MPI_Init (NULL, NULL) ;
	MPI_Comm_size (MPI_COMM_WORLD, &np) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;
	MPI_Status status ;

	if (rank == 0)
	{
		N = readTestLines (argv[1]) ;
		printf ("N = %d\n", N) ;
		arr = readTestCase (argv[1], N) ;
	}

	MPI_Bcast (&N, 	1, MPI_INT, 0, MPI_COMM_WORLD) ;

	if (N % np == 0)
	{
		locSize = N/np ;
		recvSize = N/np ;
		lastrank = np-1 ;
	}
	else
	{
		int rem, ceilN ;
		rem = N % np ;
		ceilN = N + (np - rem) ;
		recvSize = ceilN/np ;
		lastrank = N/recvSize ;

		if (lastrank*recvSize == N)
			lastrank-- ;
	}

	if (rank == 0)
		printf ("Last useful rank = %d\n", lastrank) ;

	MPI_Comm FINAL_WORLD;

	
	//Without trimming
	{
		FINAL_WORLD = MPI_COMM_WORLD ;
		if (rank > lastrank)
			locSize = 0 ;
		else if (rank < lastrank)
			locSize = recvSize ;
		else
		{
			if (N - (rank+1)*recvSize == 0)
				locSize = recvSize ;
			else
				locSize = N - rank*recvSize ;
		}
	}

	/*
	//With trimming
	{
		if (lastrank == np - 1)
			FINAL_WORLD = MPI_COMM_WORLD ;
		else
		{
			//printf ("%d TRIMMING COMM\n", rank) ;
			MPI_Group world_group, final_group ;
			MPI_Comm_group (MPI_COMM_WORLD, &world_group) ;

			//printf ("%d TRIMMING COMM  post MPI_Comm_group\n", rank) ;
			int ranges[1][3] = {lastrank + 1, np, 1 };
			MPI_Group_range_excl(world_group, 1, ranges, &final_group);
			printf ("%d TRIMMING COMM  post MPI_Group_range_excl\n", rank) ;

			MPI_Comm_create (MPI_COMM_WORLD, final_group, &FINAL_WORLD);
		}

		if (rank == lastrank)
		{
			if (N - (rank+1)*recvSize == 0)
				locSize = recvSize ;
			else
				locSize = N - rank*recvSize ;
		}
		else
			locSize = recvSize ;
	}
	*/
	
	/*
		if (rank*recvSize >= N)
		locSize = 0 ;
		else
		{
			if (N - rank*recvSize <= recvSize)
				locSize = N - rank*recvSize ;
			else
				locSize = recvSize ;
		}
	*/

	local_arr = (int *) malloc (locSize*sizeof(int)) ;
	recv_arr = (int *) malloc (recvSize*sizeof(int)) ;
	MPI_Scatter (arr, locSize, MPI_INT, local_arr, recvSize, MPI_INT, 0, FINAL_WORLD) ;

	//printf ("%d localSize = %d, recvSize = %d\n", rank, locSize, recvSize) ;
	printf ("For rank %d, locSize = %d and - ", rank, locSize) ;
	for (i = 0 ; i < locSize ; i++)
		printf ("%d, ", local_arr[i]) ;
	printf ("\n") ;

	MPI_Finalize () ;

	return 0 ;
}