/*
 * Filename:            LoggingHandler.c
 * Date:                05/07/2021
 * Author:              Preston Glenn
 * Email:               Preston.Glenn@utdallas.edu
 * Version:             1.0
 * Copyright:           2021, All Rights Reserved
 *
 * Description:
 *     This is the logging class. It stores the file name and then opens and closes it to log a message to the file whenever .log is called.
 *
 */

#include "loggingHandler.h"

Logger::Logger(){
}

Logger::~Logger(){
  file.close();
}


// opens file with filename logs message and then closes the file. Returns true if file was opened correctly.
bool Logger::log(string message){
  file.open(fileName.c_str(),ios::app);
  if(!file.is_open()){
    cerr << "Logging file  failed to open.\nProgram will continue to run without logfile. " << endl;
    return false;
  }
  file << message;
  file.close();
  return true;  
}


void Logger::setFileName(string name){
  fileName = name;
}

string Logger::getFileName(){
  return fileName;
}

bool Logger::getVerbose(){
  return verbose;
}

void Logger::setVerbose(bool t){
  verbose = t;
}
