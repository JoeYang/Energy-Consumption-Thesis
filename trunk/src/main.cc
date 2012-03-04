#define NumberOfAlgorithms	6

#define _RS	0
#define _ABC	1
#define _PSO	2
#define _SSO	3
#define _MDPSO	4
#define _MOSSO	5

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <sstream>
#include <string>
#include <cstring>
#include <bitset>

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



void readInstructions(char*, char*, int*, std::bitset<NumberOfAlgorithms>*);
void RS();
void ABC();
void PSO();
void SSO();
void MDPSO();
void MOSSO();
std::vector<std::string> getdir(std::string dir);
std::vector<int> SSOresults, RSresults, ABCresults, PSOresults, MDPSOresults,MOSSOresults;
std::vector<int> RStime, PSOtime, SSOtime, ABCtime, MDPSOtime;
std::ofstream RSout("result/RS.txt");
std::ofstream ABCout("result/ABC.txt");
std::ofstream PSOout("result/PSO.txt");
std::ofstream SSOout("result/SSO.txt");
std::ofstream MDPSOout("result/MDPSO.txt");
std::ofstream MOSSOout("result/MOSSO.txt");
void finalcalculations();

int main(int argc, char** argv){
	
	if(argc != 4){
		std::cerr<<"Invalid Argument!"<<std::endl;
		return 0;
	}	
	
	std::string testfolder = argv[2];
	std::string resultfolder = argv[3];

	std::bitset<NumberOfAlgorithms> algorithmSelection;
	
	char name[20];
	int numberOfTestCases;
	readInstructions(argv[1], name, &numberOfTestCases, &algorithmSelection);
	
	std::cout<<"Test name: "<<name<<std::endl;
	std::cout<<"Number of Test Cases: "<<numberOfTestCases<<std::endl;
	
	std::vector<std::string> testfiles;
	testfiles = getdir(testfolder);
	int tempincr = round(testfiles.size()/numberOfTestCases);
	
	std::cout<<testfiles.size()<<" "<<tempincr;
	levels.at(0).percentage = 1.0;
	levels.at(1).percentage = 0.8;
	levels.at(2).percentage = 0.7;
	levels.at(3).percentage = 0.4;
	levels.at(0).voltage = 1.75;
	levels.at(1).voltage = 1.4;
	levels.at(2).voltage = 1.2;
	levels.at(3).voltage = 0.9;
	
	srand ( time(NULL) );
	int count  = 0;
	int select = 0;
	for(int o = 0; o+tempincr<testfiles.size(); o+=tempincr)
	{
		select = (int) round(newRand()*tempincr);
		printf("file : %d \n", (o+select));
		if(testfiles.at(o+select)!= "." && testfiles.at(o+select)!= "..")
		{				
				char singlfile[70];
				strcpy(singlfile,testfolder.c_str());	
				strcat(singlfile, testfiles.at(o+select).c_str());
				Data d;
				d = parse(singlfile);
				tasklist = createDag(d);

				if(algorithmSelection[_RS] !=0) RS();
				if(algorithmSelection[_ABC] != 0) ABC();
				if(algorithmSelection[_PSO] !=0) PSO();
				if(algorithmSelection[_SSO] !=0) SSO();
				if(algorithmSelection[_MDPSO] !=0) MDPSO();
				if(algorithmSelection[_MOSSO] !=0) MOSSO();		
		}
	};
	
	//File streams for output data
	RSout.close();
	ABCout.close();
	PSOout.close();
	SSOout.close();
	MDPSOout.close();
	MOSSOout.close();		
	
	return 0;	
}

std::vector<std::string> getdir(std::string dir)
{
	std::vector<std::string> templist;
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(dir.c_str())) == NULL) 
	{
		std::cout << "Error Opening Directory " << dir << std::endl;
		exit(1);
	}
	while (dirp = readdir(dp)) 
	{
		templist.push_back(std::string(dirp->d_name));
	}
	closedir(dp);
	return templist;
}

void readInstructions(char* input, char* name, int* number, std::bitset<NumberOfAlgorithms> *set){
	std::ifstream file;
	file.open(input);
	char output[100];
 	if (file.is_open()) {
	 	file >> output;
	 	file >> output;
	 	strcpy(name, output);
	 	int i = 0;
	 	while (i<6) {
	   		file >> output;		
	   		file >> output;
	   		if(strcmp(output, "yes")==0)
	   			(*set)[i]=1;
	   		else
	   			(*set)[i]=0;
	   		i++;		
	 	}
	 	file >> output;
	 	file >> output;
	 	*number = atoi(output);
	}
	file.close();
}
void RS()
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