#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bag.h"
#include "final_project.h"
#include "cilk/cilk.h"
#include <pthread.h>

using namespace std;

void input_vector(vector< vector<int> > *matrix, char* filename, int &numedges){
  ifstream inFile;
  inFile.open(filename);
  int start, stop;

  while(inFile >> start >> stop) {
    matrix->at(start).push_back(stop);
    numedges++;
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

graph* graph_from_edge_list(vector< vector<int> > *matrix, int numedges) {
  graph *G = new graph;
  G->nv = matrix->size();
  G->ne = numedges;
  cout << numedges << endl;
  G->nbr = new int[G->ne];
  G->firstnbr = new int[G->nv+1];

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
  G->firstnbr[matrix->size()] = numedges;
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

int getNumAdjacencies(graph *G, int v, int indexOfFirstnbr) {
  int numAdjacencies;
  //if (numAdjacencies == G->ne-1) {
  if (v == G->nv-1) {
    numAdjacencies = G->ne - indexOfFirstnbr;
  }
  else {
    numAdjacencies = G->firstnbr[v+1] - G->firstnbr[v];
  }
  return numAdjacencies;
}

void bfs_bag(graph *G, int s, int level[]) {
  //level = new int[G->nv];
  /*
  for (int n = 0; n < G->nv; n++) {
    level[n] = -1;
  }
  */
  int levelnum = 0;
  
  Bag frontier(1);
  Bag visited(1);
  frontier.insert(s);
  visited.insert(s);
  
  int numAdjacencies;
  //pthread_mutex_t m;
  while (!frontier.isEmpty()) {
  //while (visited.size() != G->nv) {
    Bag succbag(1);
    for (int i = 0; i < frontier.size(); i++) {
      int indexOfFirstnbr = G->firstnbr[frontier.getElement(i)];

      cout << "vertex: " << frontier.getElement(i) << endl;
      cout << "indexOfFirstnbr: " << indexOfFirstnbr << endl;

      level[frontier.getElement(i)] = levelnum;
      cout << "level: " << level[frontier.getElement(i)] << endl;
      numAdjacencies = getNumAdjacencies(G, frontier.getElement(i), indexOfFirstnbr);

      cout << "numAdjacencies: " << numAdjacencies << endl;

      for (int j = 0; j < numAdjacencies; j++) {
	int v = G->nbr[indexOfFirstnbr + j];

	//cout << v << " ";
	
	if (!visited.inbag(v)) {
	//if (level[v] != -1) {  
	  //cout << v << " ";
	  //pthread_mutex_lock(&m);
	  succbag.insert(v);
	  visited.insert(v);
	  //pthread_mutex_unlock(&m);
	  cout << v << " ";
	}
      }
      cout << endl;
    }
    frontier = succbag;
    levelnum++;
  }
  cout << "EXITED" << endl;
  /*
  for (int i = 0; i < G->nv; i++) {
    cout << level[i] << endl;
  }
  */
}

