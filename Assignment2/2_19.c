/* cs4491-HPC Fall 2014 
 * Michael Hug
 * Assignment 2
 * Problem 2.19 
 */

#include <stdio.h>
#include <math.h>

void printHorizontalSeparator(int max)
{
	int i;
	printf("\t\t+");
	for(i=0;i<max;i++)
	{
		printf("----------");
	}
	printf("---+\n");	
}

double Tserial(int n, int problem_size_multiplier)
{
	return n;
}

double Tparallel(int n, int problem_size_multiplier, int p)
{
	return Tserial(n,problem_size_multiplier)/pow(2,p) + log(pow(2,p))/log(2);
}

int main()
{
	printf("Suppose Tserial = n and Tparallel = n/p + log2(p), where times are in microsec-\n"); 
	printf("onds. If we increase p by a factor of k, find a formula for how much we’ll\n");
	printf("need to increase n in order to maintain constant efficiency. How much should\n");
	printf("we increase n by if we double the number of processes from 8 to 16? Is the\n");
	printf("parallel program scalable?\n");

	printf("A) These qualitative questions follow intuitive thought. As the , the running\n");

	//n is the problem size
	int n;
	int max_n_power = 5;
	int problem_size_multiplier = 10;

	//p is the number of processes
	int p;

	//Information
	printf("Rows indicate the number of processes (p)\nColumns indicate the problem size(n)\n\n\n");
	
	//k is the factor 
	int k = 3;

	//Table Label
	printf("\t\tEfficiency of program when k is %d\n",k);
	
	//hat on the table
	printHorizontalSeparator(max_n_power);
	
	//column headings
	printf("\t\t");
	for(n=0;n<=max_n_power;n++)
	{
		printf("|%8.f",pow(2,n) * problem_size_multiplier);
	}
	printf("|\n");

	//separator between headings and data
	printHorizontalSeparator(max_n_power);

	//data
	for(p=1;(log(p)/log(k))<=6;p*=k)
	{
		printf("\t%8.d",p);
		for(n=0;n<=max_n_power;n++)
		{
			printf("|%8.1f",Tparallel(n,problem_size_multiplier,p));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(max_n_power);

	//spaces between tables
	printf("\n\n");

	return 0;
}
