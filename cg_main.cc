#include "cg_common.h"

int main(void) {
	cg_env env;
	env.files = NULL;
	env.unget_loc = 0;
	cg_file *file = cg_file_create_from_stdin(&env);
	// cg_file *file = cg_file_create_from_string(&env, "plot 1");
	cg_file_add_top(&env, file);
	// cg_token *token;
	// do {
		// token = cg_token_next(&env);
		// cg_token_dump(&env, token);
	cg_execute(&env, cg_parse_global(&env));
	// } while (token != NULL);
}
