// C++ header files
#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>
using namespace std;

// C header files
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "cg_def.h"
#include "cg_function.h"

#define assert(e)	if(!(e)) fprintf(stderr, "Assertion error: %s = %d\n", #e, e)
#define error(env,msg,...)	{fprintf(stderr, "Error: " msg "\n",__VA_ARGS__+0);exit(1);}

