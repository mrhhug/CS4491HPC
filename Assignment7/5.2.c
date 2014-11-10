/* cs4491-HPC Fall 2014 
 * Michael Hug
 * Assignment 7
 * Problem 5.2 
 */
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define number_of_threads 256
#define shared_number_of_tosses 1000000
double grabRand() 
{
	double div = RAND_MAX;
	return -1 + (rand() / div);
}
int main(int argc, char *argv[])
{
	int global_number_in_circle = 0;
	srand(time(NULL));
	#pragma omp parallel num_threads(number_of_threads)
	{
		int private_number_in_circle = 0;
		double y,x;
		int toss;
		for (toss = 0; toss < shared_number_of_tosses; toss++)
		{
			x = grabRand();
			y = grabRand();
			//printf("%f %f\n",x,y);
			if (x*x + y*y <= 1)
			{
				private_number_in_circle++;
			}
		}
		#pragma omp atomic
		global_number_in_circle+=private_number_in_circle;
	}
	printf("Number in Circle :%d\n",global_number_in_circle);
	printf("Number of tosses :%d\n",number_of_threads*shared_number_of_tosses);
	double pi_estimate = 4*global_number_in_circle/(double) (number_of_threads*shared_number_of_tosses);
	printf("My estimation of PI : %f\n",pi_estimate);
	return 0;
}
