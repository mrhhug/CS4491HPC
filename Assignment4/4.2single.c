#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>

double grabRand() 
{
	double div = RAND_MAX / 2;
	return -1 + (rand() / div);
}
int main(int argc, char *argv[])
{
	int number_of_tosses = 100000000;
	int number_in_circle;
	srand(time(NULL));
	int toss;
	double distance_squared;
	double y,x;
	for (toss = 0; toss < number_of_tosses; toss++)
	{
		x = grabRand();
		y = grabRand();
		//printf("%f %f\n",x,y);
		distance_squared = x*x + y*y;
		if (distance_squared <= 1)
		{
			number_in_circle++;
		}
	}
	printf("%d\n",number_in_circle);
	printf("%d\n",number_of_tosses);
	double pi_estimate = 4*number_in_circle/(double) number_of_tosses;
	printf("My estimation of PI : %f\n",pi_estimate);
	return 0;
}
