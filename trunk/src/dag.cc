#include<vector>
#include "task.h"
#include<string>
#include<stdlib.h>
#include<iostream>
#include"data.h"
/* Creates the DAG dynamically based on the data model.
*/

int convert(std::string s);


std::vector<Task> createDag(Data d){
	std::vector<Task> temptask;
	int end,counter,taskno;
	for(int i = 0; i<d.tasks.size();++i)
	{
		taskno = convert(d.tasks.at(i));
		Task t;
		t.task_no = taskno;
		for(int x = 0; x<d.procs.size();++x) t.exec_times.push_back(d.procs.at(x).at(i));
		temptask.push_back(t);
 
	}

	for(int i = 0; i< d.arcs.size();++i){
		
		taskno = convert(d.arcs.at(i).from);
		int destno = convert(d.arcs.at(i).to);
		links l;
		l.next = destno;
		l.communication = d.arcs.at(i).comm;
		temptask.at(taskno).next.push_back(l);
		temptask.at(destno).prev.push_back(taskno);
	}	
	
	//Test outputs
/*
	for(int i =0; i<temptask.size() ; ++i)
	{
		std::cout << "Task no " << temptask.at(i).task_no << std::endl;
		for(int x = 0; x<temptask.at(i).next.size(); ++x)
		{ std::cout << temptask.at(i).next.at(x).next << std::endl;}
	}*/

	return temptask;


}

int convert(std::string s)
{	
	std::string::size_type start;
	start = s.find("_") + 1;
	std::string toconvert = s.substr(start);
	return atoi(toconvert.c_str());
	
}
