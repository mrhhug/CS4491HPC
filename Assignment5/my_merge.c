#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

float* get_data (int data_count);

void err_sys (const char* message);

void* thread_merge_sort (void* args);

void merge (float* data, int lower, int upper, int mid);

int is_sorted (float* data, int size);

typedef struct
{
	float* data;
	int left;
	int right;
} subarray;

typedef struct
{
	float* data;
	float* leftArray;
	float* rightArray;
	int startingPointonDataArray;
	int leftArraycount;
	int rightArraycount;
} submerge;

int main (int argc, char* argv[])
{
	long long array_size = 1000000;
	float* data = get_data (array_size);
	pthread_t array_thread;
	subarray entire_array = {data, 0, array_size - 1};
	int result = pthread_create (&array_thread, NULL, thread_merge_sort,
			(void*)(&entire_array));
	if (result != 0)
		err_sys ("pthread create error");
	result = pthread_join (array_thread, NULL);
	if (result != 0)
		err_sys ("pthread join error");
	if (is_sorted (data, array_size))
		printf ("array is sorted\n");
	else
		printf ("array is not sorted\n");
	free(data);  //This was added. 
	return 0;
}

void* thread_merge_sort (void* args)
{
	subarray* params = (subarray*)args;
	if (params -> left < params -> right)
	{
		pthread_t right_thread;
		int right_created = 0;
		int mid = (params -> left + params -> right) / 2;
		if (params -> left < mid)
		{
			subarray left_array = {params -> data, params -> left, mid};
			thread_merge_sort ((void*)(&left_array));
		}
		if (mid < params -> right)
		{
			right_created = 1;
			subarray right_array = {params -> data, mid + 1, params -> right};
			int result = pthread_create (&right_thread, NULL, thread_merge_sort,
					(void*)(&right_array));
			if (result != 0)
				err_sys ("pthread create error");
		}
		if (right_created)
		{
			int result = pthread_join (right_thread, NULL);
			if (result != 0)
				err_sys ("pthread join error");
		}  
		merge (params -> data, params -> left, params -> right, mid);
	}
	return NULL;
}

