#include "cg_common.h"

static inline cg_token *tnext(cg_env *env) {
	return env->token = cg_token_next(env);
}

static int tcmp(cg_env *env, const char *s) {
	return (env->token->type != TK_INTVAL && env->token->type != TK_DBLVAL) &&
		strcmp(env->token->text, s) == 0;
}

static int tcmpskip(cg_env *env, const char *s) {
	int ret = tcmp(env, s);
	if (ret) tnext(env);
	return ret;
}

static int tcmperr(cg_env *env, const char *s) {
	int ret = tcmp(env, s);
	if (!ret) {
		error(env, "Require %s", s);
	}
	return ret;
}

static void tcmperrskip(cg_env *env, const char *s) {
	if (tcmperr(env, s)) tnext(env);
}

Expression *cg_parse_expression(cg_env *env);
CompoundExpression *cg_parse_compound_expression(cg_env *env);
Expression *cg_parse_expression_internal(cg_env *env, int priority) {
	Expression *ret = nullptr;
	if (priority == 0) {
		if (tcmpskip(env, "plot")) {
			// ...
		} else if (tcmpskip(env, "(")) {
			ret = cg_parse_compound_expression(env);
			tcmperrskip(env, ")");
		} else if (env->token->type == TK_INTVAL || env->token->type == TK_DBLVAL) {
			ValueExpression *val = new ValueExpression(env);
			val->values.push_back(env->token->dblval);
			ret = val;
			tnext(env);
		} else { // 変数、定数

		}
	} else if (cg_token_get_priority(env, env->token, 2) == priority) {
		auto preop = new OperatorExpression(env);
		preop->child[0] = cg_parse_expression_internal(env, priority);
		preop->op = env->token;
		ret = preop;
	} else {
		ret = cg_parse_expression_internal(env, priority - 1);
		if (cg_token_get_priority(env, env->token, 2) == priority) {
			vector<Expression *> exps;
			vector<cg_token *> ops;
			exps.push_back(ret);
			int is_right = cg_token_is_right_association(env, env->token);
			do {
				auto tkn = env->token;
				tnext(env);
				auto exp = cg_parse_expression_internal(env, priority - 1);
				if (is_right) {
					exps.push_back(exp);
					ops.push_back(tkn);
				} else {
					exps.insert(exps.begin(), exp);
					ops.insert(ops.begin(), tkn);
				}
			} while (cg_token_get_priority(env, env->token, 2) == priority);
			while (exps.size() > 1) {
				OperatorExpression *op = new OperatorExpression(env);
				if (is_right) {
					op->child[1] = exps.back(); exps.pop_back();
					op->child[0] = exps.back(); exps.pop_back();
				} else {
					op->child[0] = exps.back(); exps.pop_back();
					op->child[1] = exps.back(); exps.pop_back();
				}
				op->op = ops.back(); ops.pop_back();
				exps.push_back(op);
			}
			ret = exps.back(); exps.pop_back();
		} else {
			while (cg_token_get_priority(env, env->token, 1) == priority) {
				auto exp = new OperatorExpression(env);
				exp->op = env->token;
				tnext(env);
				exp->child[0] = ret;
				ret = exp;
			}
		}
	}
	return ret;
}

Expression *cg_parse_expression(cg_env *env) {
	return cg_parse_expression_internal(env, 11);
}

CompoundExpression *cg_parse_compound_expression(cg_env *env) {
	CompoundExpression *ret = new CompoundExpression(env);
	do {
		ret->expressions.push_back(cg_parse_expression(env));
		if (env->token == NULL) break;
		if (tcmp(env, ")")) break;
	} while (env->token != NULL && !tcmp(env, ")"));
	return ret;
}

Expression *cg_parse_global(cg_env *env) {
	tnext(env);
	return cg_parse_compound_expression(env);
}
