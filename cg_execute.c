#include "cg_common.h"
void cg_execute(cg_env *env, Expression *exp) {
	Context ctx();
	exp.evaluate(&ctx);
}
