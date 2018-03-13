#include <fstream>
#include "final_project.h"






input_vector(vector<int> *matrix, char* filename){
	std::ifstream inFile;
	inFile.open(filename);
	int start, stop;
	while(!inFile.eof()){
		inFile >> start >> stop;
		matrix[start].push_back(stop);
	}
}