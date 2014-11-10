#include<stdio.h>
#include<math.h>
#include<stdlib.h>

void prin(int n, float ** A)
{
	int j, i;
	for(i=0; i<n; i++) 
	{
		for(j=0; j<n; j++)
		{
			printf("[%3.3f]",A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
int main()
{
	int i,j,k,n=50;
	
	float **A;
	A = malloc(n * sizeof(float *));
	if(A == NULL)
		printf("out of memory\n");
	for(i = 0; i < n; i++)
	{
		A[i] = malloc(n * sizeof(float));
		if(A[i] == NULL)
			printf("out of memory\n");
	}	




	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			A[i][j]=i+j+4;
		}
	}
	prin(n,A);
//	printf("\n");
	
	float ratio;
	for(k=0;k<n;k++)
	{
		for(i=k+1;i<n;i++)
		{
			ratio = A[i][k]/fabs(A[0][k]);
			//printf("%f\n",ratio);
			for(j=k;j<n;j++)
			{
				A[i][j]-=ratio*A[0][j];
			}
		}
	}
	for(i=0; i<n; i++) 
	{
		for(j=0; j<n; j++)
		{
		//	printf("[%3.3f]",A[i][j]);
		}
		//printf("\n");
	}
	//A[2][3]=999;




	for(i = 0; i < n; i++)
		free(A[i]);
	free(A);

	return(0);
}
