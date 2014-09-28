#include <stdio.h>
#include <math.h>

double trap_area(double y1,double y2,double delta)
{
	return .5*(y1+y2)*delta;
}
double eval_function(double x)
{
	return sqrt(1-x*x);
}
int main()
{
	double i,delta=.000000001;
	double lower_bound=0;
	double upper_bound=1;
	double y1;
	double y2;
	double area = 0.0;
	y2 = eval_function(lower_bound);
	for(i=lower_bound+delta;i<=upper_bound;i+=delta)
	{
		y1=y2;
		y2=eval_function(i);
		area += trap_area(y1,y2,delta);
	}
	printf("Your trapezoidal estimation of PI: \t%.15f\n",area*4);
	printf("math.h's macro for PI: \t\t\t%.15f\n", M_PI);

	return 0;
}
