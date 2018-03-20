#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "final_project.h"
// #include <cilk/cilk.h>
// #include <cilk/reducer_vector.h>

using namespace std;

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
  for (int j = 0; j < G->nv + 1; j++) {
    cout << G->firstnbr[j] << ", ";
  }
  cout << endl;
}

void input_vector(vector< vector<int> > *matrix, char* filename, int &numedges, char direction){
  ifstream inFile;
  inFile.open(filename);
  int start, stop;
  while(inFile >> start >> stop) {
    matrix->at(start).push_back(stop);
	if (direction == 'u'){
		matrix->at(stop).push_back(start);
	}
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

void vector_BFS(graph* G){
	// print_CSR_graph(G);
	// int size1 = 1, size2 = 0;
	vector< int> nextlevel1;
	vector< int> nextlevel2;
	// cilk::reducer< cilk::op_add<double> > size1(0);
	// cilk::reducer< cilk::op_add<double> > size2(0);
	// cilk::reducer<  cilk::op_list_append<int>  > nextlevel1;
	// cilk::reducer<  cilk::op_list_append<int>  > nextlevel2;
	// cilk::op_vector_view<int> nextlevel2;
	// cilk::reducer< cilk::op_vector<int> > nextlevel1;
	// cilk::reducer< cilk::op_vector<int> > nextlevel2;
	int* vertlevels;
	pthread_mutex_t m;
	int numedges = G->ne;
	int numvertices = G->nv;
	vertlevels = (int*)malloc((numvertices) * sizeof(int));
	for (int i = 0; i < numvertices; i++){
		vertlevels[i] = -1;
	}
	nextlevel1.push_back(1);
	// *size1 += 1;
	// int value1, value2;
	vertlevels[1] = 0;
	int currlevel = 0;
	while(nextlevel1.size() > 0){
		currlevel++;
		// cout << "restarting loop" << endl;
		// value1 = size1.get_value();
		// for (int i = 0; i < nextlevel1.size(); i++){
			// cout << nextlevel1[i] << " ";
		// }
		// cout << endl;
		cilk_for(int i = 0; i < nextlevel1.size(); i++){
			add_values(nextlevel1[i], &nextlevel2, vertlevels, G, m, currlevel);
		}
		// printvertices(numvertices, vertlevels);
		// cout << "passed first cilk_for" << endl;
		// cout << nextlevel1.size() << " vertices added" << endl;
		// nextlevel1.clear();
		// size1.set_value(0);
		// cout << nextlevel2.size() << endl;
		if (nextlevel2.size()==0){
			// cout << "broke at level " << currlevel
			break;
		}	
		// cout << "got ehre" << endl;
		currlevel++;
		// value2 = size2.get_value();
		// for (int i = 0; i < nextlevel2.size(); i++){
			// cout << nextlevel2[i] << " ";
		// }
		// cout << endl;
		cilk_for(int i = 0; i < nextlevel2.size(); i++){
			add_values(nextlevel2[i], &nextlevel1, vertlevels, G, m, currlevel);
		}
		// cout << nextlevel1.size() << endl;
		// printvertices(numvertices, vertlevels);
		// cout << "passed second cilk_for" << endl;
		// cout << nextlevel2.size() << " vertices added" << endl;
		nextlevel2.clear();
		// size2.set_value(0);
		// for (int i = 0; i < numvertices; i++){
			// cout << vertlevels[i] << " ";
		// }
		// cout << endl;
		// break;
	}
	// cout << 
	printvertices(numvertices, vertlevels);
	// for (int i = 1; i < numvertices; i++){
		// cout << vertlevels[i] << " ";
	// }
	// cout << endl;
	// cout << "finished counting list" << endl;
}

void printvertices(int numvertices, int* vertlevels){
	cout << "vertex:depth\n";
	for (int i = 1; i < numvertices; i++){
		if (vertlevels[i] != -1){
			cout << i << ":" << vertlevels[i] << endl;
		}
		// cout << vertlevels[i] << " ";
	}
	cout << endl;
}

int add_values(int vertex, vector<int>* nextlevel, int* vertlevels, graph* G, pthread_mutex_t m, int currlevel){
	int start = G->firstnbr[vertex];
	int end;
	if (vertex == G->nv){
		end = G->ne;
	}
	else{
		end = G->firstnbr[vertex+1];
	}
	int numchanges = 0;
	if (start == end){
		return 0;
	}
	int len = start - end;
	// cout << "iterating from " << start << " to " << end << endl;
	for (int i = start; i < end; i++){
		if (vertlevels[G->nbr[i]] == -1){
			// cout << "adding " << G->nbr[i] << endl;
			// pthread_mutex_lock(&m);
			vertlevels[G->nbr[i]] = currlevel;
			nextlevel->push_back(G->nbr[i]);
			// pthread_mutex_unlock(&m);
			// numchanges++;
		}
	}
	return 0;
}