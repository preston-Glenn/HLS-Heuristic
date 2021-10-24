// HLS_Heuristic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <fstream>
#include <tclap/CmdLine.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include "loggingHandler.h"
#include "functions.h"

using namespace TCLAP;
using namespace std;

string toString(int &i);
void parseFile(string t);
string synthesize(vector<string> l);
string listToString(vector<string> l);
void bruteForce (vector<string> list ,int count) ;


map <string,vector<string> > propertyLists;     // 2d list of all options
map <string,string> propertyClass;              // list of each attrX type
map <string,bool> attributeMap;                 // If its been called before
int class_count = 0;
Logger logger;
string DIRECTORY = "";

int main(int argc, char** argv) {
	// Wrap everything in a try block.  Do this every time,
	// because exceptions will be thrown for problems.
  logger.setFileName("log.txt");
  logger.log("\n\n\n\n\nNEW EXECUTION:::::::::::");


	try {
		// Define the command line object.
		CmdLine cmd("Command description message", ' ', "0.9");

		SwitchArg heuristic("m","meta-heuristic","Run program as meta-heuristic",cmd,false);
		UnlabeledValueArg<string> configArg("Config-File-Name","The config file name", false,"cs3377dirmond.conf","config file name", false);
		cmd.add(configArg);

		// Parse the args.
		cmd.parse(argc, argv);

		// Get the value parsed by each arg.
		string name = configArg.getValue();
		bool heuristic_value = heuristic.getValue();
	        

		// Do what you intend too...
		logger.log("Started: Parsing");
		parseFile(name);
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

	//logger.log("\tStarted: BF--"+toString(count));

	for (int i = 1; i <= propertyLists[str].size(); i++) {
		vector<string> extendedList;

		for(int k = 0; k < list.size(); k++){
			extendedList.push_back(list.at(k));
		}	
		
		cout << "SIZE" << extendedList.size();
		extendedList.push_back(propertyLists[str].at(i-1));
		cout << extendedList.size();
		for(int k = 0; k < extendedList.size(); k++){
			cout << extendedList.at(k);
		}	
		if (count == class_count) {
		  cout <<"ExList" << count << " " << class_count << endl << extendedList.size() << endl;

			logger.log("Trying to call synthesis");
			try{
				string success = synthesize(extendedList);
				if(true){
					logger.log("Success: ");
					for(int j = 0; j < class_count; j++){
						logger.log("\t"+extendedList.at(j));
					}
					string attributeSring = listToString(extendedList);
					attributeMap[attributeSring] = true;
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
		  bruteForce(list, count + 1);
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
	cout << "COUNT" << class_count << endl;


	ofstream file;
	file.open("attrs.h",ios::trunc);
	if(file.is_open()){
		for(int i = 1; i <= class_count; i++){
			string attr_index = "attr" + toString(i);
			cout << 1 << propertyClass[attr_index] << endl;
			cout << 2 << list[i-1] << endl;
			string str = "#define ATTR"+toString(i) +" Cyber "+propertyClass[attr_index] +"="+list[i-1];
			file << str << endl;
		}
		logger.log("Successfully created attrs.h file.");
	} else{
	  logger.log("Failed to open attrs.h file. Exiting now.");
	  exit(1);
	}
	

	logger.log("Started: BDL_Pars");
	string results = commandLine("bdlpars ../benchmarks/sobel/sobel.c");                         
	string synthesisResults = "";

	logger.log(results);
	logger.log("\n\n\n\n");


	if(results.find("success") != std::string::npos){
		logger.log("Started: BDL_TRAN");
		synthesisResults = commandLine("bdltran -c1000 -s sobel.IFF -lfl /proj/cad/cwb-6.1/packages/asic_45.FLIB -lb /proj/cad/cwb-6.1/packages/asic_45.BLIB");

	
	} 

	logger.log(synthesisResults);

	return results;
}


string listToString(vector<string> list){
	string str = "";
	for(int i = 0; i < list.size();i++){
	  str.append(list.at(i));
	}
}
