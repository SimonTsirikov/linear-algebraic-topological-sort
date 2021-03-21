# Linear Algebraic Topological Sort
Implementation of the algorithm from section 5 of [Linear Algebraic Depth-First Search](https://dl.acm.org/doi/10.1145/3315454.3329962) (ARRAY workshop @ PLDI 2019) by Daniele G. Spampinato et al.

## Build
Before build installation of [GraphBlas](https://graphblas.github.io/) and CMake is required.
```
cd build
cmake ..
make
```

## Run
You can ran either tests data or your example with next commands appropriately:
```
./graphBlas --test
./graphBlas --compare
./graphBlas <input file>
```

(To use `--compare`, generate test data with `build/data/generator.py`.)
Consider input file in format of `.txt` with data like this:
```
<number of vertexes in graph> <number of start vertexes> <number of edges in graph>
<list of <vertex> (start vertexes)>
<list of <vertex, vertex> (directed edges from-to)>
```
There should be  0 <= vertex index < number of vertexes.  
e.g.:
```
3 2 2

0 2

0 1
2 1
```
