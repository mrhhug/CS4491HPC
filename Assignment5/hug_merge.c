/*
 *Author : Michael Hug
 *Author email : hmichae4@students.kennesaw.edu
 *cs4491 Fall2014
 *Assignment 5
 */

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>

int thread_count = -1; 
pthread_mutex_t mut;

int get_max_threads()
{
	int max_string_size = 10; //including null char
	FILE *fp;
	char* ret;
	int max = -1;
	char str[max_string_size];
	fp = fopen("/proc/sys/kernel/threads-max","r");
	if(NULL == fp)
	{
		printf("error opening file thread max\n");
	}
	else
	{
		ret = fgets(str, max_string_size, fp);
		if (NULL == ret)
		{
			printf("file read error\n");
		}
		else
		{
			max = atoi(str);
		}
	}
	int retu = fclose(fp);
	if (0!=retu)
	{
		printf("file close error\n");
	}
	return max;
}
int * fill_Array_with_random_ints(int * pntr, int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		pntr[n]=4;//((float)rand()/(float)(RAND_MAX)) * 10000;
	}
	return pntr;
}
void print_int_array(int * pntr, int n)
{
	int i =0;
	for( ;i<n;i++)
	{
		printf("%d ",pntr[i]);
	}
	printf("\n");

}
void *monty(void* _)
{
/*	double distance_squared,x,y;
	long long int toss;
	long long int mycircleCount =0;
	for (toss = 0; toss < number_of_tosses_per_thread; toss++)
	{
		x = grabRand();
		y = grabRand();
		distance_squared = x*x + y*y;
		if (distance_squared <= 1)
		{
			mycircleCount++;
		}
	}
	pthread_mutex_lock(&mut);
	number_in_circle+=mycircleCount; 
	pthread_mutex_unlock(&mut); */
	return NULL;
}
float* get_data (int data_count)
{
	float* data = malloc (data_count * sizeof (float));
	srand(0);
	int i;
	for (i = 0; i < data_count; i++)
		data[i] = ((float)rand()/(float)(RAND_MAX)) * 10000;
	return data;
 }
int main(int argc, char *argv[])
{
	srand(5);
	int ret;
	int max_threads,size_of_array;
	int thread_count;

	if (3 != argc)
	{
		printf("I want 2 positional arguments : thread count & size of array to sort\n");
		return 1;
	}
	max_threads = get_max_threads();
	if(1 > max_threads)
	{
		printf("Are you on a posix complicant system?\n");
		return 2;
	}
	thread_count = atoi(argv[1]);
	if(1 > thread_count || thread_count>max_threads)
	{
		printf("Must supply an integer thread count between 1 and your system's max inclusivly\n");
		return 3;
	}
	size_of_array = atoll(argv[2]);
	if(1 > size_of_array|| size_of_array>pow(10,52))
	{
		printf("Supply a size_of_array between 1 and a Sexdecillion inclusivly\n");
		return 4;
	}
	if(0!=pthread_mutex_init(&mut, NULL))
	{
		printf("mutex creation fail\n");
		return 5;
	}
	
	fill_Array_with_random_ints(intArrayPntr,size_of_array);
	float* myfloat = get_data (size_of_array);
	print_int_array(myfloat,size_of_array);
	ret = pthread_mutex_destroy(&mut);	
	if(0!=ret)
	{
		printf("mutex destroy fail\n");
		return 9;
	}
	return 0;
}
