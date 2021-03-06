%{
#include "stdafx.h"
#define YYSTYPE stnode::stnode*
#include "parser.h"
int yylex();
void yyerror(const char *);
%}

%token ID NUMBER CHARA STR DELIM DTYPE COMMA SUB_LEFT SUB_RIGHT BRACKET_LEFT BRACKET_RIGHT INC DEC NOT LGNOT DIV MUL MOD ADD SUB SHL SHR BIG BIGEQU LES LESEQU EQU NEQU AND XOR BOR LGAND LGOR ASSIGN MODASS DIVASS MULASS ADDASS SUBASS SHLASS SHRASS ANDASS XORASS BORASS QMARK COLON
%start expr

%%

exp1 : ID
 | NUMBER
 | STR
 | CHARA
 | BRACKET_LEFT exp BRACKET_RIGHT { $$ = $2; }
;
exp2 : exp1
 | exp2 SUB_LEFT exp SUB_RIGHT { $$ = new stnode::op::op(stnode::op::ops::ARRAY_SUB, $1, $3); $$->pos = $2->pos; $$->lineN = $2->lineN; delete $2; delete $4; }
 | ID BRACKET_LEFT BRACKET_RIGHT { $$ = new stnode::call(dynamic_cast<stnode::id*>($1)); $$->pos = $2->pos; $$->lineN = $2->lineN; delete $2; delete $3; }
 | ID BRACKET_LEFT exp BRACKET_RIGHT { $$ = new stnode::call(dynamic_cast<stnode::id*>($1), $3); $$->pos = $2->pos; $$->lineN = $2->lineN; delete $2; delete $4; }
 | exp2 INC { $$ = new stnode::op::op(stnode::op::ops::INC_POST, $1); $$->pos = $2->pos; $$->lineN = $2->lineN; delete $2; }
 | exp2 DEC { $$ = new stnode::op::op(stnode::op::ops::DEC_POST, $1); $$->pos = $2->pos; $$->lineN = $2->lineN; delete $2; }
;
exp3 : exp2
 | INC exp3 { $$ = new stnode::op::op(stnode::op::ops::INC_PRE, $2); $$->pos = $1->pos; $$->lineN = $1->lineN; delete $1; }
 | DEC exp3 { $$ = new stnode::op::op(stnode::op::ops::DEC_PRE, $2); $$->pos = $1->pos; $$->lineN = $1->lineN; delete $1; }
 | AND exp4 { $$ = new stnode::op::op(stnode::op::ops::REF, $2); $$->pos = $1->pos; $$->lineN = $1->lineN; delete $1; }
 | MUL exp4 { $$ = new stnode::op::op(stnode::op::ops::DEREF, $2); $$->pos = $1->pos; $$->lineN = $1->lineN; delete $1; }
 | ADD exp4 { if ($2->getType() == stnode::type::NUMBER) { $$ = $2; } else { $$ = new stnode::op::op(stnode::op::ops::POSI, $2); $$->pos = $1->pos; $$->lineN = $1->lineN; } delete $1; }
 | SUB exp4 { if ($2->getType() == stnode::type::NUMBER) { $$ = new stnode::number(-dynamic_cast<stnode::number*>($2)->val); } else { $$ = new stnode::op::op(stnode::op::ops::NEGA, $2); $$->pos = $1->pos; $$->lineN = $1->lineN; } delete $1; }
 | NOT exp4 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($1); op->arg[0] = $2; $$ = op; }
 | LGNOT exp4 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($1); op->arg[0] = $2; $$ = op; }
;
exp4 : exp3
 | BRACKET_LEFT DTYPE BRACKET_RIGHT exp4 { stnode::cast *cast = dynamic_cast<stnode::cast*>($2); cast->node = $4; $$ = cast; delete $1; delete $3; }
;
exp5 : exp4
 | exp5 MUL exp4 { $$ = new stnode::op::op(stnode::op::ops::MUL, $1, $3); $$->pos = $2->pos; $$->lineN = $2->lineN; delete $2; }
 | exp5 DIV exp4 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp5 MOD exp4 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
;
exp6 : exp5
 | exp6 ADD exp5 { $$ = new stnode::op::op(stnode::op::ops::ADD, $1, $3); $$->pos = $2->pos; $$->lineN = $2->lineN; delete $2; }
 | exp6 SUB exp5 { $$ = new stnode::op::op(stnode::op::ops::SUB, $1, $3); $$->pos = $2->pos; $$->lineN = $2->lineN; delete $2; }
;
exp7 : exp6
 | exp7 SHL exp6 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp7 SHR exp6 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
;
exp8 : exp7
 | exp8 BIG exp7 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp8 BIGEQU exp7 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp8 LES exp7 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp8 LESEQU exp7 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
;
exp9 : exp8
 | exp9 EQU exp8 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp9 NEQU exp8 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
;
exp10 : exp9
 | exp10 AND exp9 { $$ = new stnode::op::op(stnode::op::ops::AND, $1, $3); $$->pos = $2->pos; $$->lineN = $2->lineN; delete $2; }
