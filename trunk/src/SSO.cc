          	              							
#include "global.h"


void calcSSO()
{
	tasklistSSO = tasklist;
	//srand(time(NULL));
	int NumberAgents = 40;
	int maxIter = 10;
	int runtime = 10;
	int lower = 0;
	int upper = tasklistSSO.at(0).exec_times.size()-1;
	int minval;
        double Cw = 0.1;
        double Cp = 0.4;
        double Cg = 0.9;

	float rnd;
	float rndtemp;

	int gbest = 0; // initially assume first particle is best
	int pbest = 0; // initially assume local best is first particle
	int pbestval;
	int gbestval = 100000;
	int GlobalBest = 100000;
	std::vector<int> gbestsol;
	std::vector<int> pbestsol;
	std::vector<int> localvalues(NumberAgents);			//pbest	
	std::vector<int> fill(tasklistSSO.size(), 0);		
	std::vector< std::vector<int> > matrixparams(NumberAgents,fill);	

	
	for(int r = 0; r<runtime ; ++r)
	{
		for(int i = 0; i < NumberAgents ; ++i) // Random Initialisation of Solutions
		{
			for(int x = 0; x < tasklistSSO.size() ; ++x)
			{											
				rnd = newRand();	
				matrixparams.at(i).at(x)= (int)round( rnd*(upper-lower) + lower );
			}

		}
		int finish = 0;
		int firsttime = 1;
		int iter = 0;
		
		do
		{
			iter++;
			for(int i = 0; i<NumberAgents ; ++i)
			{
				for(int x = 0; x < tasklistSSO.size(); ++x) // Setting DSSO list with matrix values
				{
					tasklistSSO.at(x).processor = matrixparams.at(i).at(x);
					tasklistSSO.at(x).bvalue = tasklistSSO.at(x).exec_times.at(tasklistSSO.at(x).processor);
				}

				minval =(int) round(criticalPath(tasklistSSO));  //tests and sets for smaller critpath	
				
				//for different energy levels go through and calculate the percentage loss/gain.
				if(i==0)
				{			
					localvalues.at(i) = minval;
					pbestsol = matrixparams.at(i);
					pbestval = minval;
				}

				if(minval < pbestval)
				{
					pbestsol = matrixparams.at(i);
					pbestval = minval;
				}					
				localvalues.at(i) = minval;


				for(int x = 0; x<tasklistSSO.size(); ++x)
				{
					rnd = newRand();
					if(rnd < Cw) ;
					if(rnd >= Cw && rnd < Cp)
					{
						matrixparams.at(i).at(x) = pbestsol.at(x);
					}
					if(rnd >= Cp && rnd < Cg)
					{
						if(firsttime == 1) { matrixparams.at(i).at(x) = pbestsol.at(x);}
						else { matrixparams.at(i).at(x) = gbestsol.at(x); }
					}
					if(rnd > Cg)
					{
						rndtemp = newRand();
						matrixparams.at(i).at(x)= (int)round( rndtemp*(upper-lower) + lower );
					}
				}
			}
			if(iter >= maxIter) finish = 1;
		}while(!finish);
		firsttime = 0;
		if(pbestval < gbestval)
		{
			gbestval = pbestval;
			gbestsol = pbestsol;
		}
	}
	for(int i = 0; i<tasklistSSO.size();++i)
	{
	
		tasklistSSO.at(i).processor = gbestsol.at(i);
		tasklistSSO.at(i).bvalue = tasklistSSO.at(i).exec_times.at(tasklistSSO.at(i).processor);
		tasklistSSO.at(i).voltlevel= 0;
	}
	int finalcriticalpath = round(criticalPath(tasklistSSO));
	int reducedpath = finalcriticalpath;

	for(int i = 0; i<tasklistSSO.size(); ++i)
	{
		int level = 0;
		reducedpath = finalcriticalpath;
		while(reducedpath <= finalcriticalpath && level<=2)
		{
			tasklistSSO.at(i).voltage = levels.at(level).voltage;
			tasklistSSO.at(i).percentage = levels.at(level).percentage;		
			level = level + 1;
			std::vector<Task> testlower = tasklistSSO;
			testlower.at(i).voltlevel = level;
			testlower.at(i).exec_times.at(testlower.at(i).processor) = 
				testlower.at(i).exec_times.at(testlower.at(i).processor)*(1/levels.at(level).percentage);
			testlower.at(i).bvalue = testlower.at(i).exec_times.at(testlower.at(i).processor);
			reducedpath = round(criticalPath(testlower));
		}
	}
	
}


