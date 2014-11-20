/* cs4491-HPC Fall 2014 
 * Michael Hug
 * Assignment 8
 * Monte Carlo PI estimation on stampede
 */

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h> 

int number_of_threads = 1;
int number_of_tosses = 1;

double grabRand() 
{
	double div = RAND_MAX;
	return -1 + (rand() / div);
}
void doit()
{
	int global_number_in_circle = 0;
	srand(time(NULL));
	#pragma omp parallel num_threads(number_of_threads)
	{
		int private_number_in_circle = 0;
		double y,x;
		int toss;
		for (toss = 0; toss < number_of_tosses/number_of_threads; toss++)
		{
			x = grabRand();
			y = grabRand();
				/* START debug override */
				char* debug=getenv("DEBUG");
				if(debug!=NULL && debug[0]=='1')
				{
					printf("%f %f\n",x,y);
				}
				/* END debug override */
			if (x*x + y*y <= 1)
			{
				private_number_in_circle++;
			}
		}
		#pragma omp atomic
		global_number_in_circle+=private_number_in_circle;
	}
	/* START printPI enviCheck */
	char* debug=getenv("PRINTPI");
	if(debug!=NULL && debug[0]=='1')
	{
		printf("Number in Circle :%d\n",global_number_in_circle);
		printf("Number of tosses :%d\n",number_of_tosses);
		double pi_estimate = 4*global_number_in_circle/(double) number_of_tosses;
		printf("My estimation of PI : %f\n",pi_estimate);
	}
	/* END printPI enviCheck */
}
int main(int argc, char *argv[])
{
	if(argc ==3)
	{
		number_of_threads = atoi(argv[1]);
		number_of_tosses = atoi(argv[2]);
	}
		/* START debug override */
		char* debug=getenv("DEBUG");
		if(debug!=NULL && debug[0]=='2')
		{
			printf("%i %i\n",number_of_threads,number_of_tosses);
		}
		/* END debug override */
	clock_t start = clock();
	doit();
	clock_t end = clock();
	printf("%10.3f ", (double)(end-start) / CLOCKS_PER_SEC);
	return 0;
}
