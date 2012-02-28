#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <vector>
#include <string>
#include <cstring>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include "task.h"
#include "critical.cc"
struct voltagelevels{ // manually setting custom voltage levels
	float percentage;
	float voltage;
};

float newRand()
{
	float r = rand();
	float m = RAND_MAX;
	return r/m;
}
voltagelevels fill;
std::vector<voltagelevels> levels(4, fill);
time_t starttime, endtime, elapsedtime;
float MAKESPAN_INCREASE = 0.3;
std::vector<Task> tasklist, orderedtasks, tasklistPSO, tasklistSSO, tasklistMDPSO, tasklistABC, tasklistRS, tasklistMOSSO;


//look at pso.
std::vector<int> PSObest;
std::vector<float> pbest;
std::vector<float> maxx;
std::vector< std::vector<float> > vx;
std::vector< std::vector<float> > xx;
std::vector< std::vector<float> > tx;
std::vector< std::vector<float> > pbestx;

#endif
