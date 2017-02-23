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
	cg_token *token;
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
	char *text;		// TK_STRING, TK_SYMBOL, TK_IDENT, TK_KEYWORD
	long long intval;	// TK_INTVAL, TK_DBLVAL
	long double dblval;	// TK_DBLVAL
	int symbol;			// TK_SYMBOL

};

// cg_graphic.c
#define cg_graphic void

// cg_parse.c
class Expression {
public:
	// virtual void dump(int level) = 0;
	virtual void dump(int level) {
		while (level--) cout << "\t";
		cout << "EXPRESSION" << endl;
	}
protected:
	cg_env *env;
};

class CompoundExpression : public Expression {
public:
	CompoundExpression(cg_env *env) {
		this->env = env;
	}
	vector<Expression *> expressions;
	void dump(int level) override {
		int i;
		for (i = 0; i < level; i++) cout << "\t";
		cout << "C(" << endl;
		// for (int i = 0; i < this->expressions.size(); i++) {
		// 	this->expressions[i].dump(level + 1);
		// }
		for (Expression *item: this->expressions) {
			item->dump(level + 1);
		}
		for (i = 0; i < level; i++) cout << "\t";
		cout << ")" << endl;
	}
};

class ValueExpression : public Expression {
public:
	vector<long double> values;
	ValueExpression(cg_env *env) {
		this->env = env;
	}
	void dump(int level) override {
		int i;
		for (i = 0; i < level; i++) cout << "\t";
		cout << "( ";
		for (const auto item: this->values) {
			cout << item << " ";
		}
		cout << ")" << endl;
	}
};

class OperatorExpression : public Expression {
public:
	OperatorExpression(cg_env *env) {
		this->env = env;
		for (int i = 0; i < 3; i++) this->child[i] = nullptr;
	}
	Expression *child[3];
	cg_token *op;
	void dump(int level) override {
		int i;
		for (i = 0; i < level; i++) cout << "\t";
		cout << this->op->text << endl;
		for (i = 0; i < 3; i++) {
			if (this->child[i] != nullptr)
				this->child[i]->dump(level + 1);
		}
	}
};

class VariableExpression : public Expression {
public:
	VariableExpression(cg_env *env) {
		this->env = env;
	}
	const char *text;
	vector<Expression *> args;
	void dump(int level) override {
		for (int i = 0; i < level; i++) cout << "\t";
		cout << this->text;
		if (this->args.size()) {
			cout << " (" << endl;
			for (auto *exp : this->args) {
				exp->dump(level + 1);
			}
			for (int i = 0; i < level; i++) cout << "\t";
			cout << ")";
		}
		cout << endl;
	}
};

class CommandExpression : public Expression {
public:
	CommandExpression(cg_env *env) {
		this->env = env;
	}
	const char *text;
	vector<Expression *> args;
	void dump(int level) override {
		for (int i = 0; i < level; i++) cout << "\t";
		cout << this->text << endl;
		for (auto *exp : this->args) {
			exp->dump(level + 1);
		}
	}
};

