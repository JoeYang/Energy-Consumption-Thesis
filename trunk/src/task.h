#ifndef TASK_H
#define TASK_H
#include<vector>
struct links{
	int next,communication;
};


class Task{
public:


	std::vector<int> exec_times,prev;
	std::vector<links> next;
	int task_no, processor,bvalue,EST, MIP;
	float voltage, voltlevel,percentage,RS;





	Task();
};

Task::Task(){ voltlevel = 0;RS = 0; processor = 0; bvalue = 0; percentage = 1;


}


#endif
