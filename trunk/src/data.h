#ifndef DATA_H
#define DATA_H
#include<string>
#include<vector>
#include"task.h"

struct bvals{
	int task, bval;
};

struct arc{
	std::string from;
	std::string to;
	int comm;
};

class Data{
public:
	std::vector < std::vector <int> > procs;
	std::vector<std::string> tasks;
	std::vector<Task> tree;
	std::vector<arc> arcs;
	Data();
};

Data::Data(){

}



#endif
