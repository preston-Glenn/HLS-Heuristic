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

using namespace TCLAP;
using namespace std;

string toString(int &i);
void parseFile(string t);
string synthesize(vector<string> l);
string listToString(vector<string> l);
void bruteForce (vector<string> list ,int count) ;
bool setHeadersCSV();
string getResultsFromCSV();
bool addFileResults(string r);
string int_to_string(int x);
bool file_exists(string str);

map <string,vector<string> > propertyLists;     // 2d list of all options
map <string,string> propertyClass;              // list of each attrX type
map <string,int> attributeMap;                 // If its been called before
int class_count = 0;
Logger logger;
string DIRECTORY = "";
string FILE_NAME = "";
string FILE_NAME_WITH_EXT = "";
string RESULTS_DIRECTORY = "RESULTS";
string CSV_HEADER = "Method,Iteration,ATTR,AREA,state,FU,REG,MUX,DEC,pin_pair,net,max,min,ave,MISC,MEM,CP_delay,sim,Pmax,Pmin,Pave,Latency,BlockMemoryBit,DSP";
bool heuristic_value;
int NUMBER_OF_RUNS = 0;



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

		SwitchArg heuristic("m","meta-heuristic","Run program as meta-heuristic",cmd,false);
		UnlabeledValueArg<string> file_name_arg("f","The config file name", false,"/benchmarks/sobel/sobel.c","c or bdl file to be run", false);
		cmd.add(file_name_arg);

		// Parse the args.
		cmd.parse(argc, argv);


		// Get the value parsed by each arg.
		
		heuristic_value = heuristic.getValue();
		string file_dir = file_name_arg.getValue();



		FILE_NAME_WITH_EXT = basename(file_dir);


		for(int k = 0; k < FILE_NAME_WITH_EXT.size();k++){
		  char c = FILE_NAME_WITH_EXT[k];
		  if(c == '.') break;
		  FILE_NAME += c;

		}

		int string_size = FILE_NAME_WITH_EXT.size();

		DIRECTORY = "";
		for(int i = 0; i < file_dir.size() - string_size; i++){
		  char ch = file_dir[i];
		  DIRECTORY+=ch;	
		}
		
		string parse_file = "";
		parse_file = DIRECTORY + "lib_" + FILE_NAME + ".info";


		bool headerSuccess = setHeadersCSV();

		logger.log("Directory: "+DIRECTORY);
		logger.log("FILE_W/O E: "+FILE_NAME);
		logger.log("FILE_NameE: "+FILE_NAME_WITH_EXT);
		logger.log("PARSE_FILE: "+parse_file);

		// Do what you intend too...
		logger.log("Started: Parsing");
		parseFile(parse_file);
		logger.log("Finished: Parsing");


		// vector<string> l;
		// for(int i = 1; i <= class_count; i++){
		//   string str = "attr"+toString(i);
		//   cout << i<<propertyLists[str].size()<<endl;
		//   string temp = propertyLists[str].at(0);
		//   cout << temp<<endl;
		//   l.push_back(temp);

		// }
		vector<string> blank_list;

		//		synthesize(l);
		logger.log("Started: Brute Force");
		bruteForce(blank_list,1);
		logger.log("Finished: Brute Force");


	}
	catch (ArgException& e)  // catch any exceptions
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}



}




