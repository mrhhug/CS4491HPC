#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

double *global_vector;
int thread_count,m,n;
double** A;
double *y;
double *x;

double drand()
{
  double high = 999.00;
  double low = 0.0;
  double dub = ((double)rand()*(high - low)) / (double)RAND_MAX + low;
  return dub;
}

void* Pth_mat_vect(void* rank)
{
  long my_rank = (long) rank;
  int i, j;
  int local_m = m/thread_count;
  int my_first_row = my_rank*local_m;
  int my_last_row = (my_rank+1)*local_m - 1;
  for (i = my_first_row; i <= my_last_row; i++)
  {
    y[i] = 0.0;
    for (j = 0; j < n; j++)
    {
      y[i] += A[i][j]*x[j];
    }
  }
  return NULL;
}

void* michael_mat_vect(void* rank)
{
  long my_rank = (long) rank;
  int local_m = (m*n/thread_count);
  int i;
  int my_start = my_rank*local_m;
  int my_end = my_start+local_m;
  for (i = my_start; i < my_end; i++)
  {
    y[i/m] += global_vector[i]*x[i%m];
  }
  return NULL;
} 

void michael_Pth_setup()
{
  long rank = 0;
  pthread_t threads[thread_count];
  global_vector=malloc(sizeof(double)*(m*n));
  int i,j;
  for(i=0;i<m;i++)
  {
    for(j=0;j<n;j++)
    {
      global_vector[(m*i)+j] = A[i][j];
    }
  }
  for (i=0; i<thread_count; i++)
  {
    if(pthread_create(&threads[i], NULL, michael_mat_vect, (void *)rank++))
    {
      printf("error creating thread");
      exit(1);
    }
  }
  for (i=0; i<thread_count; i++)
  {
    pthread_join(threads[i],NULL);
  }
}
void Pth_setup()
{
  pthread_t threads[thread_count];
  int i;
  long rank = 0;
  for (i=0; i<thread_count; i++)
  {
    if(pthread_create(&threads[i], NULL,Pth_mat_vect, (void *)rank++))
    {
      printf("error creating thread");
      exit(1);
    }
  }
  for (i=0; i<thread_count; i++)
  {
    pthread_join(threads[i],NULL);
  }
}

void  buildY()
{ 
  y=malloc(sizeof(double)*m);
  int i;
  for(i=0;i<m;i++)
  {
    y[i]=0.0;
  }
}

void  buildX()
{ 
  x=malloc(sizeof(double)*m);
  int i;
  for(i=0;i<m;i++)
  {
    x[i]=drand();
  }
}
void buildA()
{
  A = (double**) malloc(m*sizeof(double*));  
  int i,j;
  for (i = 0; i < m; i++)  
  {
    A[i] = (double*) malloc(n*sizeof(double));
  }
  for(i=0;i<m;i++)
  {
    for(j=0;j<n;j++)
    {
      A[i][j]=drand();
    }
  }
}
void freeA()
{
  int i;
  for (i = 0; i < m; i++)
  {  
       free(A[i]);  
  }  
  free(A);
}
void printX()
{
  int i;
  for(i=0;i<m;i++)
  {
    printf("%1.3f\n",x[i]);
  }
  printf("\n");
}
void printY()
{
  int i;
  for(i=0;i<m;i++)
  {
    printf("%1.3f\n",y[i]);
  }
}
void printA()
{
  int i,j;
  for(i=0;i<m;i++)
  {
    for(j=0;j<n;j++)
    {
      printf("%1.3f ",A[i][j]);
    }
    printf("\n");
  }
}
int main()
{
  thread_count = 8;
  m = 1024*8;
  n = 1024*8;
  buildY();
  buildX();
  buildA();
  
  clock_t start, fin;
  start = clock();
  Pth_setup();
  fin = clock();
  unsigned long long time = fin-start;
  printf("book's implementation :  %llu\n",time);

  //printY();

  free(y);
  buildY();
  //printY();
  //printf("\n");
  
  start = clock();
  Pth_setup();
  fin = clock();
  time = fin-start;
  printf("Michael's implementation : %llu\n",time);
  michael_Pth_setup();

  //printY();

  freeA();
  free(x);
  free(y);
  free(global_vector);
  return 0;
}
