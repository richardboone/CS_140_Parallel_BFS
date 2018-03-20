#include <cilk/cilk.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "bag.h"

using namespace std;

typedef struct graphstruct { // A graph in compressed-adjacency-list (CSR) form
  int nv;            // number of vertices
  int ne;            // number of edges
  int *nbr;          // array of neighbors of all vertices
  int *firstnbr;     // index in nbr[] of first neighbor of each vtx
} graph;

void print_adjacency_matrix(int* m, int* v);
void input_vector(vector< vector<int> > *matrix, char* filename, int& numedges);
graph * graph_from_edge_list(vector< vector<int> > *matrix, int numedges);
void print_CSR_graph(graph *G);
void bfs_bag(graph *G, int s, int *level);
