#include <vector>
#include <algorithm>
#include <iostream>
#include "task.h"
#include "data.h"
#include "global.h"

struct sort_funct{

bool operator()(Task const& a, Task const& b)
{
	return a.bvalue > b.bvalue; 
}

};

int max_proc(Task t);
void calcUp(Task t);

void blevel(){
	//find all end nodes
	for(int i =0; i< tasklist.size(); ++i)
	{
		if(tasklist.at(i).next.empty())
		{
			tasklist.at(i).bvalue = max_proc(tasklist.at(i)) + tasklist.at(tasklist.at(i).prev.at(0)).next.at(0).communication;
			//function call recursively calculate bvals				
			calcUp(tasklist.at(i));			
		}
	}
	//sort by decreasing bval
	orderedtasks = tasklist;
	std::vector<Task>::iterator it;
	std::sort(orderedtasks.begin(), orderedtasks.end(),sort_funct());
}

void calcUp(Task t)
{
	for(int q = 0; q < t.prev.size(); ++q)
	{			
		Task p = tasklist.at(t.prev.at(q));
		for(int x = 0; x<p.next.size();++x)
		{
			if(p.next.at(x).next == t.task_no)
			{
				int max = max_proc(t) + p.next.at(x).communication;
				//printf("max: %d maxproc: %d comm: %d b \n", max,max_proc(t),p.next.at(x).communication);
				if(tasklist.at(t.task_no).bvalue < max){ tasklist.at(t.task_no).bvalue = max; }
			}		
		}
		if(p.prev.size() > 0) calcUp(p);
		else tasklist.at(p.task_no).bvalue = max_proc(p); 	
	}
	
}

int max_proc(Task t)
{
	int max = 0;	
	for(int z = 0; z<t.exec_times.size();++z)
	{
		if(t.exec_times.at(z) > max) max = t.exec_times.at(z);
	}
	return max;
}
