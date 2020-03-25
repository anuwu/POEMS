#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
	int np, rank ;

	MPI_Init (NULL, NULL) ;
	MPI_Comm_size (MPI_COMM_WORLD, &np) ;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;

	/*
	{
	MPI_Comm FINAL_WORLD ;

	MPI_Group world_group, final_group ;
	MPI_Comm_group (MPI_COMM_WORLD, &world_group) ;

	int ranges[1][3] = {np/2 + 1, np-1, 1};
	MPI_Group_range_excl(world_group, 1, ranges, &final_group);
	MPI_Comm_create (MPI_COMM_WORLD, final_group, &FINAL_WORLD);

	printf ("%d\n", rank) ;

	MPI_Finalize () ;
	}*/

	if (rank >= np/2 + 1)
		MPI_Finalize () ;
	else
	{
		printf ("%d\n", rank) ;
		MPI_Finalize () ;
	}

	return 0 ;
}