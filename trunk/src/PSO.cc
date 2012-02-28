#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>            	            	
#include <time.h>						
#include "global.h"
#include <vector>

void calcPSO()
{
	tasklistPSO = tasklist;
	//srand(time(NULL));
	int NumberAgents = 40;
	int maxIter = 10;
	int runtime = 10;
	int maxV = tasklistPSO.at(0).exec_times.size();
	int maxX = tasklistPSO.at(0).exec_times.size();
	int lower = 0;
	int upper = tasklistPSO.at(0).exec_times.size()-1;
	int minval;
	float weight = 0.9;
	float weight_up;
	float rnd;

	int gbest = 0; // initially assume first particle is best
	int GlobalBest = 1000000;
	std::vector<int> PSOGlobalBest;
	std::vector<int> localvalues(NumberAgents);			//pbest	
	std::vector<int> fill(tasklistPSO.size(), 0);		
	std::vector< std::vector<int> > matrixparams(NumberAgents,fill);	//xx
	std::vector< std::vector<int> > velocity(NumberAgents,fill);		//vx
	std::vector< std::vector<int> > transition(NumberAgents,fill);	//tx
	std::vector< std::vector<int> > localparams(NumberAgents,fill);	//pbestx  //((double)rand()/((double)(RAND_MAX)+(double)(1)))

	PSOGlobalBest.resize(tasklistPSO.size());
	
	for(int r = 0; r<runtime ; ++r)
	{
		for(int i = 0; i < NumberAgents ; ++i)
		{
			for(int x = 0; x < tasklistPSO.size() ; ++x)
			{								
				rnd = (double)rand() / (double)RAND_MAX;		
				matrixparams.at(i).at(x)= (int)round( rnd*(upper-lower) + lower );
				localparams.at(i).at(x) = matrixparams.at(i).at(x);
				rnd = (double)rand() / (double)RAND_MAX;
				velocity.at(i).at(x) = (int)round(maxV*rnd);
				rnd = (double)rand() / (double)RAND_MAX;
				if(rnd > 0.5) velocity.at(i).at(x) = -velocity.at(i).at(x);
			}

		}
		int finish = 0;
		int firsttime = 1;
		int iter = 0;
		
		do
		{
			iter++;
			weight_up = weight;//(weight-0.4) * ((maxIter - iter) /maxIter) +0.4;
			for(int i = 0; i<NumberAgents ; ++i)
			{
				for(int x = 0; x < tasklistPSO.size(); ++x) // Setting PSO list with matrix values
				{
					tasklistPSO.at(x).processor = matrixparams.at(i).at(x);
					tasklistPSO.at(x).bvalue = tasklistPSO.at(x).exec_times.at(tasklistPSO.at(x).processor);
				}
				minval =(int) round(criticalPath(tasklistPSO));
				if(firsttime==1) localvalues.at(i) = minval;
				if(minval < localvalues.at(i))
				{
					localvalues.at(i) = minval;
					for(int z = 0; z<tasklistPSO.size();++z) localparams.at(i).at(z) = matrixparams.at(i).at(z);
					if(localvalues.at(i) < localvalues.at(gbest)) gbest = i;
				}
				for(int x = 0; x<tasklistPSO.size(); ++x)
				{
					rnd = (double)rand() / (double)RAND_MAX;
					float randtemp = (double)rand() / (double)RAND_MAX;					
					velocity.at(i).at(x) = (int)round(weight_up*velocity.at(i).at(x) + 2*rnd*(localparams.at(i).at(x) - matrixparams.at(i).at(x)) + 2*randtemp*(localparams.at(gbest).at(x) - matrixparams.at(i).at(x))) ;
					if(velocity.at(i).at(x) > maxV) velocity.at(i).at(x) = maxV;
					else if(velocity.at(i).at(x) < -maxV) velocity.at(i).at(x) = -maxV;

					transition.at(i).at(x) = matrixparams.at(i).at(x) + velocity.at(i).at(x);
					if(transition.at(i).at(x) > upper) transition.at(i).at(x) = upper;
					if(transition.at(i).at(x) < lower) transition.at(i).at(x) = lower;
				}
			}
			for(int i = 0; i < NumberAgents; ++i)
			{
				for(int x = 0; x<tasklistPSO.size();++x)
				{
					matrixparams.at(i).at(x) = transition.at(i).at(x);
				}
			}
			if(iter >= maxIter) finish = 1;
			firsttime = 1;
		}while(!finish);
		if(localvalues.at(gbest) < GlobalBest)
		{
			GlobalBest = localvalues.at(gbest);
			for(int i = 0; i<tasklistPSO.size(); ++i) PSOGlobalBest.at(i) = localparams.at(gbest).at(i);
		}
	}
	for(int i = 0; i<tasklistPSO.size();++i)
	{
	
		tasklistPSO.at(i).processor = PSOGlobalBest.at(i);
		tasklistPSO.at(i).bvalue = tasklistPSO.at(i).exec_times.at(tasklistPSO.at(i).processor);
		tasklistPSO.at(i).voltlevel= 0;
	}
	int finalcriticalpath = round(criticalPath(tasklistPSO));
	int reducedpath = finalcriticalpath;

	for(int i = 0; i<tasklistPSO.size(); ++i)
	{
		int level = 0;
		reducedpath = finalcriticalpath;
		while(reducedpath <= finalcriticalpath && level<=2)
		{
			tasklistPSO.at(i).voltage = levels.at(level).voltage;
			tasklistPSO.at(i).percentage = levels.at(level).percentage;					
			level = level + 1;
			std::vector<Task> testlower = tasklistPSO;
			testlower.at(i).voltlevel = level;
			testlower.at(i).bvalue = 
				testlower.at(i).exec_times.at(testlower.at(i).processor)*(1/levels.at(level).percentage);
				testlower.at(i).bvalue = testlower.at(i).exec_times.at(testlower.at(i).processor);
			reducedpath = round(criticalPath(testlower));
		}
	}
	//endtime = time(NULL);
	//elapsedtime = endtime - starttime;
	//std::cout << "Elapsed Time: "<< elapsedtime << std::endl;
	
}
