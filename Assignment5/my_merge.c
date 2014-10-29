#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


void* thread_merge_sort (void* args);

void err_sys (const char* message);

void serialMerge(float* data, int lower, int upper, int mid);

void* parallelMerge (void* args);

void merge (float* data, int lower, int upper, int mid);

float* get_data (int data_count);

int is_sorted (float* data, int size);

typedef struct
{
	float* data;
	int left;
	int right;
} subarray;

typedef struct
{
	float* temp;
	float* data;
	int leftCursor;
	int leftLength;
	int rightCursor;
	int rightLength;
	int dataWriteCursor;
} submerge;

int main (int argc, char* argv[])
{
	long long array_size = 1000000;

	/* START ARRAYSIZE override */
	char* arraysize=getenv("ARRAYSIZE");
	if(arraysize!=NULL)
	{
		array_size=atoll(arraysize);
	}
	/* END ARRAYSIZE override */

	/* START debug override */
	char* debug=getenv("DEBUG");
	if(debug!=NULL && debug[0]=='1')
	{
		array_size = 16;
	}
	/* END debug override */

	float* data = get_data (array_size);
	pthread_t array_thread;
	subarray entire_array = {data, 0, array_size - 1};
	int result;
	result = pthread_create (&array_thread, NULL, thread_merge_sort,
			(void*)(&entire_array));
	if (result != 0)
		err_sys ("pthread create error");
	result = pthread_join (array_thread, NULL);
	if (result != 0)
		err_sys ("pthread join error");
	if (is_sorted (data, array_size))
	{
		/* START sucess print override */
		char* debug=getenv("SUPPRESSSUCCES");
		if(debug!=NULL && debug[0]=='1')
			;
		else
			printf ("array is sorted\n");
		/* END sucess print override */
	}
	else
		printf ("array is not sorted\n");
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

	/* START debug printing */
	char* debug=getenv("DEBUG");
	if(debug!=NULL && debug[0]=='1')
	{
		int i;
		printf("\nData write cursor : %d\n ",params->dataWriteCursor);
		printf("left side begin:%i end:%i  elements : ",params->leftCursor,params->leftLength);
		for(i=params->leftCursor;i<params->leftLength+params->leftCursor;i++)
			printf("%1.0f ",params->temp[i]);
		printf("\n ");
		printf("right side begin:%i end:%i  elements : ",params->rightCursor,params->rightLength);
		for(i=params->rightCursor;i<params->rightLength+params->rightCursor;i++)
			printf("%1.0f ",params->temp[i]);
	}
	/* END debug printing */

	//the decision loop for when there are items left in both arrays
	while(params->leftLength > 0 && params->rightLength > 0)
	{
		if(params->temp[params->leftCursor] < params->temp[params->rightCursor])
		{
			params->data[params->dataWriteCursor++]=params->temp[params->leftCursor++];
			params->leftLength--;
		}
		else
		{
			params->data[params->dataWriteCursor++]=params->temp[params->rightCursor++];
			params->rightLength--;
		}
	}

	//figure out who still has items left in their array, then grab remaining
	if(params->leftLength == 0)
		while(params->rightLength > 0 )
		{
			params->data[params->dataWriteCursor++]=params->temp[params->rightCursor++];
			params->rightLength--;
		}
	else
		while(params->leftLength > 0)
		{
			params->data[params->dataWriteCursor++]=params->temp[params->leftCursor++];
			params->leftLength--;
		}
	return NULL;
}
void merge (float* data, int lower, int upper, int mid)
{	
	//we had to have some type of threshold
	//we don't want to spawn threads to
	//merge sublists of 0 or 1.
	int thres = 2024;

	/* START THRES override */
	char* enviThre=getenv("THRES");
	if(enviThre!=NULL)
	{
		thres=atoi(enviThre);
	}
	/* END THRES override */

	if(upper-lower<=thres)
		serialMerge(data,lower,upper,mid);
	else
	{
		int x = (lower+mid)/2; // this index will be the mid of our 2 merges
		int LLcount=x-lower;
		int LRcount=mid-x; /// could be changed +- 1
				
		//temp array
		float* temp = malloc(sizeof(float)* (upper-lower));

		/* START debug printing */
		char* debug=getenv("DEBUG");
		if(debug!=NULL && debug[0]=='1')
		{
			printf("Status before  merge : \n");
			int kk = 0;
			for ( ; kk<=16;kk++)
			{
				printf("%1.0f ",data[kk]);
			}
			printf("\n");
			for ( kk=lower; kk<=upper;kk++)
			{
				printf("%1.0f ",data[kk]);
			}
			printf("\nlow is %d\n",lower);
			printf("Middle is %d\n",mid);
			printf("upper is %d\n",upper);
			printf("mid num: %1.0f",data[x]);
		}
		/* END debug printing */

		//copy and count
		int i=lower;
		int j=0;
		for( ;j<LLcount;j++,i++)
			temp[j]=data[i];
		for(++i;j<LLcount+LRcount;i++,j++)
			temp[j]=data[i];
		for( ;data[i]<=data[x] && i<=upper;i++,j++)
			temp[j]=data[i];
		int RLcount=j-LRcount-LLcount; 
		int RRcount=upper-mid-RLcount;	
		for( ;i<=upper;i++)
			temp[j++]=data[i];

		/* START debug printing */
		if(debug!=NULL && debug[0]=='1')
		{
			printf("\nleft left count%d\n",LLcount);
			printf("left right count%d\n",LRcount);
			printf("right left count%d\n",RLcount);
			printf("right right count%d\n",RRcount);
		}
		/* END debug printing */

		data[lower+LLcount+RLcount]=data[x];
		submerge left_merge_data = {temp, data, 0, LLcount, LLcount+LRcount, RLcount, lower};
		submerge right_merge_data = {temp, data, LLcount, LRcount ,LLcount+LRcount+RLcount, RRcount,lower+LLcount+RLcount+1};//skip over new mid

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
			printf("\ntemp array : ");
			for(i=0;i<upper-lower;i++)
			{

				printf("%1.0f ",temp[i]);
			}
			printf("\nStatus after merge : \n");
			int kk =0 ;
			for ( ; kk<=16;kk++)
			{
				printf("%1.0f ",data[kk]);
			}
			printf("\n");
			for ( kk=lower; kk<=upper;kk++)
			{
				printf("%1.0f ",data[kk]);
			}
			printf("\n\n\n");
		}
		/* END debug printing */
		//free(temp);
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
	{
		int var = time(NULL)%100;
		printf("timeer = %i",var);
		srand(0);
		//srand(time(NULL));
	}
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
