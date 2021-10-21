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

#include "functions.h"

using namespace TCLAP;
using namespace std;

string toString(int &i);
void parseFile(string t);

map <string,vector<string> > propertyLists;
map <string,string> propertyClass;
map <string,bool> attributeMap;
int class_count = 0;


int main(int argc, char** argv) {
	// Wrap everything in a try block.  Do this every time,
	// because exceptions will be thrown for problems.
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
	        
		cout << heuristic_value << endl;
		// Do what you intend too...
		parseFile(name);
		
	}
	catch (ArgException& e)  // catch any exceptions
	{
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
	}



}




void bruteForce (string list,int length,int count = 0) {

	for (int i = 0; i < length; i++) {

		
		if (count == length) {
			//append last element @[i]
			//synthesis
		  int a = 1;
		}
		else {
		  bruteForce(list,length, count + 1);
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
	       cout << s1 << " " << s2 << " " << s3 << endl;
	      
	      propertyLists[s1].push_back(s3);
	      
	      if(previousString == s1){
		// cout << "PREVIOUS" << s1 << endl;
		
	      } else {                                  // new attr
		propertyClass[s1] = s2;
		class_count++;
		previousString = s1;
	      }
	    }
	    file.close(); //close the file object.

	    cout << "COUNT: " << class_count << endl;

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
	  cout << endl<<endl<<endl;
	  for(int i = 1; i <= class_count; i++){
	    string str = "attr" + toString(i);
	    cout << propertyClass[str] << endl;

	    for(int j = 0; j < propertyLists[str].size();j++){
	      cout << "\t" << propertyLists[str].at(j) << endl;
	    }

          
          }


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


bool synthesize(/*vector<string> list*/){

  string st = commandLine("mkdir .versions");                           

}
