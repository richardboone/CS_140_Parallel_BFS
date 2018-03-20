#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bag.h"
#include "final_project.h"
#include "cilk/cilk.h"
#include <pthread.h>

using namespace std;
pthread_mutex_t mutex_global;

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
  // cout << numedges << endl;
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
  for (int j = 0; j < G->nv+1; j++) {
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


void vector_BFS(graph* G){
	vector< int> nextlevel1;
	vector< int> nextlevel2;
	int* vertlevels;
	// pthread_mutex_t m;
	int numedges = G->ne;
	int numvertices = G->nv;
	vertlevels = (int*)malloc((numvertices) * sizeof(int));
	for (int i = 0; i < numvertices; i++){
		vertlevels[i] = -1;
	}
	nextlevel1.push_back(1);
	vertlevels[1] = 0;
	// cilk::reducer< cilk::op_list_append<int> > int_reducer1;
	// cilk::reducer< cilk::op_list_append<int> > int_reducer2;
	int currlevel = 0;
	while(nextlevel1.size() > 0){
		currlevel++;
		// cout << endl;
		cilk_for(int i = 0; i < nextlevel1.size(); i++){
			add_values(nextlevel1[i], &nextlevel2,/* int_reducer1,*/ vertlevels, G, currlevel);
		}
		if (nextlevel2.size()==0){
			break;
		}	
		currlevel++;
		cilk_for(int i = 0; i < nextlevel2.size(); i++){
			add_values(nextlevel2[i], &nextlevel1, /*int_reducer1,*/ vertlevels, G, currlevel);
		}
		nextlevel2.clear();
	}
	printvertices(numvertices, vertlevels);
	free(vertlevels);
}

void printvertices(int numvertices, int* vertlevels){
	cout << "vertex:depth\n";
	for (int i = 1; i < numvertices; i++){
		if (vertlevels[i] != -1){
			cout << i << ":" << vertlevels[i] << endl;
		}
	}
	cout << endl;
}

int add_values(int vertex, vector<int>* nextlevel,/* cilk::reducer< cilk::op_list_append<int> > int_reducer,*/ int* vertlevels, graph* G, /*pthread_mutex_t m,*/ int currlevel){
	int start = G->firstnbr[vertex];
	int end;
	// if (vertex == G->nv){
		// end = G->ne;
	// }
	// else{
		end = G->firstnbr[vertex+1];
	// }
	int numchanges = 0;
	if (start == end){
		return 0;
	}
	// int len = start - end;
	// cout << "iterating from " << start << " to " << end << endl;
	for (int i = start; i < end; i++){
		if (vertlevels[G->nbr[i]] == -1){
			// cout << "adding " << G->nbr[i] << endl;
			vertlevels[G->nbr[i]] = currlevel;
			// cout <<"pushing" << i << endl;
			
			pthread_mutex_lock(&mutex_global);
			nextlevel->push_back(G->nbr[i]);
			pthread_mutex_unlock(&mutex_global);
			// numchanges++;
		}
	}
	return 0;
}

