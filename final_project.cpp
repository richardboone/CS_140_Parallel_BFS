#include <iostream>
#include "final_project.h"
#include <vector>
#include <string.h>

using namespace std;

int main(int argc, char** argv){
  unsigned int numvertices;
  int numedges = 0;
  
  if (argc < 3){
    cout << "Usage: ./final_project [a,b]  [d, u] <numvertices> <file_name>" << endl;
    exit(-1);
  }

  numvertices = (unsigned int)atoi(argv[3]);
  vector< vector<int> > *matrix = new vector< vector<int> >(numvertices+1);  
  input_vector(matrix, argv[4], numedges, argv[2][0]);
	// cout << "got input" << endl;
  graph *G = graph_from_edge_list(matrix, numedges);
  // print_CSR_graph(G);
  delete  matrix;

  if (argv[1][0] == 'a') {
    
  }
  else if (argv[1][0] == 'b') {
    vector_BFS(G);
  }
	
	
	delete G;
  return 0;
}


void print_adjacency_matrix(int* m, int* v){
  /*
  int numvertices = m.size();
  int nextmax = 0;
  int prevmax = 0;
  for ( int i = 0; i < numvertices; i++) {
    prevmax = nextmax;
    nextmax = v[i+1];
    for (int j = prevmax; j < nextmax; j++){
      cout << m[j];
    }
    cout << endl;
  }
  */
}
