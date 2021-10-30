// HLS_Heuristic.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <string>
#include <fstream>
#include <tclap/CmdLine.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include "fileSystem.h"
#include "loggingHandler.h"
#include "functions.h"
#include "csv_function.h"

using namespace TCLAP;
using namespace std;

void bruteForce (vector<string> list ,int count) ;
void parseFile(string t);
// bool setHeadersCSV();
// bool addFileResults(string r,string a);
// bool file_exists(string str);
// string int_to_string(int x);
// string getResultsFromCSV();
string synthesize(vector<string> l);
// string listToString(vector<string> l);


map <string,vector<string> > propertyLists;     // 2d list of all options
map <string,string> propertyClass;              // list of each attrX type
map <string,int> attributeMap;                 // If its been called before

int class_count = 0;

Logger logger;

string DIRECTORY = "";		                // Directory path of file
string FILE_NAME = "";			      	// file name with out extension
string FILE_WITH_DIR = "";			// passed in path of file.c
string FILE_NAME_WITH_EXT = ""; 		// filename with extension
string RESULTS_DIRECTORY = "RESULTS";		// file to store results (.csv)
string CSV_HEADER = "Method,Iteration,ATTR,AREA,state,FU,REG,MUX,DEC,pin_pair,net,max,min,ave,MISC,MEM,CP_delay,sim,Pmax,Pmin,Pave,Latency,BlockMemoryBit,DSP";

bool heuristic_value;
bool exhaustive_value;

int NUMBER_OF_RUNS = 0;
int LATENCY;
int AREA;



int main(int argc, char** argv) {
	// Wrap everything in a try block.  Do this every time,
	// because exceptions will be thrown for problems.
	int results_count = 0;
	while(true){
	  RESULTS_DIRECTORY =  "RESULTS_0";
	  string temp = int_to_string(results_count);
	  RESULTS_DIRECTORY += temp;
	  RESULTS_DIRECTORY += ".csv";
	  ifstream file(RESULTS_DIRECTORY.c_str());
	  if(!file.is_open()){
	    file.close();
	    break;
	  }
	   results_count++;
	}
	cout << RESULTS_DIRECTORY << endl;
	logger.setFileName("log.txt");
	logger.log("\n\n\n\n\nNEW EXECUTION:::::::::::");


	try {
		// Define the command line object.
		CmdLine cmd("Command description message", ' ', "0.9");
		SwitchArg exhaustive("e","exhaustive","Run exhaustive search",cmd,false);
		SwitchArg heuristic("m","meta-heuristic","Run program as meta-heuristic",cmd,false);
		UnlabeledValueArg<string> file_name_arg("f","The config file name", false,"/benchmarks/sobel/sobel.c","c or bdl file to be run", false);
		cmd.add(file_name_arg);
		
		// Parse the args.
		cmd.parse(argc, argv);

		// Get the value parsed by each arg.
		exhaustive_value = exhaustive.getValue();
		heuristic_value = heuristic.getValue();
		FILE_WITH_DIR = file_name_arg.getValue();

		FILE_NAME_WITH_EXT = basename(FILE_WITH_DIR);

		// GET FILE_NAME with out the extension or parent path
		for(int k = 0; k < FILE_NAME_WITH_EXT.size();k++){
		  char c = FILE_NAME_WITH_EXT[k];
		  if(c == '.') break;
		  FILE_NAME += c;
		}

		int string_size = FILE_NAME_WITH_EXT.size();

		// Get the DIRECTORY with out the file name
		DIRECTORY = "";
		for(int i = 0; i < FILE_WITH_DIR.size() - string_size; i++){
		  char ch = FILE_WITH_DIR[i];
		  DIRECTORY+=ch;	
		}
		
		string parse_file = DIRECTORY + "lib_" + FILE_NAME + ".info";


		bool headerSuccess = setHeadersCSV();

		logger.log("Directory: "+DIRECTORY);
		logger.log("FILE_W/O E: "+FILE_NAME);
		logger.log("FILE_NameE: "+FILE_NAME_WITH_EXT);
		logger.log("PARSE_FILE: "+parse_file);

		// Do what you intend too...
		logger.log("Started: Parsing");
		parseFile(parse_file);
		logger.log("Finished: Parsing");



		if(exhaustive_value){
		  vector<string> blank_list;

		  logger.log("Started: Brute Force");
		  bruteForce(blank_list,1);

		  logger.log("Finished: Brute Force");
		}

		if(heuristic_value){
		  logger.log("Started Meta Heuristic");
		  meta_heuristic(numberOfRuns);
		  logger.log("Finished Meta Heuristic");


		}
	}
	catch (ArgException& e)  // catch any exceptions
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}



}




