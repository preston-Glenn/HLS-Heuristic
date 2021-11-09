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

void meta_heuristic(int numberOfRuns);
void bruteForce (vector<int> list ,int count) ;
void parseFile(string t);
void synthesize(vector<int> l);

// bool setHeadersCSV();
// bool addFileResults(string r,string a);
// bool file_exists(string str);
// string int_to_string(int x);
// string getResultsFromCSV();
// string listToString(vector<int> l);


map <string,vector<string> > propertyLists;     // 2d list of all options
map <string,vector<string> > propertyClass;              // list of each attrX type
map <int,int> attributeMap;                 // If its been called before
map <int,int> attributeMap_AREA;                 // If its been called before
map <int,int> attributeMap_LATENCY;                 // If its been called before


int class_count = 0;

Logger logger;

string EXTENSION = "";
string DIRECTORY = "";		                // Directory path of file
string FILE_NAME = "";			      	// file name with out extension
string FILE_WITH_DIR = "";			// passed in path of file.c
string FILE_NAME_WITH_EXT = ""; 		// filename with extension
string RESULTS_DIRECTORY = "RESULTS";		// file to store results (.csv)
string CSV_HEADER = "Method,Iteration,ATTR,AREA,state,FU,REG,MUX,DEC,pin_pair,net,max,min,ave,MISC,MEM,CP_delay,sim,Pmax,Pmin,Pave,Latency,BlockMemoryBit,DSP";
string LABEL;

bool CUSTOMISED_AREA_WEIGHT = false;
bool heuristic_value;
bool exhaustive_value;
bool VERBOSE = false;

int ATTEMPT_FOR_RANDOM_ORGANISM = 50;
int NUMBER_OF_RUNS = 0;
int LATENCY;
int AREA;
float MUTATION_RATE = 0.25;   // ADD TO TCLAP
float AREA_WEIGHT = 0.5;     // ADD TO TCLAP
int SEED = 1;                // ADD TO TCLAP
int NUMBER_OF_ITERATIONS = 0;

#include "csv_function.h"
#include "genetic_functions.h"

