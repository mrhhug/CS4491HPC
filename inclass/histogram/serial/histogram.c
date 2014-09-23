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

int* determine_bin_counts (float data[], int data_count, int num_bins, 
			   float bin_maxes[])
{
  int* bin_counts = malloc (num_bins * sizeof (int));
  if (bin_counts == NULL)
    err_sys ("malloc error");
  int i;
  for (i = 0; i < num_bins; i++)
    bin_counts[i] = 0;
  float interval_length = bin_maxes[1] - bin_maxes[0];
  float left_endpoint = bin_maxes[0] - interval_length;
  for (i = 0; i < data_count; i++)
  {
    int index = (int)((data[i] - left_endpoint) / interval_length);
    bin_counts[index]++;
  }
return bin_counts;
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
