          	              							
#include "global.h"
float newRand();


void calcMOSSO()
{
	tasklistMOSSO = tasklist;
	//srand(time(NULL));
	int NumberAgents = 40;
	int maxIter = 10;
	int runtime = 10;
	int lower = 0;
	int upper = tasklistMOSSO.at(0).exec_times.size()-1;
	float minval;
	float minnrg;
        double Cw = 0.1;
        double Cp = 0.4;
        double Cg = 0.9;
	//Energy Vs Exectime tradeoff:
	float weightExec = 0.3;
	float weightEnergy = 0.7;
	
	float rnd;
	float rndtemp;

	int gbest = 0; // initially assume first particle is best
	int pbest = 0; // initially assume local best is first particle
	float pbestval;
	float pbestnrgval;
	float gbestnrgval;
	float gbestval;
	int GlobalBest = 100000;
	std::vector<int> gbestsol;
	std::vector<int> gbestDVS;
	std::vector<int> pbestsol;
	std::vector<int> pbestDVS;
	std::vector<int> probproc(tasklistMOSSO.size(), 0);
	std::vector<int> fill(tasklistMOSSO.size(), 0);		
	std::vector< std::vector<int> > matrixparams(NumberAgents,fill);	
	std::vector< std::vector<int> > DVSsettings(NumberAgents,fill);

	
	for(int r = 0; r<runtime ; ++r)
	{
		for(int i = 0; i < NumberAgents ; ++i) // Random Initialisation of Solutions
		{
			for(int x = 0; x < tasklistMOSSO.size() ; ++x)
			{											
				rnd = newRand();	
				matrixparams.at(i).at(x)= (int)round( rnd*(upper-lower) + lower );
				DVSsettings.at(i).at(x) = (int)round( rnd*(levels.size()-1) );
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
				for(int x = 0; x < tasklistMOSSO.size(); ++x) // Setting DMOSSO list with matrix values
				{
					tasklistMOSSO.at(x).processor = matrixparams.at(i).at(x);
					tasklistMOSSO.at(x).bvalue = tasklistMOSSO.at(x).exec_times.at(tasklistMOSSO.at(x).processor);
					tasklistMOSSO.at(x).voltage = levels.at(DVSsettings.at(i).at(x)).voltage;

				}

				minval = criticalPath(tasklistMOSSO);  //tests and sets for smaller critpath
				minnrg = calcEnergy(tasklistMOSSO); // tests for energy consumption
							
				//for different energy levels go through and calculate the percentage loss/gain.
				if(i==0)
				{			
					pbestsol = matrixparams.at(i);
					pbestDVS = DVSsettings.at(i);
					pbestval = minval;
					pbestnrgval = minnrg;
				}
				//Calculate the hybrid fitness value:
				float diffexec = ((pbestval-minval)/pbestval) * weightExec;
				float diffnrg = ((pbestnrgval-minnrg)/pbestnrgval) * weightEnergy;
				float hybridfitness = diffexec + diffnrg; // +ve if saving combination -ve if losing

				//printf("exec: %f , nrg: %f , hybrid: %f \n", diffexec, diffnrg, hybridfitness);
				if(hybridfitness >= 0)
				{
					pbestsol = matrixparams.at(i);
					pbestDVS = DVSsettings.at(i);
					pbestval = minval;
					pbestnrgval = minnrg;
				}					

				for(int x = 0; x<tasklistMOSSO.size(); ++x)
				{
					rnd = newRand();
					if(rnd < Cw) ;
					if(rnd >= Cw && rnd < Cp)
					{
						matrixparams.at(i).at(x) = pbestsol.at(x);
						DVSsettings.at(i).at(x) = pbestDVS.at(x);
					}
					if(rnd >= Cp && rnd < Cg)
					{
						if(firsttime == 1) { 
							matrixparams.at(i).at(x) = pbestsol.at(x);
							DVSsettings.at(i).at(x) = pbestDVS.at(x);} // UP TO HERE
						else { 	matrixparams.at(i).at(x) = gbestsol.at(x); 
							DVSsettings.at(i).at(x) = gbestDVS.at(x);}
					}
					if(rnd > Cg)
					{
						rndtemp = newRand();
						matrixparams.at(i).at(x)= (int)round( rndtemp*(upper-lower) + lower );
						DVSsettings.at(i).at(x) = (int) round(rndtemp*(levels.size()-1));
					}
				}
			}
			if(iter >= maxIter) finish = 1;
		}while(!finish);		

		if(pbestval < gbestval || firsttime == 1)
		{
			gbestval = pbestval;
			gbestnrgval = pbestnrgval;
			gbestsol = pbestsol;
			gbestDVS = pbestDVS;
			//gbestnrgval = pbestnrgval;
		}
		firsttime = 0;
	}	

	for(int i = 0; i<tasklistMOSSO.size();++i)
	{
	
		tasklistMOSSO.at(i).processor = gbestsol.at(i);
		tasklistMOSSO.at(i).bvalue = tasklistMOSSO.at(i).exec_times.at(tasklistMOSSO.at(i).processor);
		tasklistMOSSO.at(i).voltage = levels.at(gbestDVS.at(i)).voltage;
		//printf("%f \n", levels.at(gbestDVS.at(i)).voltage);
	}	
}


