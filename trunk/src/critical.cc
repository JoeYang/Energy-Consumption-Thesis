#include <vector>
#include <algorithm>
#include <iostream>
#include "task.h"
#include "data.h"
#include <math.h>


double maxpath = 0;
std::vector<Task> tasklistcrit;
int calcPath(Task t);

double criticalPath(std::vector<Task> tlist)
{	
	tasklistcrit = tlist;
	//for(int i = 0 ; i< tasklistcrit.size();++i) printf("bvalues: %d \n",tasklistcrit.at(i).bvalue);
	//std::cout << "--- PROBLEM HERE --- " << std::endl;	
	//Attach solution to tasklist *under*
	//recalculate bvalues according to solution
	//find all end nodes
	double critpath = 0;
	for(int i =0; i< tasklistcrit.size(); ++i)
	{
		if(tasklistcrit.at(i).next.empty())
		{
			Task t = tasklistcrit.at(i);		
			int x = calcPath(t);
			//printf("----- path calc: %d \n", x);
			if(x> critpath) critpath = x;
			maxpath = 0;
			//end node
		}

		//std::cout <<"proc: " << tasklist.at(i).processor << " taskno: "<<tasklist.at(i).task_no <<" exectime: "<< tasklist.at(i).bvalue <<std::endl;
	}
	return critpath;


}


int calcPath(Task t)
{		
	if(!t.prev.empty())
	{
		int max = 0;
		int largest = 0;
		//std::cout << "prev: " << t.prev.size() << " first is: " << t.prev.at(0) << std::endl;		
		for(int x = 0; x<t.prev.size();++x)
		{
			if(tasklistcrit.at(t.prev.at(x)).processor != tasklistcrit.at(t.task_no).processor)
				tasklistcrit.at(t.prev.at(x)).bvalue +=tasklistcrit.at(t.prev.at(x)).next.at(0).communication;
			if(tasklistcrit.at(t.prev.at(x)).bvalue > max)
			{				
				max = tasklistcrit.at(t.prev.at(x)).bvalue;
				largest = t.prev.at(x);
				//std::cout << "bvalues: " << max << std::endl;
			}
							
		}
		//std::cout << "tasklargest: " << tasklist.at(largest).task_no << std::endl;
		//if(tasklist.at(largest).prev.empty()) std::cout << " empty " << std::endl;
		maxpath += max;
		calcPath(tasklistcrit.at(largest));
	}
	return maxpath;

}

float calcEnergy(std::vector<Task> tasknrg)
{
	float energy = 0;	
	for(int i = 0; i<tasknrg.size(); ++i)
	{
		energy+=pow(tasknrg.at(i).voltage,2)*tasknrg.at(i).exec_times.at(tasknrg.at(i).processor)*(1/tasknrg.at(i).percentage);
	}
	return energy;
}