void err_sys (const char* message)
{
	printf ("%s\n", message);
	exit (0);
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


void* parallelMerge (void* args)
{
	submerge* params = (submerge*)args;
	int leftIndex =0;
	int rightIndex =0;

	/* START debug printing */
	char* debug=getenv("DEBUG");
	if(debug!=NULL && debug[0]=='1')
	{
		int i;
		printf("left array : ");
		for(i=0;i<params->leftArraycount;i++)
			printf("%1.0f ",params->leftArray[i]);
		printf("\n");
		printf("right array : ");
		for(i=0;i<params->rightArraycount;i++)
			printf("%1.0f ",params->rightArray[i]);
		printf("\n");
	}
	/* END debug printing */

	//the decision loop for when there are items left in both arrays
	while(params->leftArraycount>leftIndex && params->rightArraycount>rightIndex)
	{
		if(params->leftArray[leftIndex] < params->rightArray[rightIndex])
			params->data[params->startingPointonDataArray++]=params->leftArray[leftIndex++];
		else
			params->data[params->startingPointonDataArray++]=params->rightArray[rightIndex++];
	}

	//figure out who still has items left in their array, then grab all
	if(params->leftArraycount==leftIndex)
		while(params->rightArraycount>rightIndex)
			params->data[params->startingPointonDataArray++]=params->rightArray[rightIndex++];
	else
		while(params->leftArraycount>leftIndex)
			params->data[params->startingPointonDataArray++]=params->leftArray[leftIndex++];    
	return NULL;
}
void merge (float* data, int lower, int upper, int mid)
{	
	//we had to have some type of threshold
	//we don't want to spawn threads to
	//merge sublists of 0 or 1. I took it up to 5
	if(upper-lower<=4)
		serialMerge(data,lower,upper,mid);
	else
	{
		int x = (lower+mid)/2; // this index will be the mid of our 2 merges, we need to never consider this index again
		int leftLeftArrayCount=0;
		int leftRightArrayCount=0; 
		int rightLeftArrayCount=0; // first one here will be our mid
		int rightRightArrayCount=0;

		/* START debug printing */
		char* debug=getenv("DEBUG");
		if(debug!=NULL && debug[0]=='1')
		{
			printf("Status before  merge : \n");
			int kk = lower;
			for ( ; kk<=upper;kk++)
			{
				printf("%1.0f ",data[kk]);
			}
			printf("\nlow is %d\n",lower);
			printf("Middle is %d\n",mid);
			printf("upper is %d\n",upper);
			printf("mid num: %1.0f\n",data[x]);
		}
		/* END debug printing */

		//go through and read each value once
		int i;
		for(i=lower;i<=upper;i++)
		{
			if(i<=mid) //we are on x_left array
			{
				if(data[i]<data[x])
					leftLeftArrayCount++;
				else
					rightLeftArrayCount++;
			}
			else // right
			{
				if(data[i]<data[x])
					leftRightArrayCount++;
				else
					rightRightArrayCount++;
			}
		}

		//allocate temp arrays
		float* ll = malloc(sizeof(float)*leftLeftArrayCount);
		float* lr = malloc(sizeof(float)*leftRightArrayCount); 
		float* rl = malloc(sizeof(float)*--rightLeftArrayCount); //mind the mid
		float* rr = malloc(sizeof(float)*rightRightArrayCount);

		//write temps
		int localCount = lower;
		for(i=0; i<leftLeftArrayCount;i++,localCount++)
			ll[i] = data[localCount];
		localCount++; // account for mid
		for(i=0; i<rightLeftArrayCount;i++,localCount++)
			rl[i] = data[localCount];
		for(i=0; i<leftRightArrayCount;i++,localCount++)
			lr[i] = data[localCount];
		for(i=0; i<rightRightArrayCount;i++,localCount++)
			rr[i] = data[localCount];

		//write to mid
		data[upper-rightLeftArrayCount-rightRightArrayCount]=data[x];

		//create structs
		submerge left_merge_data = {data, ll,lr, lower,leftLeftArrayCount,leftRightArrayCount};
		submerge right_merge_data = {data, rl,rr, upper-rightLeftArrayCount-rightRightArrayCount+1,rightLeftArrayCount,rightRightArrayCount};

		//create one thread. This thread will do it's own half
		pthread_t mergeThread;
		int result = pthread_create (&mergeThread, NULL, parallelMerge,(void*)(&right_merge_data));
		if (result != 0)
			err_sys ("pthread create error");
		parallelMerge((void*) &left_merge_data);

		//wait for other thread
		result = pthread_join (mergeThread, NULL);
		if (result != 0)
			err_sys ("pthread join error");
		
		/* START debug printing */
		if(debug!=NULL && debug[0]=='1')
		{
			printf("Status after merge : \n");
			int kk = lower;
			for ( ; kk<=upper;kk++)
			{
				printf("%1.0f ",data[kk]);
			}
			printf("\n\n\n");
		}
		/* END debug printing */
		free(ll);
		free(lr);
		free(rl);
		free(rr);
	}
}

float* get_data (int data_count)
{
	float* data = malloc (data_count * sizeof (float));
	if (data == NULL)
		err_sys ("malloc error");
	srand(time(NULL));

	/* START debug override */
	char* debug=getenv("DEBUG");
	if(debug!=NULL && debug[0]=='1')
		srand(0);
	/* END debug override */

	int i;
	for (i = 0; i < data_count; i++)
		data[i] = ((float)rand()/(float)(RAND_MAX)) * 10000;
	return data;
}

int is_sorted (float* data, int size)
{
	int i = 0;
	while (i < size-1 && data[i] <= data[i+1])
		i++;
	return i == size - 1;
}
