// Time-stamp: </w/parallel/code/potential/omp.cc, Sun,  5 Apr 2015, 17:10:59 EDT, http://wrfranklin.org/>

// Find the potential energy of N 2D points.

#include "common"

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
#pragma omp parallel
  {
#pragma omp master
    cout << PRINTN(omp_get_num_threads());
#pragma omp for reduction(+:tpot) // schedule(dynamic)
    for (int i=0; i<n-1; i++) 
      for (int j=i+1; j<n; j++)
	tpot += pot(i,j);
  }
  cout << PRINTC(n) << PRINTC(tpot);
  cout << PRINTN(Delta_Clock_Time());
}

