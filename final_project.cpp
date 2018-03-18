#include <iostream>
#include "final_project.h"
#include <vector>
#include <string.h>

using namespace std;

int main(int argc, char** argv){
  unsigned int numvertices;
  int numedges = 0;
  
  if (argc < 3){
    cout << "Usage: ./final_project [a,b]  <numvertices> <file_name>" << endl;
    exit(-1);
  }

  numvertices = (unsigned int)atoi(argv[2]);
  vector< vector<int> > *matrix = new vector< vector<int> >(numvertices);  
  input_vector(matrix, argv[3], numedges);

  graph *G = graph_from_edge_list(matrix, numedges);
  print_CSR_graph(G);

  if (argv[1][0] == 'a') {
    
  }
  else if (argv[1][0] == 'b') {
    
  }
	
	
	
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
