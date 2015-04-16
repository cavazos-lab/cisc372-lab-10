// Time-stamp: </w/parallel/code/potential/thr.cu, Sun,  5 Apr 2015, 17:25:16 EDT, http://wrfranklin.org/>

// Find the potential energy of N 2D points arranged in a square
// grid.  There are K=N(N-1)/2 pairs of points.  Index the pairs
// from 0 to K-1.  Function k2pot finds the potential energy of
// pair #K.

// Thrust apparently cannot use long long int.

#define PRINTTIMESHORT
#define COLORTTY
#define REALLYPRINTTIME

#include "mycuda.h"

using namespace std;
using namespace thrust;

typedef double Tcoord;

//const int n = 60000;
const int n = 10;

int n1 = sqrt(Tcoord(n))+1;  // # points on a side of the grid

// Make the points in a square grid. 
// Given the index of a point in the grid, find its X and Y coords.
#define coord(n1,k) (thrust::pair<Tcoord,Tcoord> ((k)/(n1), (k)%(n1)))

#define sqr(x) ((x)*(x))

// Given the index of a pair of points, find their potential energy.
struct k2pot : public thrust::unary_function<int,Tcoord>
{
  const int m;
  k2pot(int _m) : m(_m) {}
  __host__ __device__
  Tcoord operator()(const int k) const { 
    int j = sqrt(2.0*k+0.25)+0.5000000001;
    int i = k - j*(j-1LL)/2;
    return (1.0/sqrt(sqr(coord(m,i).first-coord(m,j).first)+sqr(coord(m,i).second-coord(m,j).second)));
  }
};

int main() {
  Delta_Clock_Time();
  cout.imbue(locale("en_US.utf8"));  // Print big numbers with commas.
  cerr.imbue(locale("en_US.utf8"));  // Print big numbers with commas.

  long long int npair = n*(n-1LL)/2;  

#if 0
  cout << "dx dy:" << endl;
  for (int i=0;i<n;i++)
    cout << i << ' ' << coord(n1,i).first << ' ' << coord(n1,i).second << endl;

  cout << "k->i j pot:" << endl;
  for(int k=0; k<npair; k++) {
    int j = sqrt(2.0*k+0.25)+0.5000000001;
    int i = k - j*(j-1LL)/2;
    cout << k << ' ' << i << ' ' << j << ' ' << k2pot(n1)(k) << endl;
  }

#endif

  Tcoord tpot=
    thrust::transform_reduce(counting_iterator<int>(0LL),
			   counting_iterator<int>(npair), k2pot(n1), 0.0,
			   thrust::plus<Tcoord>());

  cout << PRINTC(n) << PRINTC(tpot);
  cout << PRINTN(Delta_Clock_Time());
}

