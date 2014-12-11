#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <pthread.h>

#define thread_count 5

long long int num;

void* find_num_primes (void*);

void err_sys (const char* message);

int main (int argc, char* argv[])
{
	if (argc < 2)
		err_sys ("missing command line argument");
	num = strtoll (argv[1], 0 , 10);
	if (num < 0)
		err_sys ("invalid command line argument");
	pthread_t threads[thread_count];
	long long int i;
	for (i=0; i<thread_count; i++)
		if(pthread_create(&threads[i], NULL,find_num_primes, (void *)i))
			err_sys("error creating thread");
	void* ret_val;
	long long int global_sum = 0;
	for (i=0; i<thread_count; i++)
	{
		if(pthread_join(threads[i],&ret_val))
			err_sys("error creating thread");
		int local_sum=(long long int) ret_val;
		global_sum+=local_sum;

	}
	printf ("%lli primes <= %lli\n", global_sum, num);
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

void* find_num_primes (void* datum)
{
	long long int my_rank = (long long int) datum;
	long long int result = 0;
	long long int i = my_rank*num/thread_count;
	long long int n = (my_rank+1)*num/thread_count;
	if(2>i)
		i=1;
	for (i+=1; i <= n; i++)
		if (is_prime (i))
			result++;
	return (void *) result;
}

void err_sys (const char* message)
{
	printf ("%s\n", message);
	exit (1);
}
