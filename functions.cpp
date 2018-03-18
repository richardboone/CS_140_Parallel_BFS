#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "final_project.h"

using namespace std;

graph* graph_from_edge_list(vector< vector<int> > *matrix, int numedges) {
  graph *G = new graph;
  G->nv = matrix->size();
  G->ne = numedges;
  cout << numedges << endl;
  G->nbr = new int[G->ne];
  G->firstnbr = new int[G->nv];

  int nbr_index = 0;
  int firstnbr_index = 0;
  for (int i = 0; i < matrix->size(); i++) {
    G->firstnbr[firstnbr_index] = nbr_index;
    for (int j = 0; j < matrix->at(i).size(); j++) {
      G->nbr[nbr_index] = matrix->at(i).at(j);
      nbr_index++;
    }
    firstnbr_index++;
  }
  return G;
}

void print_CSR_graph(graph *G) {
  cout << "nbr = ";
  for (int i = 0; i < G->ne; i++) {
    cout << G->nbr[i] << ", ";
  }  
  cout << endl;
  cout << "first_nbr = ";
  for (int j = 0; j < G->nv; j++) {
    cout << G->firstnbr[j] << ", ";
  }
  cout << endl;
}

void input_vector(vector< vector<int> > *matrix, char* filename, int &numedges){
  ifstream inFile;
  inFile.open(filename);
  int start, stop;
 
  while(inFile >> start >> stop) {
    matrix->at(start).push_back(stop);
    (numedges)++;
  }
  /*
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix.at(i).size(); j++) {
      cout << matrix.at(i).at(j) << " ";
    }
    cout << endl;
  }
  */
}
