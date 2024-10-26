/*debugger.cpp*/

//
// A Python debugger that contains a constructor, destructor,
// and a run function that takes a python program and allows user
// to use it to debug with numerous functions
// 
// Helena Yuan
// Northwestern University
// CS 211
// 

#include <iostream>

#include "debugger.h"
#include "ram.h"
#include "execute.h"
#include "programgraph.h"

using namespace std;

//
// constructor
//
Debugger::Debugger(struct STMT* program)
: Program(program)
{}

//
// destructor
//
Debugger::~Debugger()
{}

//
// runs numerous functions on a given python program
//
void Debugger::run()
{
  // initializes all necessary variables
  string cmd;
  string state = "Loaded";
  RAM* memory = ram_init();
  struct STMT* prev = nullptr;
  struct STMT* cur = Program;
  struct ExecuteResult result;

  // runs until user enters "q"
  while (cmd != "q") {
    // prompts user
    cout << "Enter a command, type h for help. Type r to run. > ";
    cin >> cmd;

    if (cmd == "h") { // h: outputs the help screen
      cout << "Available commands:" << endl;
      cout << "r -> Run the program / continue from a breakpoint" << endl;
      cout << "b n -> Breakpoint at line n" << endl;
      cout << "rb n -> Remove breakpoint at line n" << endl;
      cout << "lb -> List all breakpoints" << endl;
      cout << "cb -> Clear all breakpoints" << endl;
      cout << "p varname -> Print variable" << endl;
      cout << "sm -> Show memory contents" << endl;
      cout << "ss -> Show state of debugger" << endl;
      cout << "w -> What line are we on?" << endl;
      cout << "q -> Quit the debugger" << endl;
    }
    else if (cmd == "ss") { // ss: outputs the current state of debugger
      cout << state << endl;
    }
    else if (cmd == "sm") { // sm: outputs the contents of memory
      ram_print(memory);
    }
    else if (cmd == "p") { // p varname: prints information about a variable

      // input variable name
      string varname;
      cin >> varname;
      const char* name = varname.c_str();
      struct RAM_VALUE* value = ram_read_cell_by_name(memory, (char*) name);

      // specific outputs for each type
      if (value == NULL) {
        cout << "no such variable" << endl;
      }
      else if (value->value_type == RAM_TYPE_INT) {
        cout << varname << " (int): " << value->types.i << endl;
      }
      else if (value->value_type == RAM_TYPE_PTR) {
        cout << varname << " (ptr): " << value->types.i << endl;
      }
      else if (value->value_type == RAM_TYPE_BOOLEAN) {
        cout << varname << " (bool): " << value->types.i << endl;
      }
      else if (value->value_type == RAM_TYPE_REAL) {
        cout << varname << " (real): " << value->types.d << endl;
      }
      else if (value->value_type == RAM_TYPE_STR) {
        cout << varname << " (str): " << value->types.s << endl;
      }
      else if (value->value_type == RAM_TYPE_NONE) {
        cout << varname << " (none): None" << endl;
      }

      // free copy of value
      if (value != NULL) {
        ram_free_value(value);
      }
    }
    else if (cmd == "r") { // r: runs the program
      if (state == "Completed") { // program already ran
        cout << "program has completed" << endl;
      }
      else if (state == "Loaded") { // start running the program
        state = "Running";
        result = execute(Program, memory);

        if (prev == nullptr) { // no breakpoints
          state = "Completed";
        }
        else { 
          // if program fails
          if (!result.Success) {
            state = "Completed";
          }
          // fix program
          if (prev->stmt_type == STMT_ASSIGNMENT) {
            prev->types.assignment->next_stmt = cur;
          }
          else if (prev->stmt_type == STMT_FUNCTION_CALL) {
            prev->types.function_call->next_stmt = cur;
          }
          else if (prev->stmt_type == STMT_IF_THEN_ELSE) {
            if (prev->types.if_then_else->condition) {
              prev->types.if_then_else->true_path = cur;
            }
            else {
              prev->types.if_then_else->false_path = cur;
            }
          }
          else if (prev->stmt_type == STMT_WHILE_LOOP) {
            if (prev->types.while_loop->condition) {
              prev->types.while_loop->loop_body = cur;
            }
            else {
              prev->types.while_loop->next_stmt = cur;
            }
          }
          else if (prev->stmt_type == STMT_PASS) {
            prev->types.pass->next_stmt = cur;
          }
          
          cur = Program;
          prev = nullptr;
        }
      }
      else if (state == "Running") {
        // find next part to run
        struct STMT* next;
        if (result.LastStmt->stmt_type == STMT_ASSIGNMENT) {
          next = result.LastStmt->types.assignment->next_stmt;
        }
        else if (result.LastStmt->stmt_type == STMT_FUNCTION_CALL) {
          next = result.LastStmt->types.function_call->next_stmt;
        }
        else if (result.LastStmt->stmt_type == STMT_IF_THEN_ELSE) {
          if (result.LastStmt->types.if_then_else->condition) {
            next = result.LastStmt->types.if_then_else->true_path;
          }
          else {
            next = result.LastStmt->types.if_then_else->false_path;
          }
        }
        else if (result.LastStmt->stmt_type == STMT_WHILE_LOOP) {
          if (result.LastStmt->types.while_loop->condition) {
            next = result.LastStmt->types.while_loop->loop_body;
          }
          else {
            next = result.LastStmt->types.while_loop->next_stmt;
          }
        }
        else if (result.LastStmt->stmt_type == STMT_PASS) {
          next = result.LastStmt->types.pass->next_stmt;
        }

        // run next part
        if (prev == nullptr) { // no breakpoints
          result = execute(next, memory);
          state = "Completed";
        }
        else { // there is a breakpoint
          result = execute(next, memory);

          // if program fails
          if (!result.Success) {
            state = "Completed";
          }

          // fix program
          if (prev->stmt_type == STMT_ASSIGNMENT) {
            prev->types.assignment->next_stmt = cur;
          }
          else if (prev->stmt_type == STMT_FUNCTION_CALL) {
            prev->types.function_call->next_stmt = cur;
          }
          else if (prev->stmt_type == STMT_IF_THEN_ELSE) {
            if (prev->types.if_then_else->condition) {
              prev->types.if_then_else->true_path = cur;
            }
            else {
              prev->types.if_then_else->false_path = cur;
            }
          }
          else if (prev->stmt_type == STMT_WHILE_LOOP) {
            if (prev->types.while_loop->condition) {
              prev->types.while_loop->loop_body = cur;
            }
            else {
              prev->types.while_loop->next_stmt = cur;
            }
          }
          else if (prev->stmt_type == STMT_PASS) {
            prev->types.pass->next_stmt = cur;
          }
          
          cur = Program;
          prev = nullptr;
        }
      }
    }
    else if (cmd == "b") { // b n: sets breakpoint at line n
      // input line number
      int n;
      cin >> n;
      bool found = false;
      
      // breakpoint already set
      if (prev != nullptr) {
        cout << "breakpoint already set" << endl;
        continue;
      }

      // check if line n exists
      while (cur != nullptr) {
        if (cur->line == n) {
          found = true;
          break;
        }

        // traverse the graph
        prev = cur;
        if (cur->stmt_type == STMT_ASSIGNMENT) {
          cur = cur->types.assignment->next_stmt;
        }
        else if (cur->stmt_type == STMT_FUNCTION_CALL) {
          cur = cur->types.function_call->next_stmt;
        }
        else if (cur->stmt_type == STMT_IF_THEN_ELSE) {
          if (cur->types.if_then_else->condition) {
            cur = cur->types.if_then_else->true_path;
          }
          else {
            cur = cur->types.if_then_else->false_path;
          }
        }
        else if (cur->stmt_type == STMT_WHILE_LOOP) {
          if (cur->types.while_loop->condition) {
            cur = cur->types.while_loop->loop_body;
          }
          else {
            cur = cur->types.while_loop->next_stmt;
          }
        }
        else if (cur->stmt_type == STMT_PASS) {
          cur = cur->types.pass->next_stmt;
        }
      }

      
      if (found) { // set up breakpoint if line n is found
        if (prev->stmt_type == STMT_ASSIGNMENT) {
          cur = prev->types.assignment->next_stmt;
          prev->types.assignment->next_stmt = nullptr;
        }
        else if (prev->stmt_type == STMT_FUNCTION_CALL) {
          cur = prev->types.function_call->next_stmt;
          prev->types.function_call->next_stmt = nullptr;
        }
        else if (prev->stmt_type == STMT_IF_THEN_ELSE) {
          if (prev->types.if_then_else->condition) {
            cur = prev->types.if_then_else->true_path;
            prev->types.if_then_else->true_path = nullptr;
          }
          else {
            cur = prev->types.if_then_else->false_path;
            prev->types.if_then_else->false_path = nullptr;
          }
        }
        else if (prev->stmt_type == STMT_WHILE_LOOP) {
          if (prev->types.while_loop->condition) {
            cur = prev->types.while_loop->loop_body;
            prev->types.while_loop->loop_body = nullptr;
          }
          else {
            cur = prev->types.while_loop->next_stmt;
            prev->types.while_loop->next_stmt = nullptr;
          }
        }
        else if (prev->stmt_type == STMT_PASS) {
          cur = prev->types.pass->next_stmt;
          prev->types.pass->next_stmt = nullptr;
        }
      }
      else { // no line n found
        cout << "no such line" << endl;
        prev = nullptr;
        cur = Program;
      }
    }
    else if (cmd == "rb") { // rb: remove breakpoint
      // input line number
      int n;
      cin >> n;

      if (prev == nullptr) { // no breakpoints to remove
        cout << "no such breakpoint" << endl;
      }
      else if (cur->line == n) { // breakpoint found and remove
        if (prev->stmt_type == STMT_ASSIGNMENT) {
          prev->types.assignment->next_stmt = cur;
        }
        else if (prev->stmt_type == STMT_FUNCTION_CALL) {
          prev->types.function_call->next_stmt = cur;
        }
        else if (prev->stmt_type == STMT_IF_THEN_ELSE) {
          if (prev->types.if_then_else->condition) {
            prev->types.if_then_else->true_path = cur;
          }
          else {
            prev->types.if_then_else->false_path = cur;
          }
        }
        else if (prev->stmt_type == STMT_WHILE_LOOP) {
          if (prev->types.while_loop->condition) {
            prev->types.while_loop->loop_body = cur;
          }
          else {
            prev->types.while_loop->next_stmt = cur;
          }
        }
        else if (prev->stmt_type == STMT_PASS) {
          prev->types.pass->next_stmt = cur;
        }

        cur = Program;
        prev = nullptr;

        cout << "breakpoint removed" << endl;
      }
      else { // given n does not match previous breakpoint
        cout << "no such breakpoint" << endl;
      }
    }
    else if (cmd == "cb") { // cb: clear all breakpoints
      if (prev == nullptr) { // no breakpoints
        cout << "breakpoints cleared" << endl;
      }
      else { // clear breakpoints
        if (prev->stmt_type == STMT_ASSIGNMENT) {
          prev->types.assignment->next_stmt = cur;
        }
        else if (prev->stmt_type == STMT_FUNCTION_CALL) {
          prev->types.function_call->next_stmt = cur;
        }
        else if (prev->stmt_type == STMT_IF_THEN_ELSE) {
          if (prev->types.if_then_else->condition) {
            prev->types.if_then_else->true_path = cur;
          }
          else {
            prev->types.if_then_else->false_path = cur;
          }
        }
        else if (prev->stmt_type == STMT_WHILE_LOOP) {
          if (prev->types.while_loop->condition) {
            prev->types.while_loop->loop_body = cur;
          }
          else {
            prev->types.while_loop->next_stmt = cur;
          }
        }
        else if (prev->stmt_type == STMT_PASS) {
          prev->types.pass->next_stmt = cur;
        }

        cur = Program;
        prev = nullptr;

        cout << "breakpoints cleared" << endl;
      }
    }
    else if (cmd == "lb") { // lb: list breakpoints
      if (prev == nullptr) { // no breakpoints to list
        cout << "no breakpoints" << endl;
      }
      else { // prints breakpoint
        cout << "breakpoints on lines: " << cur->line << endl;
      }
    }
    else if (cmd == "w") { // w: gives the next line that will run
      if (state == "Loaded") { // has not started running yet
        cout << "line 1" << endl;
      }
      else if (state == "Running") { // gives next line to run
        // find next line going to run
        struct STMT* next;
        if (result.LastStmt->stmt_type == STMT_ASSIGNMENT) {
          next = result.LastStmt->types.assignment->next_stmt;
        }
        else if (result.LastStmt->stmt_type == STMT_FUNCTION_CALL) {
          next = result.LastStmt->types.function_call->next_stmt;
        }
        else if (result.LastStmt->stmt_type == STMT_IF_THEN_ELSE) {
          if (result.LastStmt->types.if_then_else->condition) {
            next = result.LastStmt->types.if_then_else->true_path;
          }
          else {
            next = result.LastStmt->types.if_then_else->false_path;
          }
        }
        else if (result.LastStmt->stmt_type == STMT_WHILE_LOOP) {
          if (result.LastStmt->types.while_loop->condition) {
            next = result.LastStmt->types.while_loop->loop_body;
          }
          else {
            next = result.LastStmt->types.while_loop->next_stmt;
          }
        }
        else if (result.LastStmt->stmt_type == STMT_PASS) {
          next = result.LastStmt->types.pass->next_stmt;
        }

        cout << "line " << next->line << endl;
      }
      else if (state == "Completed") { // already finished running
        cout << "completed execution" << endl;
      }
    }
    else if (cmd != "q") { // command does not exist
      cout << "unknown command" << endl;
    }
  }
  
  if (prev != nullptr) {
    // fix program
    if (prev->stmt_type == STMT_ASSIGNMENT) {
      prev->types.assignment->next_stmt = cur;
    }
    else if (prev->stmt_type == STMT_FUNCTION_CALL) {
      prev->types.function_call->next_stmt = cur;
    }
    else if (prev->stmt_type == STMT_IF_THEN_ELSE) {
      if (prev->types.if_then_else->condition) {
        prev->types.if_then_else->true_path = cur;
      }
      else {
        prev->types.if_then_else->false_path = cur;
      }
    }
    else if (prev->stmt_type == STMT_WHILE_LOOP) {
      if (prev->types.while_loop->condition) {
        prev->types.while_loop->loop_body = cur;
      }
      else {
        prev->types.while_loop->next_stmt = cur;
      }
    }
    else if (prev->stmt_type == STMT_PASS) {
      prev->types.pass->next_stmt = cur;
    }
    
    cur = Program;
    prev = nullptr;
  }

  // destroy ram
  ram_destroy(memory);
  return;
}