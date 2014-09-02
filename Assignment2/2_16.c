/* cs4491-HPC Fall 2014 
 * Michael Hug
 * Assignment 2
 * Problem 2.16 
 */

#include <stdio.h>
#include <math.h>

#define nMin 10
#define nMax 320
#define pMin 1
#define pMax 128

void printHorizontalSeparator(int min, int max)
{
	int i;
	printf("\t\t+");
	for(i=min;i<max;i*=2)
	{
		printf("----------");
	}
	printf("---+\n");	
}

double Tserial(int n)
{
	return pow(n,2);
}

double Tparallela(int n, int p)
{
	return Tserial(n)/p + log(p)/log(2);
}

double Tparallelb(int n, int p, int Toverhead)
{
	return Tserial(n)/p + Toverhead;
}

double speedupA(int n, int p)
{
	return Tserial(n)/Tparallela(n,p);
}

double efficiencyA(int n, int p)
{
	return Tserial(n)/(p*Tparallela(n,p));
}

double efficiencyB(int n, int p, int Toverhead)
{
	return Tserial(n)/(p*Tparallelb(n,p,Toverhead));
}

int main()
{

	//n is the problem size
	int n;

	//p is the number of processes
	int p;
	
	printf("Suppose the run-time of a serial program is given by Tserial = n^2 , where\n"); 
	printf("the units of the run-time are in microseconds. Suppose that a paral-\n");
	printf("lelization of this program has run-time Tparallel = n^2 /p + log 2 (p). Write a\n");
	printf("program that finds the speedups and efficiencies of this program for vari-\n");
	printf("ous values of n and p. Run your program with n = 10, 20, 40, . . . , 320, and\n");
	printf("p = 1, 2, 4, . . . , 128. What happens to the speedups and efficiencies as p\n");
	printf("is increased and n is held fixed? What happens when p is fixed and n is\nincreased?\n\n");

	printf("A) Considering the given range for n and p:\n");
	printf("As p grows and n is fixed, we see a better efficiency and speedup in all cases except n=10.\n");
	printf("As n grows and p is fixed, we always see a better efficiency and speedup.\n");
	printf("A developer could reach diminishing returns is he trys to use many more processes than the problem size.\n");
	printf("the speedup actually started to decrease at n=10,p=128. The efficiency nose dived with the small problem size/large process size.\n");
	printf("The efficiency and speedup stayed quite nice for large problem sizes/large process sizes.\n");
	printf("This test showed that that it is not possible to just keep throwing processes at bad code, elegance is a requirement for efficiency.\n\n");

	//Information
	printf("Rows indicate the number of processes (p)\nColumns indicate the problem size(n)\n\n\n");

	//Table Label
	printf("\t\tSpeedup\n");
	
	//hat on the table
	printHorizontalSeparator(nMin,nMax);
	
	//column headings
	printf("\t\t");
	for(n=nMin;n<=nMax;n*=2)
	{
		printf("|%8.d",n);
	}
	printf("|\n");

	//separator between headings and data
	printHorizontalSeparator(nMin,nMax);

	//data
	for(p=pMin;p<=pMax;p*=2)
	{
		printf("\t%8.d",p);
		for(n=nMin;n<=nMax;n*=2)
		{
			printf("|%8.4f",speedupA(n,p));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(nMin,nMax);

	//spaces between tables
	printf("\n\n");
	
	//Table Label
	printf("\t\tEfficiency\n");
	
	//hat on the table
	printHorizontalSeparator(nMin,nMax);
	
	//column headings
	printf("\t\t");
	for(n=nMin;n<=nMax;n*=2)
	{
		printf("|%8.d",n);
	}
	printf("|\n");

	//separator between headings and data
	printHorizontalSeparator(nMin,nMax);

	//data
	for(p=pMin;p<=pMax;p*=2)
	{
		printf("\t%8.d",p);
		for(n=nMin;n<=nMax;n*=2)
		{
			printf("|%8.4f", efficiencyA(n,p));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(nMin,nMax);

	//spaces between tables
	printf("\n\n");
	
	printf("Suppose that Tparallel = Tserial /p + Toverhead . Also suppose that we fix p\nand increase the problem size.\n");
	printf("\t- Show that if T overhead grows more slowly than Tserial , the parallel\n\t\tefficiency will increase as we increase the problem size.\n");
	printf("\t- Show that if, on the other hand, Toverhead grows faster than Tserial , the\n\t\tparallel efficiency will decrease as we increase the problem size.\n\n");

	//Table Label
	printf("\t\tEfficiency when Toverhead=n\n");
	
	//hat on the table
	printHorizontalSeparator(nMin,nMax);
	
	//column headings
	printf("\t\t");
	for(n=nMin;n<=nMax;n*=2)
	{
		printf("|%8.d",n);
	}
	printf("|\n");

	//separator between headings and data
	printHorizontalSeparator(nMin,nMax);

	//data
	for(p=pMin;p<=pMax;p*=2)
	{
		printf("\t%8.d",p);
		for(n=nMin;n<=nMax;n*=2)
		{
			printf("|%8.6f",efficiencyB(n,p,n));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(nMin,nMax);

	//spaces between tables
	printf("\n\n");

	//Table Label
	printf("\t\tEfficiency when Toverhead=n^3\n");
	
	//hat on the table
	printHorizontalSeparator(nMin,nMax);
	
	//column headings
	printf("\t\t");
	for(n=nMin;n<=nMax;n*=2)
	{
		printf("|%8.d",n);
	}
	printf("|\n");

	//separator between headings and data
	printHorizontalSeparator(nMin,nMax);

	//data
	for(p=pMin;p<=pMax;p*=2)
	{
		printf("\t%8.d",p);
		for(n=nMin;n<=nMax;n*=2)
		{
			printf("|%8.6f",efficiencyB(n,p,n*n*n));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(nMin,nMax);

	//spaces between tables
	printf("\n\n");

	return 0;
}
