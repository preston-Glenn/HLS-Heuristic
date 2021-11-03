using namespace std;
using namespace boost;

string indexToString(vector<int> indexes){
	string str = "";

	for(int i = 0; i < indexes.size();i++){
    string attr_index = "attr" + int_to_string(i+1);
	  str.append(propertyClass[attr_index].at(indexes.at(i)));
    str.append(propertyLists[attr_index].at(indexes.at(i)));
	}
	return str;
}

int listToHash(vector<int> indexes){
  string str = "";
  for(int i = 0; i < indexes.size();i++){
    str.append(int_to_string(indexes.at(i)));
  }
  return string_to_int(str);
}



vector<string> parseCSVLine(string line){
   std::vector<std::string> vec;

   // Tokenizes the input string
   tokenizer<escaped_list_separator<char> > tk(line, escaped_list_separator<char>
   ('\\', ',', '\"'));
   for (tokenizer<escaped_list_separator<char> >::iterator i = tk.begin();  i != tk.end();  ++i)
     vec.push_back(*i);

   return vec;
}


bool file_exists(string str){
  ifstream file(str.c_str());
  if(!file.is_open()){
    return false;
  }
  file.close();
  return true;
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


string getResultsFromCSV(vector<int> list){
  fstream file;
  file.open((FILE_NAME+".CSV").c_str(),ios::in);
  if(file.is_open()){
    string results = "";
    getline(file,results);
    cout << results << endl;
    getline(file,results);
    cout << "Results: " << results << "space" << endl;
    logger.log("RESULTS: "+results);

    try{
      vector<string> words = parseCSVLine(results);
      LATENCY = string_to_int(words.at(18));
      AREA    = string_to_int(words.at(0 ));
    } catch (const char* message){
		  cout << "Exception occurred!" << message << endl;
      logger.log("Synthesis Failed at: ");
      logger.log("\t"+int_to_string(NUMBER_OF_RUNS));
      logger.log("\t"+indexToString(list));
      AREA = 10000000;
			LATENCY = 10000000;
      return "";
    }
    

    cout << "LATENCY: " << LATENCY << " AREA: " << AREA << endl;		

    return results;

  }
  logger.log("Failed to fetch results from CSV");
  return "Failed";
  exit(1);

}


void addFileResults(string results, vector<int> index_list){
  bool file_opened = false;
  string indexes = "";
  for(int i = 0; i < class_count; i++){
    indexes+=int_to_string(index_list.at(i));
    indexes+="-";
  }
  if(results == "") return;

  ofstream file;
  file.open(RESULTS_DIRECTORY.c_str(),ios::app);
  if(file.is_open()){


    string modified_results = "Exhaustive Search,1," + indexes  + "," + results;
    file << modified_results << endl;
    file.close();

    logger.log("\tAdded :" + results + " to results directory file");
    return;
  }
  logger.log("Failed to open results file");
  return;
}

