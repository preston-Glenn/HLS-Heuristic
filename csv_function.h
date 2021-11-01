using namespace std;
using namespace boost;

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


string getResultsFromCSV(){
  fstream file;
  file.open((FILE_NAME+".CSV").c_str(),ios::in);
  if(file.is_open()){
    string results = "";
    getline(file,results);
    getline(file,results);
    cout << "Results: " << results << endl;
    logger.log("RESULTS: "+results);

	vector<string> words = parseCSVLine(results);

	LATENCY = string_to_int(words.at(18));
	AREA    = string_to_int(words.at(0 ));

	cout << "LATENCY: " << LATENCY << " AREA: " << AREA << endl;		

    return results;

  }
  logger.log("Failed to fetch results from CSV");
  return "Failed";
  exit(1)

}


void addFileResults(string results, string attributeString){
  bool file_opened = false;

  ofstream file;
  file.open(RESULTS_DIRECTORY.c_str(),ios::app);
  if(file.is_open()){

    string modified_results = "Exhaustive Search," + int_to_string(NUMBER_OF_RUNS) + "," + attributeString  + "," + results;
    file << modified_results << endl;
    file.close();

    logger.log("\tAdded :" + results + " to results directory file");
    return;
  }
  logger.log("Failed to open results file");
  return;
}

string listToString(vector<string> list){
	string str = "";
	for(int i = 0; i < list.size();i++){
	  str.append(list.at(i));
	}
	return str;
}