;
exp11 : exp10
 | exp11 XOR exp10 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
;
exp12 : exp11
 | exp12 BOR exp11 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
;
exp13 : exp12
 | exp13 LGAND exp12 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
;
exp14 : exp13
 | exp14 LGOR exp13 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
;
exp15 : exp14
 | exp14 QMARK exp COLON exp15 { stnode::ifelse *cExp = new stnode::ifelse; cExp->pos = $2->pos; cExp->lineN = $2->lineN; cExp->exp = $1; cExp->blockTrue = new stTree; cExp->blockTrue->push_back($3); cExp->blockFalse = new stTree; cExp->blockFalse->push_back($5); delete $2; delete $4; $$ = cExp; }
;
exp16 : exp15
 | exp3 ASSIGN exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 MODASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 DIVASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 MULASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 ADDASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 SUBASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 SHLASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 SHRASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 ANDASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 XORASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
 | exp3 BORASS exp16 { stnode::op::op *op = dynamic_cast<stnode::op::op*>($2); op->arg[0] = $1; op->arg[1] = $3; $$ = op; }
;
exp : exp16
 | exp COMMA exp16 { $$ = new stnode::expTree($3, $1); $$->pos = $2->pos; delete $2; }
;
expr : exp DELIM { $$ = $1; yacc_result = $1; }

%%

