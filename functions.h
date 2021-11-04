string commandLine(string a);  



string int_to_string(int x){
  stringstream ss;
  ss << x;
  return ss.str();
}

int string_to_int(string str){
  return atoi(str.c_str());
}

string double_to_string(double x){
  stringstream ss;
  ss << x;
  return ss.str();
}