void bruteForce (vector<string> list, int count = 1) {

	string str = "attr"+toString(count);
	// cout << "first size" <<  propertyLists[str].size() << endl;

	//logger.log("\tStarted: BF--"+toString(count));

	for (int i = 1; i <= propertyLists[str].size(); i++) {
		vector<string> extendedList;

		for(int k = 0; k < list.size(); k++){
			extendedList.push_back(list.at(k));
		}	
		
		// cout <<"I: "<<i<< " SIZE: " << extendedList.size() << " COUNT: " << count << endl;
		extendedList.push_back(propertyLists[str].at(i-1));


		// for(int k = 0; k < extendedList.size(); k++){
		// 	cout << extendedList.at(k) << endl;
		// }	

		if (count == class_count) {

			logger.log("Trying to call synthesis");
			try{
				string success = synthesize(extendedList);
				if(true){
					logger.log("Success: "+toString(++NUMBER_OF_RUNS));
					for(int j = 0; j < class_count; j++){
						logger.log("\t"+extendedList.at(j));
					}
					logger.log("Getting string");
					string attributeString = listToString(extendedList);
					logger.log("Adding to attr. map");
					attributeMap[attributeString] = 1;
					logger.log("Added attribute string to map");

					string results = getResultsFromCSV();
					
					bool addResults = addFileResults(results);

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

	    logger.log("CLASS_COUNT: "+ toString(class_count));

	  } else {
	    cerr << "Attribute File did not open correctly\n";
	  }


	  // while not EOF
	     // read line from file as string, then split string by three parts 

	  /*
	  for(int i = 1; i <= class_count; i++){
	    string str = "attr" + toString(i);
	    cout << propertyClass[str] << endl;

	  }
	  */

// Loging output of parser
	// 	logger.log("\n\n\n");

	//   for(int i = 1; i <= class_count; i++){
	//     string str = "attr" + toString(i);
	//     //	    cout << propertyClass[str] << endl;

	//     for(int j = 0; j < propertyLists[str].size();j++){
	//     //   cout << "\t" << propertyLists[str].at(j) << endl;
	//       logger.log("\t"+propertyLists[str].at(j)+"\t");
	//     }

          
          }

	


	//map<char, int>::iterator it;
	//for(it=first.begin(); it!=first.end(); ++it){
	//	  cout << it->first << " => " << it->second << '\n';
	//}


string toString(int  &i) {
  std::stringstream ss;
  ss << i;
 
  return ss.str();
}


string synthesize(vector<string> list){




  cout << "NUMBER of Runs: " <<  NUMBER_OF_RUNS << endl;

  
	// Check if .csv file exits if so delete it.
	// string csv_file = FILE_NAME + ".CSV";
	// if(file_exists(csv_file)){
	// 	string delete_result = commandLine("rm -f "+csv_file);
	// 	cout << "Deleted: " << delete_result << endl;
	// 	cout << csv_file << endl;
	// }

	ofstream file;
	string fileName = DIRECTORY + "attrs.h";
	file.open(fileName.c_str(),ios::trunc);
	if(file.is_open()){
		for(int i = 1; i <= class_count; i++){
			string attr_index = "attr" + toString(i);
			string str = "#define ATTR"+toString(i) +" Cyber "+propertyClass[attr_index] +"="+list[i-1];
			file << str << endl;
		}
		file.close();
		logger.log("Successfully created attrs.h file.");
	} else{
	  logger.log("Failed to open attrs.h file. Exiting now.");
	  exit(1);
	}
	

	logger.log("Started: BDL_Pars");
	string results = commandLine("bdlpars ../benchmarks/sobel/sobel.c");                         
	string synthesisResults = "";

	logger.log("\n\n\n\n");


	if(results.find("success") != std::string::npos){
		logger.log("Started: BDL_TRAN");
		synthesisResults = commandLine("bdltran -c1000 -s sobel.IFF -lfl /proj/cad/cwb-6.1/packages/asic_45.FLIB -lb /proj/cad/cwb-6.1/packages/asic_45.BLIB > tran.output");

	
	} 

	logger.log(synthesisResults);
	logger.log("Finished BDL_TRAN");


	return results;
}


string listToString(vector<string> list){
	string str = "";
	for(int i = 0; i < list.size();i++){
	  str.append(list.at(i));
	}
	return str;
}


bool addFileResults(string results){
  bool file_opened = false;

  ofstream file;
  file.open(RESULTS_DIRECTORY.c_str(),ios::app);
  if(file.is_open()){

    string modified_results = "Exhaustive Search," + int_to_string(NUMBER_OF_RUNS) + "," + int_to_string(NUMBER_OF_RUNS) + "," + results;
    file << modified_results << endl;
    file.close();

    logger.log("Added :" + results + " to results directory file");
    return true;
  }
  logger.log("Failed to open results file");
  return false;
}



string getResultsFromCSV(){
  fstream file;
  file.open((FILE_NAME+".CSV").c_str(),ios::in);
  if(file.is_open()){
    string results = "";
    getline(file,results);
    getline(file,results);
    cout << "Results: " << results << endl;
    logger.log("RESULTS: "+results);
    return results;

  }
  logger.log("Failed to fetch results from CSV");
  return "Failed";



}


bool setHeadersCSV(){

  ofstream file;
  file.open(RESULTS_DIRECTORY.c_str(),ios::trunc);
  if(file.is_open()){
    file << CSV_HEADER << endl;
    file.close();
    logger.log("Added CSV header");
    return true;
  }


  return false;
}


string int_to_string(int x){
  stringstream ss;
  ss << x;
  string str = ss.str();
  return str;
}


bool file_exists(string str){
  ifstream file(str.c_str());
  if(!file.is_open()){
    return false;
  }
  file.close();
  return true;
}
