#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
	int np, rank ;

	MPI_Init (NULL, NULL) ;
	MPI_Comm_size (MPI_COMM_WORLD, &np) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;

	int *recv_arr = (int *) malloc (5*sizeof(int)) ;

	if (rank == 0)
	{
		int *send0 = (int *) malloc (5*sizeof(int)) ;
		send0[0] = 0 ;
		send0[1] = 1 ;
		send0[2] = 2 ;
		send0[3] = 3 ;
		send0[4] = 4 ;

		MPI_Status status ;
		int recvcount ;

		MPI_Recv (recv_arr, 5, MPI_INT, 1, 0, MPI_COMM_WORLD, &status) ;
		MPI_Get_count (&status, MPI_INT, &recvcount) ;

		printf ("recvcount = %d\n", recvcount) ;
	}
	else
	{
		int *send1 = (int *) malloc (3*sizeof(int)) ;
		send1[0] = 10 ;
		send1[1] = 11 ;
		send1[2] = 12 ; 

		MPI_Send (send1, 3, MPI_INT, 0, 0, MPI_COMM_WORLD) ;
	}

	MPI_Finalize () ;

	return 0 ;
}