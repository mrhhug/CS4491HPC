/* cs4491-HPC Fall 2014 
 * Michael Hug
 * Assignment 9
 * Monte Carlo PI estimation on stampede with mpi
 */

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h> 

unsigned long long int doit(int number_of_tosses,int number_of_threads, int my_rank)
{
	unsigned long long int number_in_circle = 0;
	struct drand48_data randBuffer;
	srand48_r(time(NULL)-my_rank, &randBuffer);
	double y,x;
	int toss;
	for (toss = 0; toss < number_of_tosses/number_of_threads; toss++)
	{
		drand48_r(&randBuffer, &x);
		drand48_r(&randBuffer, &y);
			/* START debug override */
			char* debug=getenv("DEBUG");
			if(debug!=NULL && debug[0]=='1')
			{
				printf("%f %f\n",x,y);
			}
			/* END debug override */
		if (x*x + y*y <= 1)
		{
			number_in_circle++;
		}
	}
	return number_in_circle;
}
int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int my_rank, number_of_threads;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &number_of_threads);
	int number_of_tosses = 16000000;
	if(my_rank == 0)
	{
		unsigned long long int total_hits,local_hits;
		total_hits=doit(number_of_tosses, number_of_threads,my_rank);
		int i; 
		for(i = 1;i< number_of_threads;i++)
		{
			MPI_Recv( &local_hits, 1, MPI_LONG_LONG_INT,
				i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_hits+=local_hits;
		}
		/* START printPI enviCheck */
		char* debug=getenv("PRINTPI");
		if(debug!=NULL && debug[0]=='1')
		{
			printf("Number in Circle :%llu\n",total_hits);
			printf("Number of tosses :%i\n",number_of_tosses);
			double pi_estimate = 4*total_hits/(double) number_of_tosses;
			printf("My estimation of PI : %f\n",pi_estimate);
		}
		/* END printPI enviCheck */

	}
	else
	{
		unsigned long long int hits = doit(number_of_tosses, number_of_threads,my_rank);
		MPI_Send(&hits, 1, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
