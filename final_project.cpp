#include <iostream>
#include <cilk>
#include "final_project.h"


int main(int argc, char** argv){
	char* filename;
	
	
	if (argc < 2){
		std::cout << "Usage: ./final_project [a,b] <file_name> << std::endl;
		exit(-1);
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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