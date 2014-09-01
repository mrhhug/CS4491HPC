/* cs4491-HPC Fall 2014 
 * Michael Hug
 * Assignment 2
 * Problem 2.16 
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
	return pow(pow(2,n)*problem_size_multiplier,2);
}

double Tparallela(int n, int problem_size_multiplier, int p)
{
	return Tserial(n,problem_size_multiplier)/pow(2,p) + log(pow(2,p))/log(2);
}

double Tparallelb(int n, int problem_size_multiplier, int p, int Toverhead)
{
	return Tserial(n,problem_size_multiplier)/pow(2,p) + Toverhead;
}

int main()
{
	printf("Suppose the run-time of a serial program is given by Tserial = n^2 , where\n"); 
	printf("the units of the run-time are in microseconds. Suppose that a paral-\n");
	printf("lelization of this program has run-time Tparallel = n^2 /p + log 2 (p). Write a\n");
	printf("program that finds the speedups and efficiencies of this program for vari-\n");
	printf("ous values of n and p. Run your program with n = 10, 20, 40, . . . , 320, and\n");
	printf("p = 1, 2, 4, . . . , 128. What happens to the speedups and efficiencies as p\n");
	printf("is increased and n is held fixed? What happens when p is fixed and n is\nincreased?\n\n");

	printf("A) Considering the given range for n and p:\n");
	printf("A person could lose efficiency by using many more processes than the attempted problem size. We called this the law of diminishing returns and it kicked in\n");
	printf("very clearly at the bottom left hand corner of the efficiency and speedup charts.\n\n");

	printf("If one would fix the process count and increase the problem size, the graphs always show an increase in speedup and efficiency.\n\n");

	//n is the problem size
	int n;
	int max_n_power = 5;
	int problem_size_multiplier = 10;

	//p is the number of processes
	int p;
	int max_p_power = 7;

	//used in part b
	double Toverhead;

	//Information
	printf("Rows indicate the number of processes (p)\nColumns indicate the problem size(n)\n\n\n");

	/*
	//Table Label
	printf("\t\tTime of serial programmed application execution\n");
	
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
	for(p=0;p<=max_p_power;p++)
	{
		printf("\t%8.f",pow(2,p));
		for(n=0;n<=max_n_power;n++)
		{
			printf("|%8.1f",Tserial(n,problem_size_multiplier));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(max_n_power);

	//spaces between tables
	printf("\n\n");

	//Table Label
	printf("\t\tTime of parallel programmed application execution\n");
	
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
	for(p=0;p<=max_p_power;p++)
	{
		printf("\t%8.f",pow(2,p));
		for(n=0;n<=max_n_power;n++)
		{
			printf("|%8.1f",Tparallela(n,problem_size_multiplier,p));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(max_n_power);

	//spaces between tables
	printf("\n\n");
	*/

	//Table Label
	printf("\t\tSpeedup\n");
	
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
	for(p=0;p<=max_p_power;p++)
	{
		printf("\t%8.f",pow(2,p));
		for(n=0;n<=max_n_power;n++)
		{
			printf("|%8.4f",Tserial(n,problem_size_multiplier)/Tparallela(n,problem_size_multiplier,p));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(max_n_power);

	//spaces between tables
	printf("\n\n");
	
	//Table Label
	printf("\t\tEfficiency\n");
	
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
	for(p=0;p<=max_p_power;p++)
	{
		printf("\t%8.f",pow(2,p));
		for(n=0;n<=max_n_power;n++)
		{
			printf("|%8.4f",Tserial(n,problem_size_multiplier)/(pow(2,p)+Tparallela(n,problem_size_multiplier,p)));
		}
		printf("|\n");
	}

	//base on the table
	printHorizontalSeparator(max_n_power);

	//spaces between tables
	printf("\n\n");
	
	printf("Suppose that Tparallel = Tserial /p + Toverhead . Also suppose that we fix p\nand increase the problem size.\n");
	printf("\t- Show that if T overhead grows more slowly than Tserial , the parallel\n\t\tefficiency will increase as we increase the problem size.\n");
	printf("\t- Show that if, on the other hand, Toverhead grows faster than Tserial , the\n\t\tparallel efficiency will decrease as we increase the problem size.\n\n");

	//Information
	printf("Number of processes (p) will be fixed at 128\nColumns indicate the problem size(n)\nRows indicate the time to swich processes (Toverhead)\n\n");
	p = 128;

	//Table Label
	printf("\t\tEfficiency as Toverhead increases linearly\n");
	
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
	Toverhead=1.0;
	int i;
	for(i=1;i<=8;i++)
	{
		printf("\t%8.f",Toverhead);
		for(n=0;n<=max_n_power;n++)
		{
			printf("|%8.4f",Tserial(n,problem_size_multiplier)/(p*Tparallelb(n,problem_size_multiplier,p,Toverhead)));
		}
		printf("|\n");
		Toverhead++;
	}

	//base on the table
	printHorizontalSeparator(max_n_power);

	//spaces between tables
	printf("\n\n");

	//Table Label
	printf("\t\tEfficiency as Toverhead increases cubically\n");
	
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
	Toverhead=2.0;
	for(i=1;i<=3;i++)
	{
		printf("\t%8.f",Toverhead);
		for(n=0;n<=max_n_power;n++)
		{
			printf("|%8.4f",Tserial(n,problem_size_multiplier)/(p*Tparallelb(n,problem_size_multiplier,p,Toverhead)));
		}
		printf("|\n");
		Toverhead*=Toverhead*Toverhead;
	}

	//base on the table
	printHorizontalSeparator(max_n_power);

	return 0;
}
