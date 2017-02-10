#include "cg_common.h"

cg_file *cg_file_create(cg_env *env) {
	cg_file *ret = (cg_file *) calloc(1, sizeof(cg_file));
	return ret;
}

cg_file *cg_file_create_from_stdin(cg_env *env) {
	cg_file *ret = cg_file_create(env);
	ret->type = FT_FILE;
	ret->fp = stdin;
	return ret;
}

cg_file *cg_file_create_from_string(cg_env *env, char *value) {
	cg_file *ret = cg_file_create(env);
	ret->type = FT_STRING;
	ret->value = ret->loc = strdup(value);
	return ret;
}

cg_file *cg_file_create_from_filename(cg_env *env, char *fname) {
	cg_file *ret = cg_file_create(env);
	ret->type = FT_FILE;
	ret->fp = fopen(fname, "rb");
	ret->fname = strdup(fname);
	return ret;
}

void cg_file_destroy(cg_env *env, cg_file *file) {
	if (file->fp != NULL && file->fname != NULL)
		fclose(file->fp);
	free(file->fname);	// may null
	free(file->value);	// may null
	free(file);
}

void cg_file_add_top(cg_env *env, cg_file *file) {
	file->next = env->files;
	env->files = file;
}

void cg_file_add_bottom(cg_env *env, cg_file *file) {
	file->next = NULL;
	if (env->files == NULL) env->files = file;
	else {
		cg_file *base = env->files;
		while (base->next != NULL) base = base->next;
		base->next = file;
	}
}

int cg_file_ungetchar(cg_env *env, int c) {
	env->unget[env->unget_loc++] = c;
	assert(env->unget_loc <= 16);
}

static int cg_file_getchar0(cg_env *env) {
	if (env->unget_loc) return env->unget[--env->unget_loc];
	cg_file *file = env->files;
	if (file != NULL) {
		if (file->type == FT_FILE) return fgetc(file->fp);
		if (file->type == FT_STRING) {
			char c = *file->loc++;
			if (c != '\0') return (int) c;
		}
		env->files = file->next;
		cg_file_destroy(env, file);
		return cg_file_getchar(env);
	} else return EOF;
}

int cg_file_getchar(cg_env *env) {
	int ret = cg_file_getchar0(env);
	if (ret == '\\') {
		ret = cg_file_getchar0(env);
		if (ret == '\r' || ret == '\n') {
			do {
				ret = cg_file_getchar(env);
			} while (ret == '\r' || ret == '\n');
		} else {
			cg_file_ungetchar(env, ret);
			ret = '\\';
		}
	}
	return ret;
}


