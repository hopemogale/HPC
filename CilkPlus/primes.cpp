#include <iostream>
#include <cilk/cilk.h>
#include <sys/time.h>
#include <math.h>

using namespace std;

bool isPrime(int n){
int limit = sqrt(n);
for(int i=2; i<=limit; i++)
if(n%i == 0)
return false;
return true;
}

int main(){
int n = 10000000;
int gs = 25000;
int numPrimes = 0;
struct timeval start,end;

gettimeofday(&start,NULL); //Start timing the computation

#pragma grainsize = gs

cilk_for(int i = 0; i < n/gs; i++){
for(int j = i*gs+1; j < (i+1)*gs; j += 2){
if(isPrime(j))
numPrimes++;
}
}

gettimeofday(&end,NULL); //Stop timing the computation

double myTime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);

cout << "Found " << numPrimes << " primes in " << myTime << " seconds.\n";

}
