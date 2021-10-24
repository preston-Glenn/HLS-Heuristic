/*
 * Filename:            CommandHandler.c
 * Date:                05/07/2021
 * Author:              Preston Glenn
 * Email:               Preston.Glenn@utdallas.edu
 * Version:             1.0
 * Copyright:           2021, All Rights Reserved
 *
 * Description:
 *        This file contains the function in order to call bash commands.
 *
 *
 */


//#include <cstdlib>
//#include <csignal>
#include <iostream>
#include <string>
//#include <stdio.h>
//#include <stdexcept>


#include "loggingHandler.h"

using namespace std;



extern Logger logger;

string commandLine(string instr){
  char buffer[10000];
  string response;

  // Opens a pipe with instr as the command.
  FILE * pipe = popen(instr.c_str(),"r");
  if(!pipe){// If pipe doesn't open correctly log it and exit program
    logger.log("COMMAND HANDLER STATUS: Command failed. Exiting Program");
    if(logger.getVerbose()) cout << endl << "Pipe failed to open. Terminating program." << endl;
    cerr << "TERMINATING PROGRAM" << endl;

    //    exit(1);
  }
  while(!feof(pipe)){
    if(fgets(buffer,1000,pipe)!= NULL)
      response += buffer;

  }
  pclose(pipe);
  return response; // output is the response form the command line

 }
 
