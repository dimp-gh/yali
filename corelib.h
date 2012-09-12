#ifndef CORELIB_H
#define CORELIB_H
#include "sexpr.h"
#include "evaluator.h"


void load_core_library();
void *find_function(char *mention);


#endif
