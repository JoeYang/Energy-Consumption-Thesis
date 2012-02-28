
#include"task.h"
#include"global.h"
#include"data.h"
#include<vector>
#include<math.h>




void findLocalOpt(int t, int processor, int MIP);

void calcRS(){
	int maxiter = 10;
	int MIP = 0;

		for(int i=0; i < orderedtasks.size(); ++i) // for each task in the ordered blevel list
		{
			int ordertask = orderedtasks.at(i).task_no;
			int maxprnt = 0;
			for(int q = 0; q<tasklistRS.at(ordertask).prev.size();++q) // Finds MIP for communications cost calculation
			{
				if(tasklistRS.at(tasklistRS.at(ordertask).prev.at(q)).bvalue > maxprnt) 
				{
				   	maxprnt = tasklistRS.at(tasklistRS.at(ordertask).prev.at(q)).bvalue;				
					MIP = tasklistRS.at(tasklistRS.at(ordertask).prev.at(q)).task_no;
				}
			}
			for(int x=0; x<tasklistRS.at(ordertask).exec_times.size() ; ++x) // for each processor
			{			
				if(x == 0) tasklistRS.at(ordertask).voltage = levels.at(0).voltage; // set voltage level to max

				findLocalOpt(ordertask, x,MIP); // <-- seperated the actual local optimum search
			
			}
		}
	
}


void findLocalOpt(int tno, int processor, int MIP)
{
	float rsoptimal, commoptimal = 0;
	int commscost = 0;
	if(tasklistRS.at(MIP).processor != processor) commscost = tasklistRS.at(MIP).next.at(0).communication;
	for(int z = 1; z<levels.size(); ++z) // for each voltage level
	{					
		float ebest = pow(tasklistRS.at(tno).voltage,2)*(tasklistRS.at(tno).exec_times.at(tasklistRS.at(tno).processor));// v^2*wi
		float enext = pow(levels.at(z).voltage,2)*tasklistRS.at(tno).exec_times.at(processor);// Vnext^2*wi
		float eftbest = tasklistRS.at(tno).exec_times.at(tasklistRS.at(tno).processor)*tasklistRS.at(tno).percentage; // Current set best execution time
		float eftnext = tasklistRS.at(tno).exec_times.at(processor)*((1-levels.at(z).percentage)+1); // next execution time
		
		float RScalNext = -((ebest - enext) / enext + (eftbest - eftnext) / eftnext); 

		if(RScalNext > 0 && RScalNext > tasklistRS.at(tno).RS)
		{ 
			rsoptimal = RScalNext;
			tasklistRS.at(tno).processor = processor;
			tasklistRS.at(tno).bvalue = round(((1-levels.at(z).percentage)+1) * tasklistRS.at(tasklistRS.at(tno).task_no).exec_times.at(processor));
			tasklistRS.at(tno).RS = rsoptimal;
			tasklistRS.at(tno).voltage = levels.at(z).voltage;
			tasklistRS.at(tno).percentage = (1 - levels.at(z).percentage)+1;
			z = 0; // If you take this out, it isnt relative, but performs better.
		}			
	}

}
