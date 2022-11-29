// C++ program for implementation of Newton Raphson Method for
// solving equation
#define EPSILON 0.001
using namespace std;
#include <cstring>
#include <cmath>
// An example function whose solution is determined using
// Bisection Method. The function is x^3 - x^2 + 2
double func(double x)
{
	return x*x*x - x*x + 2;
}

// Derivative of the above function which is 3*x^x - 2*x
double derivFunc(double x)
{
	return 3*x*x - 2*x;
}

// Function to find the root
void newtonRaphson(double x)
{
	double h = func(x) / derivFunc(x);
	while (abs(h) >= EPSILON)
	{
		h = func(x)/derivFunc(x);

		// x(i+1) = x(i) - f(x) / f'(x)
		x = x - h;
	}

//printf( "The value of the root is :") ;
}
 double Uc_n1(double hn, double E, double R, double C, double Uc_n)
 {
     return((hn*E/R*C )+(1-hn/R*C)*Uc_n);
 }
double dUc_dt(double Uc_n, double hn,double E, double R, double C,)
{
   return ((Uc_n1(hn,E,R,C,Uc_n)-Uc_n)/hn);

}
double C_source_val(double C,double h,double t, double v)
{
    return (C*v*(t-h)/h);
}
// Driver program to test above
int main()
{
	double x0 = -20; // Initial values assumed
	newtonRaphson(x0);
	return 0;

}
