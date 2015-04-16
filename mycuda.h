// Time-stamp: </w/parallel/code/potential/mycuda.h, Tue, 31 Mar 2015, 19:37:32 EDT, http://wrfranklin.org/>
// Common stuff for all my C++ CUDA programs.
// Obsoletes misc.h

#define CUDA
#include "common2.h"

#include <thrust/adjacent_difference.h>
#include <thrust/device_vector.h>
#include <thrust/equal.h>
//#include <thrust/execution_policy.h>
#include <thrust/functional.h>
#include <thrust/generate.h>
#include <thrust/host_vector.h>
#include <thrust/iterator/constant_iterator.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/iterator/zip_iterator.h>
#include <thrust/logical.h>
#include <thrust/random.h> 
#include <thrust/scan.h>
#include <thrust/sort.h> 
//#include <thrust/system/omp/execution_policy.h>
//#include <thrust/tabulate.h> 
#include <thrust/tuple.h>
#include <thrust/version.h> 
//using namespace thrust::placeholders;


// Shorthand for defining functors.  E.g.
// Definition: LAMBDA(double, int, (int i) { return 2*i; } );
// Usage: j = double(4);
// Usage: thrust::transform(X.begin(), X.end(), Y.begin(), double);

#define LAMBDA(functor, return_type, body)			\
  struct T##functor  { __device__ __host__ return_type operator () body } functor;


//   Execute an expression and check for CUDA errors.
#define CE(exp) {						\
    cudaError_t e; e = (exp);						\
    if (e != cudaSuccess) { \
       cerr << #exp << " failed at line " << __LINE__  << " in function " << __FUNCTION__ << " of file " << __FILE__  << " with error " << cudaGetErrorString(e) << endl; \
	 throw(1);							\
    } \
}

//   Execute an expression, check for CUDA errors, and print time.
#define CET(exp) {						\
    cudaError_t e; \
    cudaEvent_t start, stop; \
    cudaEventCreate(&start); \
    cudaEventCreate(&stop); \
    cudaEventRecord(start, 0); \
    e = (exp);						\
    if (e != cudaSuccess) { \
       cerr << #exp << " failed at line " << __LINE__  << " in function " << __FUNCTION__ << " of file " << __FILE__  << " with error " << cudaGetErrorString(e) << endl; \
	 throw(1);							\
    } \
    cudaEventRecord(stop, 0); \
    cudaEventSynchronize(stop); \
    float(dt); \
    cudaEventElapsedTime(&dt, start, stop); \
    cout << "Time for " <<  #exp << " at line " << __LINE__  << " in function " << __FUNCTION__ << " of file " << __FILE__  << ": " << dt << " msec." << endl; \
}

//   Execute a CUDA expression and print time.
#define CT(exp) {						\
    cudaEvent_t start, stop; \
    cudaEventCreate(&start); \
    cudaEventCreate(&stop); \
    cudaEventRecord(start, 0); \
    (exp);								\
    cudaEventRecord(stop, 0); \
    cudaEventSynchronize(stop); \
    float(dt); \
    cudaEventElapsedTime(&dt, start, stop); \
    cout << "Time for " <<  #exp << " at line " << __LINE__  << " in function " << __FUNCTION__ << " of file " << __FILE__  << ": " << dt << " msec." << endl; \
}

// Check if a kernel (or any other earlier CUDA functions) failed.  Call this after
// the kernel.  Checks if the launch failed, then checks for error during execution.

#define CEK \
CE(cudaGetLastError()); \
CE(cudaDeviceSynchronize());


void PrintCudaMem(string s) {
  size_t cuda_mem_tot;
  size_t cuda_mem_free;
  try {CEK;} catch(...) { cout << redtty << "CEK failed in PrintCudaMem.  Trying to continue." << deftty << endl;}

  CE(cudaMemGetInfo(&cuda_mem_free, &cuda_mem_tot));
  cout << s << ": " << PRINTC(cuda_mem_tot) << PRINTN(cuda_mem_free);
}

void PrintCudaMem2(string s) {
  size_t cuda_mem_tot;
  size_t cuda_mem_free;
  CE(cudaMemGetInfo(&cuda_mem_free, &cuda_mem_tot));
  cout << s << ": " << PRINTC(cuda_mem_tot) << PRINTN(cuda_mem_free);
}

void InitCUDA(void) {  // Init CUDA stuff
  cout.imbue(locale("en_US.utf8"));  // Print big numbers with commas.
  cerr.imbue(locale("en_US.utf8"));  // Print big numbers with commas.
  cudaDeviceProp prop;
  cudaGetDeviceProperties(&prop, 0);
  int driverVersion = 0, runtimeVersion = 0;
  cudaDriverGetVersion(&driverVersion);
  cudaRuntimeGetVersion(&runtimeVersion);
  cout << "Device name: " << prop.name << endl;
  cout << "CUDA Capability version: " << prop.major << "." <<  prop.minor << endl;
  cout << "CUDA Driver / Runtime Version: " << driverVersion << "/" << runtimeVersion << ", CUDA_VERSION: " << CUDA_VERSION << endl;
  int major = THRUST_MAJOR_VERSION;
  int minor = THRUST_MINOR_VERSION;
  cout << "Thrust v" << major << "." << minor << endl;

  if (!prop.unifiedAddressing) {
    cerr << "Unified addressing not available." << endl;
    throw(1);
  }
  cudaGetDeviceProperties(&prop, 0);
  if (!prop.canMapHostMemory) {
    cerr << "Can't map host memory." << endl;
    throw(1);
  }
  cudaSetDeviceFlags(cudaDeviceMapHost);
}
