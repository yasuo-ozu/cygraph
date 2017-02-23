#include "cg_common.h"

int main(void) {
	cg_env env;
	env.files = NULL;
	env.unget_loc = 0;
	cg_file *file = cg_file_create_from_stdin(&env);
	// cg_file *file = cg_file_create_from_string(&env, "123+45*6*7-8*9");
	cg_file_add_top(&env, file);
	// cg_token *token;
	// do {
		// token = cg_token_next(&env);
		// cg_token_dump(&env, token);
		cg_parse_global(&env)->dump(0);
	// } while (token != NULL);
}
