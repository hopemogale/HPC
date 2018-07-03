#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_poly.h>
#include <cmath>

#define maxd(a,b) (((a) > (b)) ? (a) : (b))
#define mind(a,b) (((a) < (b)) ? (a) : (b))

unsigned long long cur_root;
unsigned long long capacity;

void
polynomial_recursive(double **roots, double min_x, double max_x, int min_degree, int max_degree, int cur_degree, double *coeff)
{
    if (cur_degree <= max_degree)
      {
        for (double c = -4; c <= 4; c++)
          {
            double *new_coeff;

            new_coeff = (double *) malloc (sizeof (double) * (cur_degree + 1));
            if (new_coeff == NULL)
              {
                perror ("malloc failed");
              }
            memcpy (new_coeff, coeff, cur_degree * sizeof (double));
            new_coeff[cur_degree] = c;
            if (cur_degree >= min_degree && new_coeff[cur_degree] != 0)
              {
                double z[cur_degree*2];

                gsl_poly_complex_workspace *w
                    = gsl_poly_complex_workspace_alloc (cur_degree + 1);

                gsl_poly_complex_solve (new_coeff, cur_degree + 1, w, z);

                gsl_poly_complex_workspace_free (w);

                for (int i = 0; i < cur_degree; i++)
                  {
                    if (cur_root + 2 >= capacity)
                      {
                        capacity *= 2;
                        double *tmp = (double *) realloc (*roots, capacity * sizeof (double));
                        if (tmp == NULL)
                          {
                            perror ("realloc failed");
                          }
                        else
                          {
                            *roots = tmp;
                          }
                      }
                    (*roots) [cur_root++] = z[2*i];
                    (*roots) [cur_root++] = z[2*i+1];
                  }
              }
            #pragma omp task shared(new_coeff)
            polynomial_recursive (roots, min_x, max_x, min_degree, max_degree, cur_degree + 1, new_coeff);
            #pragma omp taskwait
            free (new_coeff);
          }
      }
}

unsigned long long getDensityForPoint(double x, double y, double *points, int size, double radius)
{
    unsigned long long count = 0;
    for (int i = 0; i < size; i += 2)
      {
        double x2 = points[i];
        double y2 = points[i+1];
        if (x != x2 || y != y2)
          {
            double dist = fabs(x2 - x) + fabs(y2 - y);;
            if (dist <= radius)
              {
                count++;
              }
          }
      }
    return count;
}

double
calculateDensityScalarFromDensity(unsigned long min, unsigned long max, unsigned long density)
{
    return 0.0 + ((1.5 - 0.0) / (double)(max - min)) * (density - min);
}

int
main(void)
{
    double *roots;
    unsigned long long *densities, min, max;

    capacity = 100;
    cur_root = 0;
    roots = (double *) malloc (capacity * sizeof (double));
    if (roots == NULL)
      {
        perror ("malloc failed");
      }

    #pragma omp parallel
    {
        #pragma omp single
        {
            polynomial_recursive (&roots, -4, 4, 2, 5, 0, NULL);
        }
    }

    densities = (unsigned long long *) malloc (cur_root / 2 * sizeof (unsigned long long));
    max = 0;
    min = ~0;

    #pragma omp parallel for
    for (int i = 0; i < cur_root; i += 2)
      {
        densities[i / 2] = getDensityForPoint(roots[i], roots[i+1], roots, cur_root, 0.1);
        max = maxd(densities[i / 2], max);
        min = mind(densities[i / 2], min);
      }

    printf("max %llu, min %llu\n", max, min);
    FILE *gnuplotPipe = popen ("gnuplot -persistent", "w");
    FILE *temp = fopen("data.temp", "w");

    #pragma omp parallel for
    for (int i = 0; i < cur_root; i += 2)
      {
        fprintf (temp, "%lf %lf %lf\n", roots[i], roots[i+1], calculateDensityScalarFromDensity(min, max, densities[i / 2]));
      }

    free(densities);
    free(roots);

    fprintf (gnuplotPipe, "set zrange [0.0:1.5]\nset cbrange [0.0:1.5]\nset term pngcairo\nset terminal png size 1920,1080\nset output \"plot.png\"\nset palette defined (0 \"black\", 0.5 \"red\", 1 \"yellow\", 1.5 \"white\")\nplot 'data.temp' with points pointtype 5 pointsize 0.001 palette\n");
}
