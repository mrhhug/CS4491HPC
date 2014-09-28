#include "histogram.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/***************************************************************************/

void err_sys (const char* message)
{
  printf ("%s\n", message);
  exit (0);
}

/****************************************************************************/

int* create_bin_counts (int num_bins)
{
  assert (num_bins > 0);
  int* bin_counts = malloc (num_bins * sizeof (int));
  if (bin_counts == NULL)
    err_sys ("malloc error");
  int i;
  for (i = 0; i < num_bins; i++)
    bin_counts[i] = 0;
  return bin_counts;
}

/***************************************************************************/

void determine_max_min (float data[], int data_count, float* max, float* min)
{
  assert (data_count > 0);
  assert (max != NULL);
  assert (min != NULL);
  *max = data[0];
  *min = data[0];
  int i;
  for (i = 1; i < data_count; i++)
  {
    if (data[i] < *min)
      *min = data[i];
    if (data[i] > *max)
      *max = data[i];
  }
}

/****************************************************************************/

float* determine_bin_maxes (float min, float max, int data_count, int num_bins)
{
  assert (min <= max);
  assert (data_count > 0);
  assert (num_bins > 0);
  float* bin_maxes = malloc (data_count * sizeof (float));
  if (bin_maxes == NULL)
    err_sys ("malloc error");
  float length = (max - min) / num_bins;
  float endpoint = max;
  int i;
  for (i = num_bins - 1; i >= 0; i--)
  {
    bin_maxes [i] = endpoint;
    endpoint -= length;
  }
  return bin_maxes;
}   

/************************************************************************/

void* thread_determine_bin_counts (void* rank)
{
  int my_rank = (int)rank;
  int my_num_data = data_count / num_threads;
  int my_first = my_num_data * my_rank;
  int my_last = my_first + my_num_data;
  int* my_bins = create_bin_counts();
  float interval_length = bin_maxes[1] - bin_maxes[0];
  float left_endpoint = bin_maxes[0] - interval_length;
  int i;
  for (i = my_first; i < my_last; i++)
  {
    int index = (int)((data[i] - left_endpoint) / interval_length);
    my_bins[index]++;
  }
  int result = pthread_mutex_lock (&bin_merge_mutex);
  if (result != 0)
    err_sys ("pthread mutex lock error");
  for (i = 0; i < num_bins; i++)
    bin_counts[i] += my_bins[i];
  result = pthread_mutex_unlock (&bin_merge_mutex);
  if (result != 0)
    err_sys ("pthread mutex unlock error");
  free (my_bins);
}
void determine_bin_counts ()
{

  pthread_t* thread_handles = malloc (num_threads * sizeof(pthread_t));
  if (thread_handles == NULL)
    err_sys ("malloc error");
  int result;
  int i = 0;
  for (i = 0; i < num_threads; i++)
  {
    result = pthread_create (&create_handles[i], NULL, 
			     thread_determine_bin_counts,
			     (void*)i);
    if (result != 0)
      err_sys ("pthread create error");
  }
  for (i = 0; i < num_threads; i++)
  {
    result = pthread_join (thread_handles[i], NULL);
    if (result != 0)
      err_sys ("pthread join error");
  }
  free (thread_handles);
}

/*************************************************************************/

histogram* create_histogram (int num_intervals, int data_size, 
			     float* input_data)
{
  assert (num_intervals > 0);
  assert (data_size > 0);
  assert (input_data != NULL);
  histogram* hist = malloc (sizeof(histogram));
  if (hist == NULL)
    err_sys ("malloc error");
  hist -> num_bins = num_intervals;
  float max;
  float min;
  determine_max_min (input_data, data_size, &max, &min);
  hist -> bin_maxes = determine_bin_maxes (min, max, data_size, num_intervals);
  hist -> bin_counts = determine_bin_counts (input_data, data_size, 
					     num_intervals, hist -> bin_maxes);
  return hist;
}

/***************************************************************************/

void display_histogram (const histogram* hist)
{
  assert (hist != NULL);
  float length = hist -> bin_maxes[1] - hist -> bin_maxes[0];
  float left = hist -> bin_maxes[0] - length;
  int i;
  for (i = 0; i < hist -> num_bins; i++)
  {
    printf ("%f - %f: %d\n", left, hist -> bin_maxes[i], hist -> bin_counts[i]);
    left += length;
  }
}

/****************************************************************************/

void destroy_histogram (histogram* hist)
{
  assert (hist != NULL);
  if (hist -> bin_maxes != NULL)
    free (hist -> bin_maxes);
  if (hist -> bin_counts != NULL)
    free (hist -> bin_counts);
  free (hist);
}
