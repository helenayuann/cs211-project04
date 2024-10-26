/*main.c*/

//
// Main program for nuPython source-level debugger. The nuPython 
// program to debug can be specified as a command-line argument, 
// like this:
//
//     ./a.out test.py
//
// Or you can just run the debugger and enter the nuPython program
// manually; enter $ to denote the end of the input program. Then 
// you can debug.
// 
// Author: Prof. Joe Hummel
// Northwestern University
// CS 211
//

// to eliminate warnings about stdlib in Visual Studio
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "token.h"    // nuPython header files:
#include "scanner.h" 
#include "parser.h"
#include "programgraph.h" 
#include "ram.h"
#include "execute.h"

#include "debugger.h"

using namespace std;


//
// main
//
// usage: ./a.out [filename.py]
// 
// If a filename is given, the file is opened and serves as
// input to the debugger. If a filename is not given, then 
// input is taken from the keyboard until $ is input.
//
int main(int argc, char* argv[])
{
  FILE* input = NULL;
  bool  keyboardInput = false;

  //
  // where is the input coming from?
  //
  if (argc < 2) {
    //
    // no args, just the program name:
    //
    input = stdin;
    keyboardInput = true;
  }
  else {
    //
    // assume 2nd arg is a nuPython file:
    //
    char* filename = argv[1];

    input = fopen(filename, "r");

    if (input == nullptr) // unable to open:
    {
      cout << "**ERROR: unable to open input file '"
           << filename
           << "' for input." << endl;

      return 0;
    }

    keyboardInput = false;
  }

  if (keyboardInput)  // prompt the user if appropriate:
  {
    cout << "nuPython input (enter $ when you're done)>" << endl;
  }

  //
  // call parser to check program syntax:
  //
  struct TokenQueue* tokens = parser_parse(input);

  if (tokens == nullptr)
  {
    // 
    // program has a syntax error, error msg already output:
    //
    cout << "**parsing failed, exiting..." << endl;
  }
  else
  {
    //
    // we have a valid program in terms of syntax, so let's
    // build the program graph and start debugging:
    //
    cout << "**parsing successful" << endl;
    cout << "**building program graph" << endl;
    cout << endl;

    struct STMT* program = programgraph_build(tokens);

    // programgraph_print(program);

    //
    // now debug the program:
    //
    Debugger debugger(program);
    
    debugger.run();

    //
    // debugger has finished, free data structures:
    //
    programgraph_destroy(program);
    tokenqueue_destroy(tokens);
  }

  //
  // done:
  //
  if (!keyboardInput)
    fclose(input);

  return 0;
}
