//Sub accurate pi approximation based on Taylor series
//The following program calculates pi in serial mode

#include <stdio.h>

	int main()

	{

    int poly;

    double pi(int n) {
    double sum = 0.0;
    int sign = 1;

    for (int i = 0; i < n; ++i) {
        sum += sign/(2.0*i+1.0);
        sign *= -1;
    }

    return 4.0*sum;
    }


    printf("Enter the number of polynomials to use for approximating pi : " );
    scanf("%d", &poly);

    printf("The value of pi is: %0.11f", pi(poly));

	}
