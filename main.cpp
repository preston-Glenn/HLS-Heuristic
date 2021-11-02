// HLS_Heuristic.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <string>
#include <fstream>
#include <tclap/CmdLine.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <boost/tokenizer.hpp>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "fileSystem.h"
#include "loggingHandler.h"
#include "functions.h"

using namespace TCLAP;
using namespace std;

bool meta_heuristic(int numberOfRuns);
void bruteForce (vector<string> list ,int count) ;
void parseFile(string t);
void synthesize(vector<string> l);

// bool setHeadersCSV();
// bool addFileResults(string r,string a);
// bool file_exists(string str);
// string int_to_string(int x);
// string getResultsFromCSV();
// string listToString(vector<string> l);


map <string,vector<string> > propertyLists;     // 2d list of all options
map <string,string> propertyClass;              // list of each attrX type
map <string,int> attributeMap;                 // If its been called before
map <string,int> attributeMap_AREA;                 // If its been called before
map <string,int> attributeMap_LATENCY;                 // If its been called before



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
bool VERBOSE = false;

int NUMBER_OF_RUNS = 0;
int LATENCY;
int AREA;
float MUTATION_RATE = 0.25;   // ADD TO TCLAP
float AREA_WEIGHT = 0.5;     // ADD TO TCLAP
int SEED = 1;                // ADD TO TCLAP

#include "csv_function.h"
#include "genetic_functions.h"

int main(int argc, char** argv){
  
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

	srand(SEED);
	logger.log("Seed:" + int_to_string(SEED));


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

		// TODO RESET attributeMap

		if(heuristic_value){
		  logger.log("Started Meta Heuristic");
		  int numberOfRuns = 40;
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
		
		extendedList.push_back(propertyLists[str].at(i-1));

		if (count == class_count) {

			logger.log("Trying to call synthesis");
			try{
				synthesize(extendedList);
				if(true){
					logger.log("Success: "+ int_to_string(NUMBER_OF_RUNS));
					for(int j = 0; j < class_count; j++){
						logger.log("\t"+extendedList.at(j));
					}

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


void synthesize(vector<string> list){
	logger.log("\n\n");

	string attributeString = listToString(list);
    if(attributeMap[attributeString]){
      logger.log("Alreaady synthesised");
		AREA    = attributeMap_AREA[attributeString];
        LATENCY = attributeMap_LATENCY[attributeString];

	} else {
      cout << "NUMBER of Synthesized Runs: " <<  ++NUMBER_OF_RUNS << endl;

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
			logger.log("\tSuccessfully created attrs.h file.");
		} else{
			logger.log("Failed to open attrs.h file. Exiting now.");
			exit(1);
		}
		

		logger.log("\tStarted: BDL_Pars");
		string command = "bdlpars " +  FILE_WITH_DIR;
		if(VERBOSE) logger.log("\tCalled bdlpars with: "+ command);
		
		string results = commandLine(command);
		string synthesisResults = "";

		if(results.find("success") != std::string::npos){
			logger.log("\tStarted: BDL_TRAN");
			command = "bdltran -c1000 -s " + FILE_NAME + ".IFF -lfl /proj/cad/cwb-6.1/packages/asic_45.FLIB -lb /proj/cad/cwb-6.1/packages/asic_45.BLIB > tran.output";
			if(VERBOSE) logger.log("\tCalled bdltran with: "+ command);
			synthesisResults = commandLine(command);
		} 

		if(VERBOSE) logger.log(synthesisResults);
		logger.log("\tFinished BDL_TRAN");

		results = getResultsFromCSV();
		if(AREA == 0 && LATENCY == 1) return;

		vector<int> index_list;

		for(int i =1; i <= class_count;i++){
			string str = "attr"+ int_to_string(i);
			for (int j = 0; j < propertyLists[str].size(); j++) {
				if(propertyLists[str].at(j)==list.at(i-1)){
					index_list.push_back(i-1);
					break;
				}
			}

		}

		addFileResults(results, index_list);

		attributeMap[attributeString] = 1;
        attributeMap_AREA[attributeString] = AREA;
        attributeMap_LATENCY[attributeString] = LATENCY;
        logger.log("\tAdded attribute string to attr, AREA, and Latency map.");

	}
}





bool meta_heuristic(int numberOfRuns){

	genetic_heuristic(numberOfRuns);

}