int main(int argc, char** argv){
	time_t now, end;
  
	// Wrap everything in a try block.  Do this every time,
	// because exceptions will be thrown for problems.



	try {
		// Define the command line object.
		CmdLine cmd("Command description message", ' ', "0.9");
		SwitchArg exhaustive("e","exhaustive","Run exhaustive search",cmd,false);
		SwitchArg heuristic("m","meta-heuristic","Run program as meta-heuristic",cmd,false);
		SwitchArg verbose("v","verbose","The program will be more verbose",cmd,false);
		UnlabeledValueArg<string> file_name_arg("f","The c or bdl input file to be synthesized.", false,"/benchmarks/sobel/sobel.c","c or bdl file to be run", false);

		ValueArg<float>  mutationRateArg("r","rate","The rate at which the allels mutate.",false,0.1,"mutation rate");
		ValueArg<float>  weightArg("w","weight","The weight for the AREA in the cost function. This will overide the program from running at multiple different weight values.",false,-1,"AREA weight");
		ValueArg<int>    seedArg("s","seed","The randomness seed for the program.",false,1,"Program seed");
		ValueArg<string> resultsFileArg("f","results","The filename for the results output", false,RESULTS_DIRECTORY,"Results file.");
		ValueArg<string> resultLabelArg("l","label", "The label for the test",false,"","Results label");
		ValueArg<int>    numberOfRunArg("n","number", "The total number of iterations for meta-heuristic",false,50,"Total number of iteration.");

		// Add arguments
		cmd.add(mutationRateArg);
		cmd.add(weightArg);
		cmd.add(seedArg);
		cmd.add(file_name_arg);
		cmd.add(resultsFileArg);
		cmd.add(resultLabelArg);
		cmd.add(numberOfRunArg);
		
		// Parse the args.
		cmd.parse(argc, argv);
		
		NUMBER_OF_ITERATIONS = numberOfRunArg.getValue();

		if(resultsFileArg.getValue() == ""){
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

		} else {
		  	RESULTS_DIRECTORY = resultsFileArg.getValue();
			if(!file_exists(RESULTS_DIRECTORY)) setHeadersCSV();
		}


		LABEL = resultLabelArg.getValue();

		string file_name = LABEL+"_log.txt" 
		logger.setFileName(file_name);
		logger.log("\n\n\n\n\nNEW EXECUTION:::::::::::");

	
		logger.log("Seed:" + int_to_string(SEED));

		// Store values
		SEED = seedArg.getValue();
		srand(SEED);
		AREA_WEIGHT = weightArg.getValue();
		if(AREA_WEIGHT != 0){
			CUSTOMISED_AREA_WEIGHT = true;
		}

		VERBOSE = verbose.getValue();
		MUTATION_RATE = mutationRateArg.getValue();

		// Get the value parsed by each arg.
		exhaustive_value = exhaustive.getValue();
		heuristic_value = heuristic.getValue();
		FILE_WITH_DIR = file_name_arg.getValue();

		// Parse filename
		bool dot_found = false;
		FILE_NAME_WITH_EXT = basename(FILE_WITH_DIR);

		// GET FILE_NAME with out the extension or parent path
		for(int k = 0; k < FILE_NAME_WITH_EXT.size();k++){
		  char c = FILE_NAME_WITH_EXT[k];
		  if(c == '.') dot_found = true;
		  if(dot_found) EXTENSION += c;
		  else FILE_NAME += c;
		}

		int string_size = FILE_NAME_WITH_EXT.size();

		// Get the DIRECTORY with out the file name
		DIRECTORY = "";
		for(int i = 0; i < FILE_WITH_DIR.size() - string_size; i++){
		  char ch = FILE_WITH_DIR[i];
		  DIRECTORY+=ch;	
		}
		
		string parse_file = DIRECTORY + "lib_" + FILE_NAME + ".info";


		

		logger.log("Directory: "+DIRECTORY);
		logger.log("FILE_W/O E: "+FILE_NAME);
		logger.log("FILE_NameE: "+FILE_NAME_WITH_EXT);
		logger.log("PARSE_FILE: "+parse_file);

		// Do what you intend too...
		logger.log("Started: Parsing");
		parseFile(parse_file);
		logger.log("Finished: Parsing");


		now = time(NULL);

		// MAIN FUNCTIONALITY
		if(exhaustive_value){
		  vector<int> blank_list;
		  logger.log("Started: Brute Force");
		  bruteForce(blank_list,1);
		  logger.log("Finished: Brute Force");
		}else if(heuristic_value){
			if(AREA_WEIGHT == -1){

				AREA_WEIGHT = 0;
				logger.log("Started Meta Heuristic with area weight at: " + double_to_string(AREA_WEIGHT));
				meta_heuristic(NUMBER_OF_ITERATIONS/6);
				logger.log("Finished first Meta Heuristic");

				AREA_WEIGHT = .25;
				logger.log("Started Meta Heuristic with area weight at: " + double_to_string(AREA_WEIGHT));
				meta_heuristic(2*NUMBER_OF_ITERATIONS/6);
				logger.log("Finished second Meta Heuristic");

				AREA_WEIGHT = .5;
				logger.log("Started Meta Heuristic with area weight at: " + double_to_string(AREA_WEIGHT));
				meta_heuristic(3*NUMBER_OF_ITERATIONS/6);
				logger.log("Finished third Meta Heuristic");

				AREA_WEIGHT = .75;
				logger.log("Started Meta Heuristic with area weight at: " + double_to_string(AREA_WEIGHT));
				meta_heuristic(4*NUMBER_OF_ITERATIONS/6);
				logger.log("Finished fourth Meta Heuristic");

				AREA_WEIGHT = 1;
				logger.log("Started Meta Heuristic with area weight at: " + double_to_string(AREA_WEIGHT));
				meta_heuristic(5*NUMBER_OF_ITERATIONS/6);
				logger.log("Finished fifth Meta Heuristic");

				AREA_WEIGHT = 0;
				logger.log("Started Meta Heuristic with area weight at: " + double_to_string(AREA_WEIGHT));
				meta_heuristic(NUMBER_OF_ITERATIONS);
				logger.log("Finished last Meta Heuristic");
			} else {
				logger.log("Started Meta Heuristic with area weight at: " + double_to_string(AREA_WEIGHT));
				meta_heuristic(NUMBER_OF_ITERATIONS);
				logger.log("Finished Meta Heuristic");
			}
		}
		end = time(NULL);
		double duration = difftime(now,end);
		logger.log("The program labeled: " + LABEL + " took " + double_to_string(duration) + " seconds.");
		cout << "The program labeled: " << LABEL << " took "<< duration << " seconds." << endl;
	}
	catch (ArgException& e)  // catch any exceptions
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}
}

