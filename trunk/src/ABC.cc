#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "data.h"
#include "task.h"
#include <vector>
#include "global.h"


int FoodNumber; 	// Number of different potential solutions within foods
int ub;			// Upper Bound of Parameters
int lb;			// Lower Bount of Parameters
int GlobalMin;		// Current Global Min
int limit;
int randparam, randsol, FinalGlobalMin;
double r;		// Random number
std::vector<std::vector<int> > Foods; // 2D array of comparitive food solutions
std::vector<int> solution, fitness, trial, GlobalParams, FinalGlobalParams;
std::vector<double> prob;
int calcFitness(std::vector<int> sol);
void improveSource(int i);
void sendScoutBees();
void sendOnlookerBees();
void calculateProbabilities();
void sendEmployedBees();
void initial();


void calcABC()
{
	tasklistABC = tasklist;	
	//srand(time(NULL));
	ub = tasklistABC.at(0).exec_times.size() - 1;
	lb = 0;
	int maxIter = 10;
	int runtime = 10;
	FoodNumber = 40;
	limit = 100;
	solution.resize(tasklistABC.size());
	fitness.resize(FoodNumber);
	trial.resize(FoodNumber);
	prob.resize(FoodNumber);
	std::vector<int> d(tasklistABC.size(),0);
	Foods.resize(FoodNumber);
	for(int i = 0;i<FoodNumber;++i) Foods.at(i) = d;

	for(int i = 0; i<runtime ; ++i)
	{
		initial();
		for(int j = 0; j<maxIter ; ++j)
		{

			sendEmployedBees();
			calculateProbabilities();
			sendOnlookerBees();
			sendScoutBees();
		}
		if(i == 0){ FinalGlobalMin = GlobalMin; FinalGlobalParams = GlobalParams;}		
		if(i != 0 && FinalGlobalMin > GlobalMin)
		{
			FinalGlobalMin = GlobalMin;
			FinalGlobalParams = GlobalParams;
			
		}
	}
	
	for(int i = 0; i< FinalGlobalParams.size(); ++i)
	{
		tasklistABC.at(i).processor = FinalGlobalParams.at(i);
		tasklistABC.at(i).bvalue = tasklistABC.at(i).exec_times.at(tasklistABC.at(i).processor);
		tasklistABC.at(i).voltlevel= 0;
	}
	int finalcriticalpath = round(criticalPath(tasklistABC));
	int reducedpath = finalcriticalpath;
	bool lower = true;

	for(int i = 0; i<FinalGlobalParams.size(); ++i)
	{
		int level = 0;
		reducedpath = finalcriticalpath;
		while(reducedpath <= finalcriticalpath && level<=2)
		{
			tasklistABC.at(i).voltage = levels.at(level).voltage;
			tasklistABC.at(i).percentage = levels.at(level).percentage;
			level = level + 1;
			std::vector<Task> testlower = tasklistABC;
			testlower.at(i).voltlevel = level;
			testlower.at(i).bvalue = 
				testlower.at(i).exec_times.at(testlower.at(i).processor)*(1/levels.at(level).percentage);
				testlower.at(i).bvalue = testlower.at(i).exec_times.at(testlower.at(i).processor);
			reducedpath = round(criticalPath(testlower));
		}
	}		

}

void initial()
{	
		
	for(int i = 0; i< FoodNumber;++i)	//Randomizes intial setup
	{
		for(int j = 0; j < tasklistABC.size(); ++j)
		{	
			r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			Foods.at(i).at(j) = round(r*(ub-lb)+lb);
			solution.at(j) = Foods.at(i).at(j);
		}
		fitness.at(i) = calcFitness(solution);
		if(i == 0)			//Sets Initial Min to first fitness Value
		{
			GlobalParams = solution;
			GlobalMin = fitness.at(i);
		}
		if(i != 0 )			//Progressively finds Min val and saves parameters.
		{
			if(fitness.at(i) < GlobalMin)
			{
				GlobalMin = fitness.at(i);
				GlobalParams = Foods.at(i);
			}
		}
		trial.at(i) = 0;
	}
}

void sendEmployedBees()
{	
	for(int i = 0 ; i<FoodNumber ; ++i)
	{
		improveSource(i);
	}
}

void calculateProbabilities()
{
	int max = 0;
	for(int i = 0; i< fitness.size() ; ++i)
	{
		if(fitness.at(i) > max) max = fitness.at(i);
	}
	for(int i = 0; i < FoodNumber; ++i)
	{
		prob.at(i) = (0.9*(fitness.at(i)/max))+0.1;
	}	
}

void sendOnlookerBees()
{
	int counter = 0;
	int i = 0;
	while(counter<FoodNumber)
	{	
		r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
		if(r<prob.at(i))
		{
			counter++;
			improveSource(i);
		}
		i++;
		if(i==FoodNumber) i = 0;
	}
}

void sendScoutBees()
{
	int maxindex = 0;
	for(int i = 1; i<FoodNumber ; ++i)
	{
		if(trial.at(i) > trial.at(maxindex)) maxindex = i;
	}
	if(trial.at(maxindex) >= limit)
	{
		for(int j = 0; j < tasklistABC.size(); ++j)
		{	
			r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
			Foods.at(maxindex).at(j) = round(r*(ub-lb)+lb);
			solution.at(j)=Foods.at(maxindex).at(j);
		}
		fitness.at(maxindex) = calcFitness(solution);
		trial.at(maxindex) = 0;
	}
}

void improveSource(int i)
{
	r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
	randparam = round(r*(tasklistABC.size()-1));
	r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
	randsol = round(r*(FoodNumber-1));
	while(randsol == i)
	{
		r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
		randsol = round(r*(FoodNumber-1));
	}
	solution = Foods.at(i);
	r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
	solution.at(randparam) = Foods.at(i).at(randparam) + ( Foods.at(i).at(randparam) - Foods.at(randsol).at(randparam))*(r-0.5)*2; //???
	if(solution.at(randparam) > ub) solution.at(randparam) = ub;
	if(solution.at(randparam) < lb) solution.at(randparam) = lb;
	int currFitness = calcFitness(solution);
	if(currFitness < fitness.at(i))
	{
		trial.at(i) = 0;
		Foods.at(i) = solution;
		fitness.at(i) = currFitness;
	}
	else { trial.at(i) += 1;}
}
//Fitness Function
int calcFitness(std::vector<int> sol)
{	
	for(int i = 0; i<tasklistABC.size(); ++i)
	{
		tasklistABC.at(i).processor= sol.at(i);
		tasklistABC.at(i).bvalue = tasklistABC.at(i).exec_times.at(tasklistABC.at(i).processor);
	}
	return round(criticalPath(tasklistABC));;
}


