#include "cg_common.h"

const char * const keywords[] = {
	"int",
	"for",
	"while",
	NULL
};

struct {
	const char * const text;
	int priority[3];	// PRE, POST, BINARY
} symbols[] = {
	{NULL,  { 0,  0,  0}},
	{"++",  { 2,  1,  0}}, {"--",  { 2,  1,  0}}, {"~",   { 2,  0,  0}}, {"!",   { 2,  0,  0}},
	{"*",   { 0,  0,  4}}, {"/",   { 0,  0,  4}}, {"%",   { 0,  0,  4}},
	{"+",   { 2,  0,  5}}, {"-",   { 2,  0,  5}},
	{"<<",  { 0,  0,  6}}, {">>",  { 0,  0,  6}},
	{"<",   { 0,  0,  7}}, {"<=",  { 0,  0,  7}}, {">",   { 0,  0,  7}}, {">=",  { 0,  0,  7}},
	{"==",  { 0,  0,  8}}, {"!=",  { 0,  0,  8}},
	{"&",   { 0,  0,  9}}, {"^",   { 0,  0, 10}}, {"|",   { 0,  0, 11}},
	{"&&",  { 0,  0, 12}}, {"||",  { 0,  0, 13}},
	{"=",   { 0,  0, 15}}, {"+=",  { 0,  0, 15}}, {"-=",  { 0,  0, 15}}, {"*=",  { 0,  0, 15}},
	{"/=",  { 0,  0, 15}}, {"%=",  { 0,  0, 15}}, {"<<=", { 0,  0, 15}}, {">>=", { 0,  0, 15}},
	{"&=",  { 0,  0, 15}}, {"^=",  { 0,  0, 15}}, {"|=",  { 0,  0, 15}},
	{",",   { 0,  0, 16}}, {"(",   { 0,  0,  0}}, {")",   { 0,  0,  0}}, {"{",   { 0,  0,  0}},
	{"}",   { 0,  0,  0}}, {"[",   { 0,  0,  0}}, {"]",   { 0,  0,  0}}, {"?",   { 0,  0,  0}},
	{":",   { 0,  0,  0}}, {";",   { 0,  0,  0}},
	{NULL,  { 0,  0,  0}}
};

static int get(cg_env *env) {
	int ret = cg_file_getchar(env);
	if (ret == '/')	{
		ret = cg_file_getchar(env);
		if (ret == '/') {
			while (ret != EOF && ret != '\n')
				ret = cg_file_getchar(env);
		} else if (ret == '*') {
			while (ret != EOF) {
				ret = cg_file_getchar(env);
				if (ret == '*') {
					ret = cg_file_getchar(env);
					if (ret == '/') break;
				}
			}
			ret = ' ';
		} else {
			cg_file_ungetchar(env, ret);
			ret = '/';
		}
	}
	return ret;
}

cg_token *cg_token_next(cg_env *env) {
	int c = get(env);
	while (c != EOF && isspace(c)) c = get(env);
	if (c == EOF) return NULL;
	cg_token *ret = calloc(1, sizeof(cg_token));
	if ('0' <= c && c <= '9') {
		int base = 10;
		long long intval = 0;
		if (c == '0') {
			c = get(env);
			if      (c == 'x' || c == 'X') base = 16;
			else if (c == 'b' || c == 'B') base = 2;
			else if (c == 'o' || c == 'O') base = 8;
			else {
				cg_file_ungetchar(env, c);
				base = 8;
			}
			c = get(env);
		}
		while (1) {
			int digit;
			if      (base == 16 && 'A' <= c && c <= 'F') digit = c - 'A' + 10;
			else if (base == 16 && 'a' <= c && c <= 'f') digit = c - 'a' + 10;
			else if ('0' <= c && c <= '9') digit = c - '0';
			else if (c != '_') break;
			if (digit >= base) break;
			if (c != '_') intval = intval * base + digit;
			c = get(env);
		}
		ret->type = TK_INTVAL;
		ret->intval = intval;
		if (c == '.') {
			long double dblval = (long double) intval, power = 1.0;
			c = get(env);
			while (1) {
				int digit;
				power /= base;
				if      (base == 16 && 'A' <= c && c <= 'F') digit = c - 'A' + 10;
				else if (base == 16 && 'a' <= c && c <= 'f') digit = c - 'a' + 10;
				else if ('0' <= c && c <= '9') digit = c - '0';
				else if (c != '_') break;
				if (digit >= base) break;
				if (c != '_') dblval += power * digit;
				c = get(env);
			}
			ret->type = TK_DBLVAL;
			ret->dblval = dblval;
		}
		if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')) {
			error(env, "unknown char: %c", (char) c);
		}
	} else {
		char *s = malloc(256), *p = s;
		int i;
		if (c == '"') {
			c = cg_file_getchar(env);
			while (c != EOF && c != '"') {
				if (c == '\\') {
					c = cg_file_getchar(env);
					if      (c == 'a') c = '\a';
					else if (c == 'b') c = '\b';
					else if (c == 'f') c = '\f';
					else if (c == 'n') c = '\n';
					else if (c == 'r') c = '\r';
					else if (c == 'v') c = '\v';
					else if (c == '0') c = '\0';
				}
				*p++ = c;
				c = cg_file_getchar(env);
			}
			*p = '\0';
			if (c != '"') {
				error(env, "require end '\"'");
			} else c = get(env);
			ret->type = TK_STRING;
		} else if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_') {
			do {
				*p++ = (char) c;
				c = get(env);
			} while (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') 
					|| c == '_' || ('0' <= c && c <= '9'));
			*p = '\0';
			for (i = 0; keywords[i] != NULL; i++) {
				if (strcmp(keywords[i], s) == 0) break;
			}
			ret->type = keywords[i] == NULL ? TK_IDENT : TK_KEYWORD;
		} else {
			int symb;
			while (c != EOF) {
				*p = (char) c;
				p[1] = '\0';
				for (i = 1; symbols[i].text != NULL; i++) {
					if (strcmp(symbols[i].text, s) == 0) break;
				}
				if (symbols[i].text == NULL) break;
				symb = i;
				c = get(env);
				p++;
			}
			if (p == s) {
				error(env, "Bad char: %c\n", *s);
			} else {
				ret->type = TK_SYMBOL;
				ret->symbol = symb;
			}
			*p = '\0';
		}
		ret->string = strdup(s);
		free(s);
	}
	cg_file_ungetchar(env, c);
	return ret;
}

void cg_token_dump(cg_env *env, cg_token *token) {
	if (token == NULL) return;
	if (token->type == TK_INTVAL) printf("INT: %ld\n", token->intval);
	if (token->type == TK_DBLVAL) printf("DBL: %llf\n", token->dblval);
	if (token->type == TK_IDENT) printf("IDENT: %s\n", token->string);
	if (token->type == TK_KEYWORD) printf("KEYWORD: %s\n", token->string);
	if (token->type == TK_SYMBOL) printf("SYMBOL: %s\n", token->string);
	if (token->type == TK_STRING) printf("STRING: \"%s\"\n", token->string);
}



