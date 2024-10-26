/*debugger.h*/

#pragma once

#include "programgraph.h"

using namespace std;

class Debugger {

public:
  STMT* Program;

  Debugger(struct STMT* program);

  ~Debugger();

  void run();
};