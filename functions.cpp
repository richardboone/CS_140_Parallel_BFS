#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "final_project.h"
#include <list>
#include <time.h>
// #include <cilk/cilk.h>
// #include <cilk/reducer_vector.h>
#include <pthread.h>
using namespace std;

using namespace std;
pthread_mutex_t mutex_global;
pthread_mutex_t m;

graph* graph_from_edge_list(vector< vector<long> > *matrix, long numedges) {
  graph *G = new graph;
  G->nv = matrix->size();
  G->ne = numedges;
  cout << numedges << endl;
  G->nbr = new long[G->ne];
  G->firstnbr = new long[G->nv+1];

  int nbr_index = 0;
  int firstnbr_index = 0;
  for (long i = 0; i < matrix->size(); i++) {
    G->firstnbr[firstnbr_index] = nbr_index;
    for (long j = 0; j < matrix->at(i).size(); j++) {
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
  for (long i = 0; i < G->ne; i++) {
    cout << G->nbr[i] << ", ";
  }  
  cout << endl;
  cout << "first_nbr = ";
  for (long j = 0; j < G->nv + 1; j++) {
    cout << G->firstnbr[j] << ", ";
  }
  cout << endl;
}

void input_vector(vector< vector<long> > *matrix, char* filename, long &numedges){
  ifstream inFile;
  inFile.open(filename);
  long start, stop;
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


int getNumAdjacencies(graph *G, int v, int indexOfFirstnbr) {
  int numAdjacencies;
  //if (numAdjacencies == G->ne-1) {
  /*
  if (v == G->nv-1) {
    numAdjacencies = G->ne - indexOfFirstnbr;
  }
  else {
    numAdjacencies = G->firstnbr[v+1] - G->firstnbr[v];
  }
  */
  numAdjacencies = G->firstnbr[v+1] - G->firstnbr[v];
  return numAdjacencies;
}

void vector_BFS(graph* G){
	clock_t begin, end;
	begin = clock();
	vector<long> nextlevel1;
	vector<long> nextlevel2;
	cilk::reducer<cilk::op_vector<long> > r;
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
	int currlevel = 0;
	while(nextlevel1.size() > 0){
		currlevel++;
		cilk_for(int i = 0; i < nextlevel1.size(); i++){
			add_values(nextlevel1[i], &r, vertlevels, G, currlevel);
		}
		nextlevel1.clear();
		r.move_out(nextlevel2);
		if (nextlevel2.size()==0){
			break;
		}	
		currlevel++;
		cilk_for(int i = 0; i < nextlevel2.size(); i++){
			add_values(nextlevel2[i], &r, vertlevels, G, currlevel);
		}
		r.move_out(nextlevel1);
		nextlevel2.clear();
	
	}
	end = clock();
	double elapsed_time = (double)(end - begin)/CLOCKS_PER_SEC;
	printvertices(numvertices, vertlevels);
	cout << elapsed_time << endl;
}

void printvertices(long numvertices, int* vertlevels){
	int max = 0;
	cout << "vertex:depth\n";
	for (long i = 0; i < numvertices; i++){
		if (vertlevels[i] != -1){
			cout << i << ":" << vertlevels[i] << endl;
			if (vertlevels[i] > max){
				max = vertlevels[i];
			}
			
		}
	}
	cout << endl;
	cout << "maxdepth: " << max << endl;
	vector<int> depths(max);
	for (long i = 0; i < numvertices; i++){
		if (vertlevels[i] != -1){
			depths[vertlevels[i]]++;
		}
	}
	cout << "nodes at each depth" << endl;
	for (long i = 0; i <= max; i++){
		cout << i << ": " << depths[i] << endl;
	}
	
	
}

void add_values(long vertex, cilk::reducer<cilk::op_vector<long> >* r, int* vertlevels, graph* G, int currlevel){
	long start = G->firstnbr[vertex];
	long end;
	if (vertex == G->nv){
		end = G->ne;
	}
	else{
		end = G->firstnbr[vertex+1];
	}
	int numchanges = 0;
	if (start == end){
		return;
	}
	int len = start - end;
	// cout << "iterating from " << start << " to " << end << endl;
	for (long i = start; i < end; i++){
		if (vertlevels[G->nbr[i]] == -1){
			// cout << "adding " << G->nbr[i] << endl;
			// pthread_mutex_lock(&m);
			vertlevels[G->nbr[i]] = currlevel;
			// nextlevel->push_back(G->nbr[i]);
			(**r).push_back(G->nbr[i]);
			// pthread_mutex_unlock(&m);
			// numchanges++;
		}
	}
	return;
}

void bfs_bag(graph *G, long s, int level[]) {
  for (int n = 0; n < G->nv; n++) {
    level[n] = -1;
  }
  int levelnum = 0;
  
  Bag frontier(1);
  Bag visited(1);
  frontier.insert(s);
  visited.insert(s);
  
  int numAdjacencies;
  //pthread_mutex_t m;
  while (!frontier.isEmpty()) {
    Bag succbag(1);
    cilk_for (int i = 0; i < frontier.size(); i++) {
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
	  pthread_mutex_lock(&m);

	  succbag.insert(v);
	  visited.insert(v);

	  pthread_mutex_unlock(&m);
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

// void vector_BFS(graph* G){
	// vector< int> nextlevel1;
	// vector< int> nextlevel2;
	// int* vertlevels;
	////pthread_mutex_t m;
	// int numedges = G->ne;
	// int numvertices = G->nv;
	// vertlevels = (int*)malloc((numvertices) * sizeof(int));
	// for (int i = 0; i < numvertices; i++){
		// vertlevels[i] = -1;
	// }
	// nextlevel1.push_back(1);
	// vertlevels[1] = 0;
	////cilk::reducer< cilk::op_list_append<int> > int_reducer1;
	////cilk::reducer< cilk::op_list_append<int> > int_reducer2;
	// int currlevel = 0;
	// while(nextlevel1.size() > 0){
		// currlevel++;
		////cout << endl;
		// cilk_for(int i = 0; i < nextlevel1.size(); i++){
			// add_values(nextlevel1[i], &nextlevel2,/* int_reducer1,*/ vertlevels, G, currlevel);
		// }
		// if (nextlevel2.size()==0){
			// break;
		// }	
		// currlevel++;
		// cilk_for(int i = 0; i < nextlevel2.size(); i++){
			// add_values(nextlevel2[i], &nextlevel1, /*int_reducer1,*/ vertlevels, G, currlevel);
		// }
		// nextlevel2.clear();
	// }
	// printvertices(numvertices, vertlevels);
	// free(vertlevels);
// }