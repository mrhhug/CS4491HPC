#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <omp.h>

long long int find_num_primes (long long int m, long long int n);

void err_sys (const char* message);

int main (int argc, char* argv[])
{
	if (argc < 2)
		err_sys ("missing command line argument");
	long long int num = strtoll (argv[1], 0 , 10);
	if (num < 0)
		err_sys ("invalid command line argument");
	int world_size, my_rank;
	long long int global_sum = 0;
	#pragma omp parallel reduction(+:global_sum) private(my_rank)
	{
		world_size=omp_get_num_threads();
		my_rank = omp_get_thread_num();
		long long int num_primes = find_num_primes (my_rank*num/world_size,(my_rank+1)*num/world_size);
		global_sum = global_sum + num_primes;
	}
	printf ("%lld primes <= %lld\n", global_sum, num);
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
