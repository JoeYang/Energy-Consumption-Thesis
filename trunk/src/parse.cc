

#include "global.h"
#include "data.h"
// Parses the test files and produces Data objects
int procsize;
int end;
std::string::size_type found;
std::string letters = "abcdefghijklmnopqrstuvwxyz_@";
std::string search = "TtAFPveC0123456789";

Data parse(char filename[]){
	Data data;	
	std::ifstream indata;
	std::string line, substr;

	std::vector<int> proc;
	std::string findNext(std::string line);

	indata.open(filename,std::ios_base::in);
	if(!indata) { // file couldn't be opened
     		std::cout << "couldnt open file" << std::endl;
      		exit(1);
   	}

	while(std::getline(indata,line)){

		found = line.find_first_of(search);
		while(found < line.size())
		{

			end = line.find_first_of(" \t",found);
			substr = line.substr(found,end-found);
			if(substr == "TASK")
			{ 
				substr = findNext(line);
				data.tasks.push_back(substr);
			}
			if(substr == "ARC")
			{
				arc a;
				substr = findNext(line);
				substr = findNext(line);

				if(substr == "FROM"){
					substr = findNext(line);
					a.from = substr;					
				}
				substr = findNext(line);
				if(substr == "TO"){
					substr = findNext(line);
					a.to = substr;
				}
				data.arcs.push_back(a);
			}
			if(substr == "PE"){
				std::getline(indata,line);
				std::getline(indata,line);
				proc.clear();
				for(int i = 0; i<data.tasks.size();i++)
				{
					std::getline(indata,line);
					found = line.find_first_of(search);
					end = line.find_first_of(" \t",found);
					substr = findNext(line);
					substr = findNext(line);
					std::istringstream buffer(substr);
					int temp;
					buffer >> temp;
					proc.push_back(temp);
				}
				data.procs.push_back(proc);			
			}
			if(substr == "COMMUN"){
				std::getline(indata,line);
				std::getline(indata,line);
				for(int i = 0; i<data.arcs.size();i++)
				{
					std::getline(indata,line);
					found = line.find_first_of(search);
					end = line.find_first_of(" \t",found);
					substr = findNext(line);
					std::istringstream buffer(substr);
					int temp;
					buffer >> temp;
					data.arcs.at(i).comm = temp;
				}			
			}
			found = line.find_first_of(search,end);
			
		}
	}
	procsize = data.procs.size();
	//for(int i =0; i<data.tasks.size();i++){ std::cout << data.tasks.at(i) << std::endl;}
	//for(int i =0; i<data.arcs.size();i++){ std::cout << data.arcs.at(i).from + " " + data.arcs.at(i).to + " " <<  data.arcs.at(i).comm << std::endl;}
	//for(int i =0; i<data.procs.size();i++){
		//std::cout << "---Proc--- "<< i << std::endl;
		//for(int x = 0; x<data.procs.at(i).size();x++){

		// std::cout << data.procs.at(i).at(x) << std::endl;


		//}
	//}
return data;

}

std::string findNext(std::string line)
{
	found = line.find_first_of(search,end);
	end = line.find_first_of(" \t",found);
	std::string sub = line.substr(found,end-found);
	return sub;
}

