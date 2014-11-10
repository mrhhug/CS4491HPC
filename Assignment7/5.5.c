#include<stdio.h>
#include<math.h>
#include<stdlib.h>

void prin(int n, float A[n][n])
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
	
	float **array;
	array = malloc(n * sizeof(float *));
	if(array == NULL)
		printf("out of memory\n");
	for(i = 0; i < n; i++)
	{
		array[i] = malloc(n * sizeof(float));
		if(array[i] == NULL)
			printf("out of memory\n");
	}	




	float A[n][n];
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			A[i][j]=i+j+4;
		}
	}
//	prin(n,a);
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
		free(array[i]);
	free(array);

	return(0);
}
