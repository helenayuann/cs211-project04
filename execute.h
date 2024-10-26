/*execute.h*/

//
// Executes nuPython program, given as a Program Graph.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
//

#pragma once

#include "programgraph.h"
#include "ram.h"


//
// Public functions:
//

//
// execute_expr
//
// Given an expression such as x * y or a > b, executes
// the expression and returns the result as a RAM_VALUE.
// If the execution fails with a semantic error, an
// error message is output and NULL is returned.
//
// NOTE: this function allocates memory for the value that
// is returned. This implies if the return value != NULL, 
// the caller takes ownership of the copy and must
// eventually free this memory via ram_free_value().
//
struct RAM_VALUE* execute_expr(struct STMT* stmt, struct RAM* memory, struct EXPR* expr);

//
// execute
//
// Given a nuPython program graph and a memory, 
// executes the statements in the program graph.
// If a semantic error occurs (e.g. type error),
// an error message is output, execution stops,
// and the function returns {false, pointer to
// statement where the error occurred}.
//
// If the program executes successfully, {true,
// pointer to last stmt executed} is returned.
//
struct ExecuteResult {
  bool         Success;   // true -> success
  struct STMT* LastStmt;  // ptr to last stmt executed
};

struct ExecuteResult execute(struct STMT* program, struct RAM* memory);
