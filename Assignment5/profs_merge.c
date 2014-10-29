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

void merge (float* data, int lower, int upper, int mid)
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

 float* get_data (int data_count)
 {
   float* data = malloc (data_count * sizeof (float));
   if (data == NULL)
     err_sys ("malloc error");
   srand(0);
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
