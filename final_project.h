#include <cilk/cilk.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "bag.h"
// #include <cilk/reducer_list.h>
#include "cilk/reducer_vector.h"
using namespace std;

typedef struct graphstruct { // A graph in compressed-adjacency-list (CSR) form
  long nv;            // number of vertices
  long ne;            // number of edges
  long *nbr;          // array of neighbors of all vertices
  long *firstnbr;     // index in nbr[] of first neighbor of each vtx
} graph;

void print_adjacency_matrix(int* m, int* v);
void input_vector(vector< vector<long> > *matrix, char* filename, long& numedges);
graph * graph_from_edge_list(vector< vector<long> > *matrix, long numedges);
void print_CSR_graph(graph *G);
void bfs_bag(graph *G, long s, int *level);
void vector_BFS(graph* G);
void add_values(long vertex, cilk::reducer<cilk::op_vector<long> >* r, int* vertlevels, graph* G, int currlevel);
void printvertices(long numvertices, int* vertlevels);