#include <iostream>
#include "parse.cc"
#include "global.h"
#include "data.h"
#include "dag.cc"
#include "blevel.cc"
#include "RS.cc"
#include "ABC.cc"
#include "PSO.cc"
#include "SSO.cc"
#include "MDPSO.cc"
#include "MOSSO.cc"
#include <sys/types.h>
#include <dirent.h>

void RS();
void ABC();
void PSO();
void SSO();
void MDPSO();
void MOSSO();
std::vector<std::string> getdir(std::string dir);
std::vector<int> SSOresults, RSresults, ABCresults, PSOresults, MDPSOresults,MOSSOresults;
std::vector<int> RStime, PSOtime, SSOtime, ABCtime, MDPSOtime;
std::ofstream RSout("Output/RS.txt");
std::ofstream ABCout("Output/ABC.txt");
std::ofstream PSOout("Output/PSO.txt");
std::ofstream SSOout("Output/SSO.txt");
std::ofstream MDPSOout("Output/MDPSO.txt");
std::ofstream MOSSOout("Output/MOSSO.txt");
void finalcalculations();

int main(){
	
	return 0;	
}

{	
	starttime = time(NULL);	
	blevel();
	tasklistRS = tasklist;
	calcRS();
	int eft =  round(criticalPath(tasklistRS));
	int energy = round(calcEnergy(tasklistRS));

	RSresults.push_back(eft);
	endtime = time(NULL);
	elapsedtime = endtime - starttime;

	RSout << eft << " " << elapsedtime << " " << energy << "\n";
}

void ABC()
{
	starttime = time(NULL);
	calcABC();
	int eft = round(criticalPath(tasklistABC));
	int energy = round(calcEnergy(tasklistABC));
	ABCresults.push_back(eft);
	endtime = time(NULL);
	elapsedtime = endtime - starttime;
	ABCout << eft << " " << elapsedtime << " " << energy << "\n";
}

void PSO()
{
	starttime = time(NULL);
	calcPSO();
	int eft = round(criticalPath(tasklistPSO));
	int energy = round(calcEnergy(tasklistPSO));
	PSOresults.push_back(eft);
	endtime = time(NULL);
	elapsedtime = endtime - starttime;
	PSOout << eft << " " << elapsedtime << " " << energy << "\n";
}

void SSO()
{
	starttime = time(NULL);
	calcSSO();
	int eft = round(criticalPath(tasklistSSO));
	int energy = round(calcEnergy(tasklistSSO));
	SSOresults.push_back(eft);
	endtime = time(NULL);
	elapsedtime = endtime - starttime;
	SSOout << eft << " " << elapsedtime << " " << energy <<  "\n";
}

void MDPSO()
{
	starttime = time(NULL);
	calcMDPSO();
	int eft = round(criticalPath(tasklistMDPSO));
	int energy = round(calcEnergy(tasklistMDPSO));
	MDPSOresults.push_back(eft);
	endtime = time(NULL);
	elapsedtime = endtime - starttime;
	MDPSOout << eft << " " << elapsedtime << " " << energy << "\n";
}

void MOSSO()
{
	starttime = time(NULL);
	calcMOSSO();
	int eft = round(criticalPath(tasklistMOSSO));
	int energy = round(calcEnergy(tasklistMOSSO));
	MOSSOresults.push_back(eft);
	endtime = time(NULL);
	elapsedtime = endtime - starttime;
	MOSSOout << eft << " " << elapsedtime << " " << energy << "\n";

}