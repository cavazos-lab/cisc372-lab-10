# Time-stamp: </w/parallel/code/potential/Makefile, Sun,  5 Apr 2015, 17:04:56 EDT, http://wrfranklin.org/>

V=35
V=30

CXXFLAGS=-fopenmp -std=c++14 -fdiagnostics-color
CXXFLAGS=-fopenmp -std=c++14 -g
CXXFLAGS=-std=c++11 -g
CXXFLAGS=-std=c++11 -O3
CXXFLAGS=-fopenmp -std=c++14 -O3

LDLIBS=-lm -lgomp

CUFLAGS=--ptxas-options=-v  -arch=compute_$V $C
CUFLAGS=-arch=compute_$V -std=c++11

.SUFFIXES: .cu

.cu: 
	nvcc $(CUFLAGS) $< $(LDLIBS) -o $@ 


