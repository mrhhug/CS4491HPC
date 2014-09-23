#include "histogram.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

float* get_data (int* data_count);

void error (const char* message);

int main (int argc, char* argv[])
{
  if (argc < 2)
    error ("invalid number of command line arguments");
  int num_bins = atoi (argv[1]);
  if (num_bins <= 0)
    error ("invalid data_count command line argument");
  int data_count;
  float* data = get_data (&data_count); 
  histogram* hist = create_histogram (num_bins, data_count, data);
  display_histogram (hist);
  free (data);
  destroy_histogram (hist);
  return 0;
}

 float* get_data (int* data_count)
 {
   assert (data_count != NULL);
   *data_count = 100000;
   float* data = malloc (*data_count * sizeof (float));
   if (data == NULL)
     error ("malloc error");
   srand(0);
   int i;
   for (i = 0; i < *data_count; i++)
     data[i] = ((float)rand()/(float)(RAND_MAX)) * 10000;
   return data;
 }

void error (const char* message)
{
  printf ("%s\n", message);
  exit (0);
}
