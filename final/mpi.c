/* 
 * 4491 HPC Fall 2014 Kennesaw State University
 * Michael Hug
 * hmichae4@students.kennesaw.edu
 * Programming portion of final
 * mpi edition
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <mpi.h>

long long int find_num_primes (long long int m,long long int n);

void err_sys (const char* message);

int main (int argc, char* argv[])
{
	int ierr = MPI_Init(&argc, &argv);
	if(ierr != MPI_SUCCESS)
		err_sys("mpi init error");
	if (argc < 2)
		err_sys ("missing command line argument");
	long long int num = strtoll (argv[1], 0 , 10);
	if (num < 0)
		err_sys ("invalid command line argument");

	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	long long int num_primes = find_num_primes (my_rank*num/world_size,(my_rank+1)*num/world_size);
	//printf ("%lld primes <= %lld\n", num_primes, num);
	long long int global_sum;
	MPI_Reduce(&num_primes,&global_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0,
			             MPI_COMM_WORLD);
	if(0==my_rank)
		printf("global sum = %lli\n",global_sum);
	ierr = MPI_Finalize();
	if(ierr != MPI_SUCCESS)
		err_sys("mpi finalize error");
	return 0;
}

int is_prime (long long int n)
{
	long long int max = (long long int)sqrt (n);
	long long int i = 2;
	while (i <= max && n % i != 0)
		i++;
	return i > max;
}

long long int find_num_primes (long long int m, long long int n)
{
	long long int result = 0;
	long long int i;
	if(2>m)
		m=2;
	//printf("%lli %lli\n",m,n);
	for (i = m; i <= n; i++)
		if (is_prime (i))
			result++;
	return result;
}

void err_sys (const char* message)
{
	printf ("%s\n", message);
	exit (1);
}
