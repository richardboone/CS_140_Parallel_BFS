#include <iostream>
#include <cilk>
#include "final_project.h"
#include <vector>

int main(int argc, char** argv){
	char* filename;
	int numvertices;
	filename = malloc(100*sizeof(char));
	
	if (argc < 3){
		std::cout << "Usage: ./final_project [a,b]  <numvertices> <file_name>" << std::endl;
		exit(-1);
	}
	memcpy(filename, argv[2]);
	numvertices = argv[2];
	
	
	vector<int>  *matrix;
	
	matrix = new vector<int>[numvertices];
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	return 0;
}


print_adjacency_matrix(int* m, int* v){
	int numvertices = m.size();
	int nextmax = 0;
	int prevmax = 0;
	for ( int i = 0; i < numvertices; i++){
		prevmax = nextmax;
		nextmax = v[i+1];
		for (int j = prevmax; j < nextmax; j++){
			std::cout << m[j];
		}
		std::cout << std::endl;
	}
	
}