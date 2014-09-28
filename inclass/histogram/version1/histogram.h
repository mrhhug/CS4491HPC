#ifndef _HISTOGRAM_H
#define _HISTOGRAM_H

typedef struct
{
  int num_bins;
  float* bin_maxes;
  int* bin_counts;
} histogram;

histogram* create_histogram (int num_intervals, int data_size, 
			     float* input_data);

void display_histogram (const histogram* hist);

void destroy_histogram (histogram* hist);

#endif
