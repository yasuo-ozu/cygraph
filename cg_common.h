#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cg_def.h"
#include "cg_function.h"

#define assert(e)	if(!(e)) fprintf(stderr, "Assertion error: %s = %d\n", #e, e)
#define error(env,msg,...)	{fprintf(stderr, "Error: " msg "\n",__VA_ARGS__+0);exit(1);}

