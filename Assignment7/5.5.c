/* cs4491-HPC Fall 2014 
 * Michael Hug
 * Assignment 7
 * Problem 5.5 
 */
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

void prin(int n, float ** A, float * R)
{
	int j, i;
	for(i=0; i<n; i++) 
	{
		for(j=0; j<n; j++)
		{
			printf("[%3.3f]",A[i][j]);
		}
		printf("[%3.3f]\n",R[i]);
	}
	printf("\n");
}
void err_sys (const char* message)
{
	printf ("%s\n", message);
	exit (0);
}
int main()
{
	int i,j,k,n=2000;
	//int i,j,k,n=5;
	float ratio;
	float **A;
	float *R;
	R = malloc(n * sizeof(float *));
	A = malloc(n * sizeof(float *));
	if(A == NULL)
		err_sys("First malloc error");
	for(i = 0; i < n; i++)
	{
		A[i] = malloc(n * sizeof(float));
		if(A[i] == NULL)
			err_sys("Second malloc error");
	}	
	//These would come in from a file or stream
	//the answer array
	for(i=0; i<n; i++)
	{
		R[i]=5.2+i;
	}
	//the meat array
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			A[i][j]=i+j+4;
		}
	}
	//prin(n,A,R);
	for(i=0;i<n-1;i++)  // these loops get the leading zero
	{
		#pragma omp parallel for private (k)
		for(j=i+1;j<n;j++)
		{
			ratio = A[j][i]/fabs(A[0][i]);
			//printf("%f %i\n",ratio,i);
			for(k=i;k<n;k++)
			{
				A[j][k]-=ratio*A[0][k];
			}
		}
		R[i]-=ratio*R[i];
	}
	//prin(n,A,R);
	for(i=0;i<n;i++) // these loops get the leading one 
	{
		#pragma omp parallel for private (k)
		for(j=i;j<n;j++)
		{
			ratio = A[i][i];
			//printf("%f\n",ratio);
			for(k=i;k<n;k++)
			{
				A[j][k]/=ratio;
			}
		}
		R[i]/=ratio*R[i];
	}
	//prin(n,A,R);
	for(i = 0; i < n; i++)
		free(A[i]);
	free(A);
	free(R);
	return(0);
}