int yylex()
{
	if (yacc_p == yacc_pEnd)
		return -1;
	int ret = -1;
	switch ((*yacc_p)->getType())
	{
		case token::type::ID:
		{
			token::id* tk = static_cast<token::id*>(*yacc_p);
			yylval = new stnode::id(tk->str, dataType());
			ret = ID;
			break;
		}
		case token::type::CHARA:
		{
			token::chara* tk = static_cast<token::chara*>(*yacc_p);
			yylval = new stnode::chara(tk->ch);
			ret = CHARA;
			break;
		}
		case token::type::STR:
		{
			token::str* tk = static_cast<token::str*>(*yacc_p);
			yylval = new stnode::str(tk->strr);
			ret = STR;
			break;
		}
		case token::type::NUMBER:
		{
			token::number* tk = static_cast<token::number*>(*yacc_p);
			yylval = new stnode::number(tk->val);
			ret = NUMBER;
			break;
		}
		case token::type::OP:
		{
			token::op* tk = static_cast<token::op*>(*yacc_p);
			stnode::op::ops newOP;
			switch (tk->opType)
			{
				case token::ops::opType::COMMA:
					ret = COMMA;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::SUB_LEFT:
					ret = SUB_LEFT;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::SUB_RIGHT:
					ret = SUB_RIGHT;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::BRACKET_LEFT:
					ret = BRACKET_LEFT;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::BRACKET_RIGHT:
					ret = BRACKET_RIGHT;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::INC:
					ret = INC;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::DEC:
					ret = DEC;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::NOT:
					ret = NOT;
					newOP = stnode::op::ops::NOT;
					break;
				case token::ops::opType::LGNOT:
					ret = LGNOT;
					newOP = stnode::op::ops::LGNOT;
					break;
				case token::ops::opType::DIV:
					ret = DIV;
					newOP = stnode::op::ops::DIV;
					break;
				case token::ops::opType::MUL:
					ret = MUL;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::MOD:
					ret = MOD;
					newOP = stnode::op::ops::MOD;
					break;
				case token::ops::opType::ADD:
					ret = ADD;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::SUB:
					ret = SUB;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::SHL:
					ret = SHL;
					newOP = stnode::op::ops::SHL;
					break;
				case token::ops::opType::SHR:
					ret = SHR;
					newOP = stnode::op::ops::SHR;
					break;
				case token::ops::opType::BIG:
					ret = BIG;
					newOP = stnode::op::ops::BIG;
					break;
				case token::ops::opType::BIGEQU:
					ret = BIGEQU;
					newOP = stnode::op::ops::BIGEQU;
					break;
				case token::ops::opType::LES:
					ret = LES;
					newOP = stnode::op::ops::LES;
					break;
				case token::ops::opType::LESEQU:
					ret = LESEQU;
					newOP = stnode::op::ops::LESEQU;
					break;
				case token::ops::opType::EQU:
					ret = EQU;
					newOP = stnode::op::ops::EQU;
					break;
				case token::ops::opType::NEQU:
					ret = NEQU;
					newOP = stnode::op::ops::NEQU;
					break;
				case token::ops::opType::AND:
					ret = AND;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::XOR:
					ret = XOR;
					newOP = stnode::op::ops::XOR;
					break;
				case token::ops::opType::BOR:
					ret = BOR;
					newOP = stnode::op::ops::BOR;
					break;
				case token::ops::opType::LGAND:
					ret = LGAND;
					newOP = stnode::op::ops::LGAND;
					break;
				case token::ops::opType::LGOR:
					ret = LGOR;
					newOP = stnode::op::ops::LGOR;
					break;
				case token::ops::opType::ASSIGN:
					ret = ASSIGN;
					newOP = stnode::op::ops::ASSIGN;
					break;
				case token::ops::opType::MODASS:
					ret = MODASS;
					newOP = stnode::op::ops::MODASS;
					break;
				case token::ops::opType::DIVASS:
					ret = DIVASS;
					newOP = stnode::op::ops::DIVASS;
					break;
				case token::ops::opType::MULASS:
					ret = MULASS;
					newOP = stnode::op::ops::MULASS;
					break;
				case token::ops::opType::ADDASS:
					ret = ADDASS;
					newOP = stnode::op::ops::ADDASS;
					break;
				case token::ops::opType::SUBASS:
					ret = SUBASS;
					newOP = stnode::op::ops::SUBASS;
					break;
				case token::ops::opType::SHLASS:
					ret = SHLASS;
					newOP = stnode::op::ops::SHLASS;
					break;
				case token::ops::opType::SHRASS:
					ret = SHRASS;
					newOP = stnode::op::ops::SHRASS;
					break;
				case token::ops::opType::ANDASS:
					ret = ANDASS;
					newOP = stnode::op::ops::ANDASS;
					break;
				case token::ops::opType::XORASS:
					ret = XORASS;
					newOP = stnode::op::ops::XORASS;
					break;
				case token::ops::opType::BORASS:
					ret = BORASS;
					newOP = stnode::op::ops::BORASS;
					break;
				case token::ops::opType::QMARK:
					ret = QMARK;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::COLON:
					ret = COLON;
					newOP = stnode::op::ops::ERROR;
					break;
				default:
					yyerror("Illegal Token(OP)");
					return -1;
			}
			stnode::op::op *opPtr;
			switch (newOP)
			{
				case stnode::op::ops::NOT:
				case stnode::op::ops::LGNOT:
					opPtr = new stnode::op::op(newOP, NULL);
					break;
				case stnode::op::ops::DIV:
				case stnode::op::ops::MOD:
				case stnode::op::ops::SHL:
				case stnode::op::ops::SHR:
				case stnode::op::ops::BIG:
				case stnode::op::ops::BIGEQU:
				case stnode::op::ops::LES:
				case stnode::op::ops::LESEQU:
				case stnode::op::ops::EQU:
				case stnode::op::ops::NEQU:
				case stnode::op::ops::XOR:
				case stnode::op::ops::BOR:
				case stnode::op::ops::LGAND:
				case stnode::op::ops::LGOR:
				case stnode::op::ops::ASSIGN:
				case stnode::op::ops::MODASS:
				case stnode::op::ops::DIVASS:
				case stnode::op::ops::MULASS:
				case stnode::op::ops::ADDASS:
				case stnode::op::ops::SUBASS:
				case stnode::op::ops::SHLASS:
				case stnode::op::ops::SHRASS:
				case stnode::op::ops::ANDASS:
				case stnode::op::ops::XORASS:
				case stnode::op::ops::BORASS:
					opPtr = new stnode::op::op(newOP, NULL, NULL);
					break;
				default:
					opPtr = new stnode::op::op(newOP);
			}
			yylval = opPtr;
			break;
		}
		case token::type::KEYWORD:
		{
			token::keyword* tk = static_cast<token::keyword*>(*yacc_p);
			int ptrLvl = 0;
			yacc_p++;
			while (yacc_p != yacc_pEnd && (*yacc_p)->getType() == token::type::OP && static_cast<token::op *>(*yacc_p)->opType == token::ops::opType::MUL)
			{
				ptrLvl++;
				yacc_p++;
			}
			dataType::type varType;
			switch (tk->word)
			{
				case token::keywords::SINT:
					varType = dataType::SINT;
					break;
				case token::keywords::S8:
					varType = dataType::S8;
					break;
				case token::keywords::S16:
					varType = dataType::S16;
					break;
				case token::keywords::S32:
					varType = dataType::S32;
					break;
				case token::keywords::S64:
					varType = dataType::S64;
					break;
				case token::keywords::UINT:
					varType = dataType::UINT;
					break;
				case token::keywords::U8:
					varType = dataType::U8;
					break;
				case token::keywords::U16:
					varType = dataType::U16;
					break;
				case token::keywords::U32:
					varType = dataType::U32;
					break;
				case token::keywords::U64:
					varType = dataType::U64;
					break;
				case token::keywords::VOID:
					varType = dataType::VOID;
					break;
				default:
					varType = dataType::ERROR;
					yyerror("Illegal Token(Keyword)");
					return -1;
			}
			yylval = new stnode::cast(NULL, dataType(varType, ptrLvl));
			ret = DTYPE;
			break;
		}
		case token::type::DELIM:
			yylval = new stnode::delim;
			ret = DELIM;
	}
	yylval->pos = (*yacc_p)->pos;
	yylval->lineN = yacc_lineN;
	yacc_p++;
	return ret;
}

void yyerror(const char *err)
{
	yacc_err = err;
}