void bruteForce (vector<int> list, int count = 1) {

	string str = "attr"+ int_to_string(count);

	for (int i = 1; i <= propertyLists[str].size(); i++) {
		vector<int> extendedList;

		for(int k = 0; k < list.size(); k++){
			extendedList.push_back(list.at(k));
		}	

		extendedList.push_back(i-1);

		if (count == class_count) {

			logger.log("Trying to call synthesis");
			try{
			  synthesize(extendedList); // if success
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
		propertyClass[s1].push_back(s2);

		if(previousString != s1){                                 // new attr
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

// Convert list to indexes
// mutate and checking map

// Indexes is a list of the sub indexes of each choosen property for a given attribute
void synthesize(vector<int> indexes){
	logger.log("\n\nStarting Synthesis");

	int attributeHash = listToHash(indexes);
    if(attributeMap[attributeHash]){
		logger.log("\tAlready synthesised");
		AREA    = attributeMap_AREA[attributeHash];
		LATENCY = attributeMap_LATENCY[attributeHash];
	} else {
      cout << "NUMBER of attempted synthesized Runs: " <<  ++NUMBER_OF_RUNS << endl;

		ofstream file;
		string fileName = DIRECTORY + "attrs.h";
		file.open(fileName.c_str(),ios::trunc);
		if(file.is_open()){
		  for(int i = 1; i <= class_count; i++){
				string attr_index = "attr" + int_to_string(i);
				string str = "#define ATTR"+ int_to_string(i) +" Cyber "+propertyClass[attr_index].at(indexes.at(i-1)) +"="+propertyLists[attr_index].at(indexes.at(i-1));
			file << str << endl;
			}

			file.close();
			logger.log("\tSuccessfully created attrs.h file.");
		} else{
			logger.log("Failed to open attrs.h file. Exiting now.");
			exit(1);
		}

		string command;

		string results = "";
		if(EXTENSION == ".bdl"){
		  if(VERBOSE)logger.log("\tStarted: BDL_Pars");
		  command = "bdlpars " +  FILE_WITH_DIR;
		  results = commandLine(command);
		  if(VERBOSE) logger.log("\tCalled bdlpars with: "+ command);
		} else if(EXTENSION == ".c"){
		  if(VERBOSE)logger.log("\tStarted: C_Pars");
		  command = "bdlpars " +  FILE_WITH_DIR;
		  results = commandLine(command);
		  if(VERBOSE) logger.log("\tCalled cpars with: "+ command);
		}
		
		
		string synthesisResults = "";


		if(VERBOSE)logger.log("\tStarted: BDL_TRAN");
		command = "bdltran -c1000 -s " + FILE_NAME + ".IFF -lfl /proj/cad/cwb-6.1/packages/asic_45.FLIB -lb /proj/cad/cwb-6.1/packages/asic_45.BLIB > bdltran.output 2> bdltran.errs";
		if(VERBOSE) logger.log("\tCalled bdltran with: "+ command);
		synthesisResults = commandLine(command);
		

		if(VERBOSE) logger.log(synthesisResults);
		if(VERBOSE) logger.log("\tFinished BDL_TRAN");

		results = getResultsFromCSV(indexes);
		if(AREA == 0 && LATENCY == 1){  // dont add results to file
			AREA = 10000000;
			LATENCY = 10000000;
			logger.log("\tRESULTS set to 10000000");
		} else {
			addFileResults(results, indexes);			
		}



		attributeMap[attributeHash] = 1;
		attributeMap_AREA[attributeHash] = AREA;
		attributeMap_LATENCY[attributeHash] = LATENCY;
		logger.log("\tAdded attribute string to attr, AREA, and Latency map.");
		logger.log("Finished Synthesis\n\n");

	}
}





void meta_heuristic(int numberOfRuns){

	genetic_heuristic(numberOfRuns);

}

