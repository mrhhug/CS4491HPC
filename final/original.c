#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

long long int find_num_primes (long long int n);

void err_sys (const char* message);

int main (int argc, char* argv[])
{
	if (argc < 2)
		err_sys ("missing command line argument");
	long long int num = strtoll (argv[1], 0 , 10);
	if (num < 0)
		err_sys ("invalid command line argument");
	long long int num_primes = find_num_primes (num);
	printf ("%lld primes <= %lld\n", num_primes, num);
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

long long int find_num_primes (long long int n)
{
	long long int result = 0;
	long long int i;
	for (i = 2; i <= n; i++)
		if (is_prime (i))
			result++;
	return result;
}

void err_sys (const char* message)
{
	printf ("%s\n", message);
	exit (1);
}
