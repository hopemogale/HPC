#include<iostream>
#include<chrono>
#include<omp.h>
using namespace std;

void hello_openmp()
{
  cout << "Hello, OpenMP"
}

int int main(int argc, char const *argv[]) {
  /* code */
  hello_openmp();

  getchar();
  return 0;
}
