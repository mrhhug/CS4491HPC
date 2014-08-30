/* cs4491-HPC Fall 2014 
 * Michael Hug
 * Assignment 2
 * Problem 2.16a 
 */


/* Suppose the run-time of a serial program is given by T serial = n 2 , where
 * the units of the run-time are in microseconds. Suppose that a paral-
 * lelization of this program has run-time T parallel = n 2 /p + log 2 (p). Write a
 * program that finds the speedups and efficiencies of this program for vari-
 * ous values of n and p. Run your program with n = 10, 20, 40, . . . , 320, and
 * p = 1, 2, 4, . . . , 128. What happens to the speedups and efficiencies as p
 * is increased and n is held fixed? What happens when p is fixed and n is
 * increased?
 */

#include <stdio.h>
#include <math.h>

void printHorizontalSeparator(int i,int max)
{
	printf("\t+");
	for(i=0;i<max;i++)
	{
		printf("--------");
	}
	printf("-+\n");	
}

int main()
{
	//n is the problem size
	int n;
	int max_n_power = 5;
	int problem_size_multiplier = 10;

	//p is the number of parallel cores
	int p;
	int max_p_power = 7;
	//int core_count_multiplier = 1;

	//Information
	printf("Rows indicate the number of parallel cores\nColumns indicate the problem size\n\n\n");

	//Table Label
	printf("\tTime of serial programmed application execution\n");
	
	//hat on the table
	printHorizontalSeparator(n,max_n_power);
	
	//column headings
	printf("\t");
	for(n=0;n<=max_n_power;n++)
	{
		printf("|%6.f",pow(2,n) * problem_size_multiplier);
	}
	printf("|\n");

	//separator between headings and data
	printHorizontalSeparator(n,max_n_power);

	//data
	for(p=0;p<=max_p_power;p++)
	{
		printf("%6.f\t",pow(2,p));
		for(n=0;n<=max_n_power;n++)
		{
			printf("|%6.f",pow(pow(2,n)*problem_size_multiplier,2));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(n,max_n_power);

	//spaces between tables
	printf("\n\n");

	//Table Label
	printf("\tTime of parallel programmed application execution\n");
	
	//hat on the table
	printHorizontalSeparator(n,max_n_power);
	
	//column headings
	printf("\t");
	for(n=0;n<=max_n_power;n++)
	{
		printf("|%6.f",pow(2,n) * problem_size_multiplier);
	}
	printf("|\n");

	//separator between headings and data
	printHorizontalSeparator(n,max_n_power);

	//data
	for(p=0;p<=max_p_power;p++)
	{
		printf("%6.f\t",pow(2,p));
		for(n=0;n<=max_n_power;n++)
		{
			printf("|%f",log(2.7));//(pow(pow(2,n)*problem_size_multiplier,2))/(pow(2,p)));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(n,max_n_power);
	return 0;
}
