typedef struct cg_env 		cg_env;
typedef struct cg_file	 	cg_file;
typedef struct cg_token 	cg_token;
typedef struct cg_parse		cg_parse;

enum cg_token_type {
	TK_IDENT, TK_KEYWORD, TK_SYMBOL,
	TK_STRING, TK_INTVAL, TK_DBLVAL
};

typedef enum {
	FT_FILE, FT_STRING
} cg_file_type;

// cg_main.c
struct cg_env {
	cg_file *files;
	int unget[16], unget_loc;
};

// cg_file.c
struct cg_file {
	cg_file_type type;
	char *fname;	// FT_FILE
	FILE *fp;		// FT_FILE
	char *value, *loc;	// FT_STRING
	cg_file *next;
};

// cg_token.c
struct cg_token {
	cg_token_type type;
	char *string;		// TK_STRING, TK_SYMBOL, TK_IDENT, TK_KEYWORD
	long long intval;	// TK_INTVAL, TK_DBLVAL
	long double dblval;	// TK_DBLVAL
	int symbol;			// TK_SYMBOL

};

// cg_graphic.c
#define cg_graphic void

// cg_parse.c
struct cg_parse {
	
};
