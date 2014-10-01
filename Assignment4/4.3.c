#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

double delta=1;
int thread_count = -1; 
double area=0;
pthread_mutex_t mut;

struct my_thread_rank
{
	int rank;
};
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
	int retur = fclose(fp);
	if (0!=retur)
	{
		printf("file close error\n");
	}
	return max;
}
double trap_area(double y1,double y2,double delta)
{
	return .5*(y1+y2)*delta;
}
double eval_function(double x)
{
	return sqrt(1-x*x);
}
void* my_thread(void * data)
{
	struct my_thread_rank *info = data;
	int my_rank = info ->rank;
	double lower_bound=my_rank * 1/(double)thread_count;
	double upper_bound=(1+my_rank) * 1/(double)thread_count;
	double y1;
	double y2;
	double my_area = 0.0;
	double i;
	y2 = eval_function(lower_bound);
	for(i=lower_bound+delta;i<=upper_bound;i+=delta)
	{
		y1=y2;
		y2=eval_function(i);
		my_area += trap_area(y1,y2,delta);
	}
	pthread_mutex_lock(&mut);
	area+=my_area; 
	pthread_mutex_unlock(&mut);
	free(info);
	return 0;

}
int main(int argc, char *argv[])
{
	int i, ret;
	int max_threads;
	pthread_t* threads;

	if (3 != argc)
	{
		printf("I want 2 positional arguments : thread count & the delta of the trap(ex .00001)\n");
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
	delta = atof(argv[2]);
	if(0 > delta || 1 < delta)
	{
		printf("Supply a delta between 1 and zero\n");
		return 4;
	}
	if(0!=pthread_mutex_init(&mut, NULL))
	{
		printf("mutex creation fail\n");
		return 5;
	}
	threads = malloc(thread_count * sizeof(pthread_t));
	if (NULL == threads)
	{
		printf("pthread malloc failure\n");
		return 6;
	}
	for(i=0;i<thread_count;i++)
	{
		struct my_thread_rank *info = malloc(sizeof(struct my_thread_rank));
		if(NULL==info)
		{
			printf("strut malloc failure");
			return 7;
		}
		info->rank = i;
		ret = pthread_create(&threads[i], NULL, my_thread,info);
		if(0!=ret)
		{
			printf("thread creation fail\n");
			return 8;
		}
	}
	for(i=0;i<thread_count;i++)
	{
		ret = pthread_join(threads[i],NULL);
		if(0!=ret)
		{
			printf("thread join fail\n");
			return 9;
		}
	}
	ret = pthread_mutex_destroy(&mut);	
	if(0!=ret)
	{
		printf("mutex destroy fail\n");
		return 10;
	}
	free(threads); //void return
	printf("Your trapezoidal estimation of PI: \t%.15f\n",area*4);
	printf("math.h's macro for PI: \t\t\t%.15f\n", M_PI);

	return 0;
}
