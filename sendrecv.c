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

		if (rank*recvSize >= N)
		{
			locSize = 0 ;
			/*
			if (rank*recvSize - N < recvSize)
			{
				lastrank = rank - 1 ;
				printf ("IF1 LAST USEFUL RANK = %d\n", lastrank) ;

			}
			*/
		}
		else
		{
			if (N - rank*recvSize <= recvSize)
			{
				locSize = N - rank*recvSize ;
				/*
				if (rank == np - 1)
				{
					lastrank = rank ;
					printf ("IF2 LAST USEFUL RANK = %d\n", lastrank) ;
				}
				*/
			}
			else
				locSize = recvSize ;
		}

		/*
		if (rank == np - 1)
		{
			//printf ("UNDIVISIBLE! ") ;
			locSize = N - (rank*ceilN/np) ;
		}
		else
			locSize = ceilN/np ;
		*/
			
		//printf ("Rank = %d, localSize = %d, recvSize = %d\n", rank, locSize, recvSize) ;
	}

	local_arr = (int *) malloc (locSize*sizeof(int)) ;
	recv_arr = (int *) malloc (recvSize*sizeof(int)) ;
	MPI_Scatter (arr, locSize, MPI_INT, local_arr, recvSize, MPI_INT, 0, MPI_COMM_WORLD) ;

	//printf ("%d localSize = %d, recvSize = %d\n", rank, locSize, recvSize) ;
	printf ("For rank %d, elements - ", rank) ;
	for (i = 0 ; i < locSize ; i++)
		printf ("%d, ", local_arr[i]) ;
	printf ("\n") ;

	if (rank == 0)
		printf ("Last useful rank = %d\n", lastrank) ;

	MPI_Finalize () ;

	return 0 ;
}