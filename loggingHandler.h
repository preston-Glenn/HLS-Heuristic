/*
 * Filename:            Logginghandler.h
 * Date:                05/07/2021
 * Author:              Preston Glenn
 * Email:               Preston.Glenn@utdallas.edu
 * Version:             1.0
 * Copyright:           2021, All Rights Reserved
 *
 * Description:
 *     This is the header file for the Logger class
 *
 */



#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Logger{
 public:
  Logger();
  ~Logger();

  bool log(string message);
  void setFileName(string name);
  string getFileName();
  bool getVerbose();
  void setVerbose(bool t);

private:
  string fileName;
  ofstream file;
  bool verbose;


};
