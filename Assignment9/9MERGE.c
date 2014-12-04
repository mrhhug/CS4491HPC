/* cs4491-HPC Fall 2014 
 * Michael Hug
 * Assignment 9
 * Merger sort on stampede with mpi
 */

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h> 

void err_sys (const char* message)
{
	printf ("%s\n", message);
	exit (0);
}
float* get_data (int num_keys,int num_threads)
{
	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	float* data = malloc (num_keys/num_threads * sizeof (float));
	if (data == NULL)
		err_sys ("malloc error");
		
	double x;	
	struct drand48_data randBuffer;
	srand48_r(time(NULL)-my_rank, &randBuffer);
	int i;
	for (i = 0; i < num_keys/num_threads; i++)
	{
		drand48_r(&randBuffer, &x);
		data[i] = x;
	}
	return data;
}
void serialMerge(float* data, int lower, int upper, int mid)
{
	float temp[upper - lower + 1];
	int i = lower;
	int j = mid + 1;
	int k = 0;
	while (i <= mid && j <= upper)
	{
		if (data[i] < data[j])
		{
			temp[k] = data[i];
			i++;
		}
		else
		{
			temp[k] = data[j];
			j++;
		}
		k++;
	}
	while (i <= mid)
	{
		temp[k] = data[i];
		i++;
		k++;
	}
	while (j <= upper)
	{
		temp[k] = data[j];
		j++;
		k++;;
	}
	for (k = 0; k < upper - lower + 1; k++)
		data[lower + k] = temp[k];
}
int is_sorted (float* data, int size)
{
	int i = 0;
	while (i < size-1 && data[i] <= data[i+1])
		i++;
	return i == size - 1;
}
int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int my_rank, num_threads;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_threads);
	int num_keys = 1600000;
	float* global_data = malloc (num_keys * sizeof (float));
	if(global_data==NULL)
		printf("big malloc error\n");
	if(my_rank == 0)
	{ 
		int i;
		for(i = 1;i< num_threads;i++)
		{
			float local_data [num_keys/num_threads];
			if(local_data==NULL)
				printf("small malloc error\n");
			MPI_Recv( &local_data, num_keys/num_threads, MPI_FLOAT,
				i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			serialMerge(global_data, 0, num_keys/num_threads*i, num_keys/num_threads/2*i);				
		}
		if (is_sorted(global_data,num_keys))
			    printf ("array is sorted\n");
	}
	else
	{
		float* data = get_data (num_keys,num_threads);
		if(data==NULL)
			printf("thread malloc error");
		serialMerge(data, 0, num_keys/num_threads, num_keys/num_threads/2);
		MPI_Send(data, num_keys/num_threads, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
