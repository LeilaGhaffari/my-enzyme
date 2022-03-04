// The split version of test008

#include <stdio.h>
#include <stdlib.h>

void foo(double *u, double *x, double mu) {
    for (int i = 0; i<3; i++) u[i] = mu * (x[i] * x[i]);
}

int  __enzyme_augmentsize(void *, ...);
void __enzyme_augmentfwd(void *, ...);
void __enzyme_reverse(void *, ...);

int enzyme_dup;
int enzyme_tape;
int enzyme_const;
int enzyme_allocated;

void grad_foo(double *u, double *du, double *x, double *dx, double mu) {
    int size = __enzyme_augmentsize((void *)foo, enzyme_dup, enzyme_dup, enzyme_const); // We need to add Const as well
    void *data = malloc(size);
    __enzyme_augmentfwd((void *)foo, enzyme_allocated, size, enzyme_tape, data, u, du, x, dx, mu);
    __enzyme_reverse((void *)foo,    enzyme_allocated, size, enzyme_tape, data, u, du, x, dx, mu);
    free(data);
}

int main() {

    double x[3] = { 0.653484990079922,
                    0.744572635699587,
                    0.23483535966444968
                  };
    double u[3]; // No need for initialization
    double dx[3][3] = { {0., 0., 0.},
                        {0., 0., 0.},
                        {0., 0., 0.}
                      };
    double mu = 2;                  
    for (int i=0; i<3; i++) {
        double du[3]  = {0., 0., 0.}; du[i] = 1;
        grad_foo(u, du, x, dx[i], mu);
    }

    printf("\n\ndx =\n");
    for (int i=0; i<3; i++) printf("\t%.6lf ", dx[0][i]);
    printf("\n");
    for (int i=0; i<3; i++) printf("\t%.6lf ", dx[1][i]);
    printf("\n"); 
    for (int i=0; i<3; i++) printf("\t%.6lf ", dx[2][i]);
    printf("\n\n");
    printf("\nu(x) =\n");
    for (int i=0; i<3; i++) printf("\tu(%f) = %f\n", x[i], u[i]);
    printf("\n\n");

    return 0;
}

/*

// Output:

dx =
	2.613940 	0.000000 	0.000000 
	0.000000 	2.978291 	0.000000 
	0.000000 	0.000000 	0.939341 


u(x) =
	u(0.653485) = 0.854085
	u(0.744573) = 1.108777
	u(0.234835) = 0.110295

*/
