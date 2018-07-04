#include<stdio.h>
//#include<chrono>
#include<omp.h>

void hello_openmp()
{
  #pragma omp parallel
  printf("Testing OpenMP Threads \n");
}

int main() {
  /* code */
  hello_openmp();
  return 0;
}
