
// cg_file.c
cg_file *cg_file_create(cg_env *env);
cg_file *cg_file_create_from_stdin(cg_env *env);
cg_file *cg_file_create_from_string(cg_env *env, char *value);
cg_file *cg_file_create_from_filename(cg_env *env, char *fname);
void cg_file_destroy(cg_env *env, cg_file *file);
void cg_file_add_top(cg_env *env, cg_file *file);
void cg_file_add_bottom(cg_env *env, cg_file *file);
int cg_file_getchar(cg_env *env);
int cg_file_ungetchar(cg_env *env, int c);

// cg_token.c
cg_token *cg_token_next(cg_env *env);
void cg_token_dump(cg_env *env, cg_token *token);
int cg_token_get_priority(cg_env *env, cg_token *token, int type);
int cg_token_is_right_association(cg_env *env, cg_token *token);

// cg_parse.c
Expression *cg_parse_global(cg_env *env);