void bruteForce (vector<string> list, int count = 1) {

	string str = "attr"+ int_to_string(count);

	for (int i = 1; i <= propertyLists[str].size(); i++) {
		vector<string> extendedList;

		for(int k = 0; k < list.size(); k++){
			extendedList.push_back(list.at(k));
		}	
		
		// cout <<"I: "<<i<< " SIZE: " << extendedList.size() << " COUNT: " << count << endl;
		extendedList.push_back(propertyLists[str].at(i-1));


		if (count == class_count) {

			logger.log("Trying to call synthesis");
			try{
				string success = synthesize(extendedList);
				if(true){
					logger.log("Success: "+ int_to_string(++NUMBER_OF_RUNS));
					for(int j = 0; j < class_count; j++){
						logger.log("\t"+extendedList.at(j));
					}
					logger.log("Getting string");
					string attributeString = listToString(extendedList);
					logger.log("Adding to attr. map");
					attributeMap[attributeString] = 1;
					logger.log("Added attribute string to map");

					string results = getResultsFromCSV();
					
					bool addResults = addFileResults(results, attributeString);

				} else {
					logger.log("FAILED: ");
					for(int j = 0; j < class_count; j++){
						logger.log("\t"+extendedList.at(j));
					}
				}


			} catch (ArgException& e)	{
				cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
			}
		}
		else {
		  bruteForce(extendedList, count + 1);
		}
	}

}


	void parseFile(string file_name){
	  // open file
	  fstream file;
	  file.open(file_name.c_str(),ios::in);
	  string previousString = "";

	  if(file.is_open()){
	    string tp;
	    while(getline(file, tp)){ //read data from file object and put it into string.
	      istringstream ss(tp);
	      string del;
	      
	      if(tp[0] == '#') continue;
	      else if(tp  == "" ) continue;
	      
	      string s1,s2,s3 = "";

	      while(getline(ss, del, ' ')) {		
			s1 = s2;
			s2 = s3;
			s3 = del;
	      }	      
	      propertyLists[s1].push_back(s3);

	      if(previousString != s1){                                 // new attr
			propertyClass[s1] = s2;
			class_count++;
			previousString = s1;
	      }
	    }
	    file.close(); //close the file object.

	    logger.log("CLASS_COUNT: "+ int_to_string(class_count));

	  } else {
	    cerr << "Attribute File did not open correctly\n";
	  }
          
       }


string synthesize(vector<string> list){

  cout << "NUMBER of Runs: " <<  NUMBER_OF_RUNS << endl;

	ofstream file;
	string fileName = DIRECTORY + "attrs.h";
	file.open(fileName.c_str(),ios::trunc);
	if(file.is_open()){
		for(int i = 1; i <= class_count; i++){
			string attr_index = "attr" + int_to_string(i);
			string str = "#define ATTR"+ int_to_string(i) +" Cyber "+propertyClass[attr_index] +"="+list[i-1];
			file << str << endl;
		}
		file.close();
		logger.log("Successfully created attrs.h file.");
	} else{
	  logger.log("Failed to open attrs.h file. Exiting now.");
	  exit(1);
	}
	

	logger.log("Started: BDL_Pars");
	string command = "bdlpars " +  FILE_WITH_DIR;
	logger.log("Called bdlpars with: "+ command);
	
	string results = commandLine(command);                         
	cerr << results << endl;


	string synthesisResults = "";

	logger.log("\n\n\n\n");


	if(results.find("success") != std::string::npos){
		logger.log("Started: BDL_TRAN");
		command = "bdltran -c1000 -s " + FILE_NAME + ".IFF -lfl /proj/cad/cwb-6.1/packages/asic_45.FLIB -lb /proj/cad/cwb-6.1/packages/asic_45.BLIB > tran.output";
		logger.log("Called bdltran with: "+ command);
		synthesisResults = commandLine(command);
	
	} 

	logger.log(synthesisResults);
	logger.log("Finished BDL_TRAN");


	return results;
}





bool meta_heuristic(int numberOfRuns){



}

