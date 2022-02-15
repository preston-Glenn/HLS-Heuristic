# HLS-Heuristic

## Description

This project allows the user the ability to explore the design space of High Level Synthesis code for the HLS tool Cyberworkbench and find the most optimal trade-off curve using a meta-heursitic. In particular this design space exploration is necesacary because CyberWorkbench allows users to indicate various parameters that impact how the circuit is synthesised. For example, you can control how far you unroll a for loop. 

In this project, instead of indicating a specific parameter for each function, I define keywords such as ATTR1Cyber, ATTR2Cyber,... etc in a attrs.h file, where each keyword can then be used in the code as a argument to control what parameter value each function is modified by. The list of possible values for each keyword is provided by a file OPTION_DEFINED_NAME.ino. See the Benchmarks directory for examples.

This project uses the well known meta-heuristic Genetic Algorithm to evaluate the various designs

## Requirements

The TCLAP library is necessary to compile the program.

## How to Run

The first step is to `cd` into the project directory.

Next you will want to update the Makefile's CPP flag to point to your own /include location for header files. You may also want to change the location of the EXEC file. Currently it places it in a sub-directory /RUNNER/dse.out

Then run `make` in order to compile and link the code. After that you can run the program with `./dse.out`. View the Usage section for help.

Once the program has completed running you will want to open the file in the opensource DSEFrame software to view the optimal trade-off curve.


## Usage:

USAGE: 

   ./dse.out  [-n <Total number of iteration.>] [-l <Results label>] [-f
              <Results file.>] [-s <Program seed>] [-w <AREA weight>] [-r
              <mutation rate>] [-v] [-m] [-e] [--] [--version] [-h] <c or
              bdl file to be run>


Where: 

   -n <Total number of iteration.>,  --number <Total number of iteration.>
     The total number of iterations for meta-heuristic

   -l <Results label>,  --label <Results label>
     The label for the test

   -f <Results file.>,  --results <Results file.>
     The filename for the results output

   -s <Program seed>,  --seed <Program seed>
     The randomness seed for the program.

   -w <AREA weight>,  --weight <AREA weight>
     The weight for the AREA in the cost function. This will overide the
     program from running at multiple different weight values.

   -r <mutation rate>,  --rate <mutation rate>
     The rate at which the allels mutate.

   -v,  --verbose
     The program will be more verbose

   -m,  --meta-heuristic
     Run program as meta-heuristic

   -e,  --exhaustive
     Run exhaustive search

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <c or bdl file to be run>
     The c or bdl input file to be synthesized.
