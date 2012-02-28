#include "global.h"
float newRand();


void calcMDPSO()
{
	tasklistMDPSO = tasklist;
	//srand(time(NULL));
	int NumberAgents = 40;
	int maxIter = 10;
	int runtime = 10;
	int lower = 0;
	int upper = tasklistMDPSO.at(0).exec_times.size()-1;
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
	std::vector<int> probproc(tasklistMDPSO.size(), 0);
	std::vector<int> localvalues(NumberAgents);			//pbest	
	std::vector<int> fill(tasklistMDPSO.size(), 0);
	std::vector<float> flfill(tasklistMDPSO.at(0).exec_times.size());		
	std::vector< std::vector<int> > matrixparams(NumberAgents,fill);
	std::vector< std::vector<float> > CRF(tasklistMDPSO.size(), flfill);	

	
	for(int r = 0; r<runtime ; ++r)
	{
		for(int i = 0; i < NumberAgents ; ++i) // Random Initialisation of Solutions
		{
			for(int x = 0; x < tasklistMDPSO.size() ; ++x)
			{											
				rnd = newRand();	
				matrixparams.at(i).at(x)= (int)round( rnd*(upper-lower) + lower );
				if(i < tasklistMDPSO.size())
				{
					if(x<tasklistMDPSO.at(i).exec_times.size())
					{
						CRF.at(i).at(x) = (float)(tasklistMDPSO.at(i).exec_times.at(x)*(float)tasklistMDPSO.at(i).exec_times.size())/(float)tasklistMDPSO.at(i).exec_times.size();
					}
				}
			}

		}
		int finish = 0;
		int firsttime = 1;
		int iter = 0;
		
		do
		{
			//printf("In main loop; \n");
			iter++;
			for(int i = 0; i<NumberAgents ; ++i)
			{
				for(int x = 0; x < tasklistMDPSO.size(); ++x) // Setting DMDPSO list with matrix values
				{
					tasklistMDPSO.at(x).processor = matrixparams.at(i).at(x);
					tasklistMDPSO.at(x).bvalue = tasklistMDPSO.at(x).exec_times.at(tasklistMDPSO.at(x).processor);
				}
				//printf("Initialized solution; \n");
				minval =(int) round(criticalPath(tasklistMDPSO));  //tests and sets for smaller critpath
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

				//printf("Before creating probability; \n");
				for(int x = 0; x< tasklistMDPSO.size();++x)
				{
					float sht = (tasklistMDPSO.at(x).exec_times.at(tasklistMDPSO.at(x).processor))*tasklistMDPSO.size();
					probproc.at(x) = sht/minval;
				}
				//printf("Going into edit loop \n");
				for(int x = 0; x<tasklistMDPSO.size(); ++x)
				{
					rnd = newRand(); // ADD limit testing
					if(rnd>probproc.at(x))
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
							int changed = 0;
							int counter = 0;
							//printf("CRF stuffs; \n");							
							while(!changed && counter < 10)
							{	
								++counter;						
								rndtemp = newRand()*tasklistMDPSO.at(x).exec_times.size();
								for(int a = 0; a<CRF.at(x).size();++a)
								{
									if(CRF.at(x).at(a) < rndtemp)
									{
										matrixparams.at(i).at(x)= CRF.at(x).at(a);
										changed = 1;
										break;
									}
									//printf("Stuck in this loop \n");
									//printf("crf: %f  rand: %f \n",CRF.at(x).at(a), rndtemp);
								}
							}
						}
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
	for(int i = 0; i<tasklistMDPSO.size();++i)
	{
		tasklistMDPSO.at(i).processor = gbestsol.at(i);
		tasklistMDPSO.at(i).bvalue = tasklistMDPSO.at(i).exec_times.at(tasklistMDPSO.at(i).processor);
		tasklistMDPSO.at(i).voltlevel= 0;
	}
	int finalcriticalpath = round(criticalPath(tasklistMDPSO));
	int reducedpath = finalcriticalpath;

	for(int i = 0; i<tasklistMDPSO.size(); ++i)
	{
		int level = 0;
		reducedpath = finalcriticalpath;
		while(reducedpath <= finalcriticalpath && level<=2)
		{
			tasklistMDPSO.at(i).voltage = levels.at(level).voltage;
			tasklistMDPSO.at(i).percentage = levels.at(level).percentage;
					
			level = level + 1;
			std::vector<Task> testlower = tasklistMDPSO;
			testlower.at(i).voltlevel = level;
			testlower.at(i).bvalue = 
				testlower.at(i).exec_times.at(testlower.at(i).processor)*(1/levels.at(level).percentage);
				testlower.at(i).bvalue = testlower.at(i).exec_times.at(testlower.at(i).processor);
			reducedpath = round(criticalPath(testlower));
		}
	}
	
}


