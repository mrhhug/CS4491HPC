#include <time.h>
#include <stdio.h>

int fibP(int n)
{ 
	int i, j; 
	if (n<2) 
		return n; 
	else
	{ 
		#pragma omp task shared(i) 
		i=fibP(n-1); 
		#pragma omp task shared(j) 
		j=fibP(n-2); 
		#pragma omp taskwait 
		return i+j; 
	} 
}
int fib(int n)
{
	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;
	else
		return (fib(n-1) + fib(n-2));
} 

int main()
{
	int n =55;
	clock_t start, end, diff;

	start = clock();
	fibP(n);	
	end = clock();
	diff = end-start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("OpenMP version time was : %d\n", msec);	

	start = clock();
	fib(n);	
	end = clock();
	diff = end-start;
	msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Simple recursion version time was : %d\n", msec);	
	return 0;
}
