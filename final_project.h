#include <cilk/cilk.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cilk/reducer_vector.h>
#include <cilk/reducer_opadd.h>
// #include <mutex>
using namespace std;

// #include <tbb/mutex.h>


typedef struct graphstruct { // A graph in compressed-adjacency-list (CSR) form
  int nv;            // number of vertices
  int ne;            // number of edges
  int *nbr;          // array of neighbors of all vertices
  int *firstnbr;     // index in nbr[] of first neighbor of each vtx
} graph;

void print_adjacency_matrix(int* m, int* v);
void input_vector(vector< vector<int> > *matrix, char* filename, int& numedges, char direction);
graph * graph_from_edge_list(vector< vector<int> > *matrix, int numedges);
void print_CSR_graph(graph *G);
void vector_BFS(graph* G);
int add_values(int vertex, vector<int>* nextlevel, int* vertlevels, graph* G, pthread_mutex_t m, int currlevel);
void printvertices(int numvertices, int* vertlevels);