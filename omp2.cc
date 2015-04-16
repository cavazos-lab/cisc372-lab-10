// Time-stamp: </w/parallel/code/potential/omp2.cc, Sun,  5 Apr 2015, 17:11:53 EDT, http://wrfranklin.org/>

// Find the potential energy of N 2D points.
// Use 1 loop, not 2 nested ones.
// Note the necessary long long in places.

#include "common"
#include <utility>

typedef double Tcoord;

const int n = 60000;

// Make the coordinates in a square grid.
const int n1 = sqrt(Tcoord(n))+1;  // # points on a side

Tcoord px(const int i) { return Tcoord(i/n1); }
Tcoord py(const int i) { return Tcoord(i%n1); }
Tcoord sqr(const Tcoord x) { return x*x; }
Tcoord pot(const int i, const int j) {
  return 1.0/sqrt(sqr(px(i)-px(j))+sqr(py(i)-py(j)));}

int main() {
  cout.imbue(locale("en_US.utf8"));  // Print big numbers with commas.
  cerr.imbue(locale("en_US.utf8"));  // Print big numbers with commas.
  //  for (int i=0;i<n;i++)
  //    cout << i << ' ' << px(i) << ' ' << py(i) << endl;
  Delta_Clock_Time();
  Tcoord tpot=0;
  const long long int kt = n*(n-1LL)/2LL;

#pragma omp parallel
  {
#pragma omp master
    cout << PRINTN(omp_get_num_threads());
#pragma omp for reduction(+:tpot) // schedule(dynamic)
    for (long long int k=0; k<kt; k++) {
      const int j = sqrt(2.0*k+0.25)+0.5000000001;
      const int i = k - j*(j-1LL)/2;
      //      cout << PRINTC(k) << PRINTC(i) << PRINTN(j);
      tpot += pot(i,j);
    }
  }
  cout << PRINTC(n) << PRINTC(tpot);
  cout << PRINTN(Delta_Clock_Time());
}

