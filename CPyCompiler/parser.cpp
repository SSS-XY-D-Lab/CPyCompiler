#include "stdafx.h"
#include "parser.h"

#define nextToken(x) p++; errPtr++; if (p == pEnd) { x return errPtr - 1; }
#define prevToken p--; errPtr--

stnode::stnode *yacc_result;
tokenList::iterator yacc_p, yacc_pEnd;
char *yacc_err;
int yyparse();

namespace stnode
{
	namespace op
	{
		std::string op2Str(ops op)
		{
			static opItem opMap[] = {
				ops::ERROR, "ERROR",
				ops::COMMA, "COMMA",
				ops::SUB_LEFT, "SUB_LEFT",
				ops::SUB_RIGHT, "SUB_RIGHT",
				ops::BRACKET_LEFT, "BRACKET_LEFT",
				ops::BRACKET_RIGHT, "BRACKET_RIGHT",
				ops::ARRAY_SUB, "ARRAY_SUB",
				ops::CAST, "CAST",
				ops::MEMBER, "MEMBER",
				ops::POSI, "POSI",
				ops::NEGA, "NEGA",
				ops::INC, "INC",
				ops::DEC, "DEC",
				ops::INC_POST, "INC_POST",
				ops::DEC_POST, "DEC_POST",
				ops::INC_PRE, "INC_PRE",
				ops::DEC_PRE, "DEC_PRE",
				ops::REF, "REF",
				ops::DEREF, "DEREF",
				ops::NOT, "NOT",
				ops::LGNOT, "LGNOT",
				ops::DIV, "DIV",
				ops::MUL, "MUL",
				ops::MOD, "MOD",
				ops::ADD, "ADD",
				ops::SUB, "SUB",
				ops::SHL, "SHL",
				ops::SHR, "SHR",
				ops::BIG, "BIG",
				ops::BIGEQU, "BIGEQU",
				ops::LES, "LES",
				ops::LESEQU, "LESEQU",
				ops::EQU, "EQU",
				ops::NEQU, "NEQU",
				ops::AND, "AND",
				ops::XOR, "XOR",
				ops::BOR, "BOR",
				ops::LGAND, "LGAND",
				ops::LGOR, "LGOR",
				ops::ASSIGN, "ASSIGN",
				ops::MODASS, "MODASS",
				ops::DIVASS, "DIVASS",
				ops::MULASS, "MULASS",
				ops::ADDASS, "ADDASS",
				ops::SUBASS, "SUBASS",
				ops::SHLASS, "SHLASS",
				ops::SHRASS, "SHRASS",
				ops::ANDASS, "ANDASS",
				ops::XORASS, "XORASS",
				ops::BORASS, "BORASS", };
			for (int i = 0; i < 51; i++)
				if (op == opMap[i].val)
					return opMap[i].str;
			return "";
		}
	}

	alloc::~alloc()
	{
		std::list<allocUnit>::iterator p, pEnd = var.end();
		for (p = var.begin(); p != pEnd; p++)
		{
			if (p->init)
			{
				if (p->var->subCount == 0)
					delete p->val;
				else
					delete[] p->val;
			}
		}
	}
}

stnode::varType getVarType(token::keywords::keywords kw, bool isPtr = false)
{
	stnode::varType varType;
	switch (kw)
	{
		case token::keywords::SINT:
			varType = (isPtr ? stnode::varType::SINT_PTR : stnode::varType::SINT);
			break;
		case token::keywords::S8:
			varType = (isPtr ? stnode::varType::S8_PTR : stnode::varType::S8);
			break;
		case token::keywords::S16:
			varType = (isPtr ? stnode::varType::S16_PTR : stnode::varType::S16);
			break;
		case token::keywords::S32:
			varType = (isPtr ? stnode::varType::S32_PTR : stnode::varType::S32);
			break;
		case token::keywords::S64:
			varType = (isPtr ? stnode::varType::S64_PTR : stnode::varType::S64);
			break;
		case token::keywords::UINT:
			varType = (isPtr ? stnode::varType::UINT_PTR : stnode::varType::UINT);
			break;
		case token::keywords::U8:
			varType = (isPtr ? stnode::varType::U8_PTR : stnode::varType::U8);
			break;
		case token::keywords::U16:
			varType = (isPtr ? stnode::varType::U16_PTR : stnode::varType::U16);
			break;
		case token::keywords::U32:
			varType = (isPtr ? stnode::varType::U32_PTR : stnode::varType::U32);
			break;
		case token::keywords::U64:
			varType = (isPtr ? stnode::varType::U64_PTR : stnode::varType::U64);
			break;
		case token::keywords::VOID:
			varType = (isPtr ? stnode::varType::VOID_PTR : stnode::varType::VOID);
			break;
		default:
			varType = stnode::varType::_ERROR;
	}
	return varType;
}

stnode::op::op *getOp(token::ops::opType op)
{
	stnode::op::ops newOP;
	switch (op)
	{
		case token::ops::opType::MEMBER:
			newOP = stnode::op::ops::MEMBER;
			break;
		case token::ops::opType::POSI:
			newOP = stnode::op::ops::POSI;
			break;
		case token::ops::opType::NEGA:
			newOP = stnode::op::ops::NEGA;
			break;
		case token::ops::opType::INC:
			newOP = stnode::op::ops::INC;
			break;
		case token::ops::opType::DEC:
			newOP = stnode::op::ops::DEC;
			break;
		case token::ops::opType::REF:
			newOP = stnode::op::ops::REF;
			break;
		case token::ops::opType::DEREF:
			newOP = stnode::op::ops::DEREF;
			break;
		case token::ops::opType::NOT:
			newOP = stnode::op::ops::NOT;
			break;
		case token::ops::opType::LGNOT:
			newOP = stnode::op::ops::LGNOT;
			break;
		case token::ops::opType::DIV:
			newOP = stnode::op::ops::DIV;
			break;
		case token::ops::opType::MUL:
			newOP = stnode::op::ops::MUL;
			break;
		case token::ops::opType::MOD:
			newOP = stnode::op::ops::MOD;
			break;
		case token::ops::opType::ADD:
			newOP = stnode::op::ops::ADD;
			break;
		case token::ops::opType::SUB:
			newOP = stnode::op::ops::SUB;
			break;
		case token::ops::opType::SHL:
			newOP = stnode::op::ops::SHL;
			break;
		case token::ops::opType::SHR:
			newOP = stnode::op::ops::SHR;
			break;
		case token::ops::opType::BIG:
			newOP = stnode::op::ops::BIG;
			break;
		case token::ops::opType::BIGEQU:
			newOP = stnode::op::ops::BIGEQU;
			break;
		case token::ops::opType::LES:
			newOP = stnode::op::ops::LES;
			break;
		case token::ops::opType::LESEQU:
			newOP = stnode::op::ops::LESEQU;
			break;
		case token::ops::opType::EQU:
			newOP = stnode::op::ops::EQU;
			break;
		case token::ops::opType::NEQU:
			newOP = stnode::op::ops::NEQU;
			break;
		case token::ops::opType::AND:
			newOP = stnode::op::ops::AND;
			break;
		case token::ops::opType::XOR:
			newOP = stnode::op::ops::XOR;
			break;
		case token::ops::opType::BOR:
			newOP = stnode::op::ops::BOR;
			break;
		case token::ops::opType::LGAND:
			newOP = stnode::op::ops::LGAND;
			break;
		case token::ops::opType::LGOR:
			newOP = stnode::op::ops::LGOR;
			break;
		case token::ops::opType::ASSIGN:
			newOP = stnode::op::ops::ASSIGN;
			break;
		case token::ops::opType::MODASS:
			newOP = stnode::op::ops::MODASS;
			break;
		case token::ops::opType::DIVASS:
			newOP = stnode::op::ops::DIVASS;
			break;
		case token::ops::opType::MULASS:
			newOP = stnode::op::ops::MULASS;
			break;
		case token::ops::opType::ADDASS:
			newOP = stnode::op::ops::ADDASS;
			break;
		case token::ops::opType::SUBASS:
			newOP = stnode::op::ops::SUBASS;
			break;
		case token::ops::opType::SHLASS:
			newOP = stnode::op::ops::SHLASS;
			break;
		case token::ops::opType::SHRASS:
			newOP = stnode::op::ops::SHRASS;
			break;
		case token::ops::opType::ANDASS:
			newOP = stnode::op::ops::ANDASS;
			break;
		case token::ops::opType::XORASS:
			newOP = stnode::op::ops::XORASS;
			break;
		case token::ops::opType::BORASS:
			newOP = stnode::op::ops::BORASS;
			break;
		case token::ops::opType::COMMA:
			newOP = stnode::op::ops::COMMA;
			break;
		case token::ops::opType::SUB_LEFT:
			newOP = stnode::op::ops::SUB_LEFT;
			break;
		case token::ops::opType::SUB_RIGHT:
			newOP = stnode::op::ops::SUB_RIGHT;
			break;
		case token::ops::opType::BRACKET_LEFT:
			newOP = stnode::op::ops::BRACKET_LEFT;
			break;
		case token::ops::opType::BRACKET_RIGHT:
			newOP = stnode::op::ops::BRACKET_RIGHT;
			break;
		default:
			return NULL;
	}
	stnode::op::op *opPtr;
	switch (newOP)
	{
		case stnode::op::ops::INC:
		case stnode::op::ops::DEC:
		case stnode::op::ops::NOT:
		case stnode::op::ops::LGNOT:
		case stnode::op::ops::POSI:
		case stnode::op::ops::NEGA:
		case stnode::op::ops::REF:
		case stnode::op::ops::DEREF:
			opPtr = new stnode::op::opSingle(newOP);
			break;
		case stnode::op::ops::ARRAY_SUB:
		case stnode::op::ops::MEMBER:
		case stnode::op::ops::DIV:
		case stnode::op::ops::MUL:
		case stnode::op::ops::MOD:
		case stnode::op::ops::ADD:
		case stnode::op::ops::SUB:
		case stnode::op::ops::SHL:
		case stnode::op::ops::SHR:
		case stnode::op::ops::BIG:
		case stnode::op::ops::BIGEQU:
		case stnode::op::ops::LES:
		case stnode::op::ops::LESEQU:
		case stnode::op::ops::EQU:
		case stnode::op::ops::NEQU:
		case stnode::op::ops::AND:
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
			opPtr = new stnode::op::opDouble(newOP);
			break;
		default:
			opPtr = new stnode::op::op(newOP);
	}
	return opPtr;
}

stnode::stnode *getNode(token::token *tk)
{
	stnode::stnode *ret = NULL;
	switch (tk->getType())
	{
		case token::type::ID:
		{
			token::id* tok = dynamic_cast<token::id*>(tk);
			ret = new stnode::id(tok->str);
			break;
		}
		case token::type::CHARA:
		{
			token::chara* tok = dynamic_cast<token::chara*>(tk);
			ret = new stnode::chara(tok->ch);
			break;
		}
		case token::type::STR:
		{
			token::str* tok = dynamic_cast<token::str*>(tk);
			ret = new stnode::str(tok->strr);
			break;
		}
		case token::type::OP:
		{
			token::op* tok = dynamic_cast<token::op*>(tk);
			ret = getOp(tok->opType);
			break;
		}
		case token::type::NUMBER:
		{
			token::number* tok = dynamic_cast<token::number*>(tk);
			ret = new stnode::number(tok->val);
			break;
		}
		case token::type::KEYWORD:
		{
			token::keyword* tok = dynamic_cast<token::keyword*>(tk);
			stnode::varType type = getVarType(tok->word);
			if (type == stnode::varType::_ERROR)
				return NULL;
			ret = new stnode::vartype(type);
			break;
		}
	}
	return ret;
}

int parser_exp(tokenList &tList, stnode::stnode **root, tokenList::iterator &p, int &errPtr)
{
	tokenList::iterator pBeg = p, pEnd = tList.end();
	for (; p != pEnd; p++)
		if ((*p)->getType() == token::type::DELIM)
			break;
	p++;
	yacc_result = NULL;
	yacc_err = NULL;
	yacc_p = pBeg;
	yacc_pEnd = p;
	yyparse();
	if (yacc_err != NULL)
		return 0;
	*root = yacc_result;
	return -1;
}

int parser_dim(tokenList &tList, stnode::alloc *allocPtr, tokenList::iterator &p, int &errPtr)
{
	tokenList::iterator pEnd = tList.end();
	if ((*p)->getType() == token::type::KEYWORD)
	{
		//type
		token::keyword *type = dynamic_cast<token::keyword *>(*p);
		nextToken(;);
		bool isPtr = false;
		if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::DEREF)
		{
			isPtr = true;
			nextToken(;);
		}
		stnode::varType varType = getVarType(type->word, isPtr);
		if (varType == stnode::varType::_ERROR || varType == stnode::varType::VOID)
			return errPtr;
		stnode::id *newVar = NULL;
		token::id *varName = NULL;
		long long subCount = 1;
		for (; p != pEnd;)
		{
			//name
			varName = dynamic_cast<token::id *>(*p);
			if (varName == NULL)
				return errPtr;
			nextToken(;);
			subCount = 0;
			if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::SUB_LEFT)
			{
				//array
				nextToken(;);
				if ((*p)->getType() != token::type::NUMBER)
					return errPtr;
				subCount = dynamic_cast<token::number *>(*p)->val;
				nextToken(;);
				if ((*p)->getType() != token::type::OP || dynamic_cast<token::op *>(*p)->opType != token::ops::opType::SUB_RIGHT)
					return errPtr;
				nextToken(;);
			}
			newVar = new stnode::id(varName->str, varType, subCount);
			if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::ASSIGN)
			{
				//init val
				nextToken(delete newVar;);
				if (subCount == 0)
				{
					//init var
					stnode::stnode **initVal = new stnode::stnode*; 
					*initVal = getNode(*p);
					allocPtr->var.push_back(stnode::allocUnit(newVar, initVal));
					nextToken(delete newVar;);
				}
				else
				{
					//init array
					if ((*p)->getType() != token::type::OP || dynamic_cast<token::op *>(*p)->opType != token::ops::opType::BRACE_LEFT)
					{
						delete newVar;
						return errPtr;
					}
					nextToken(delete newVar;);
					stnode::stnode **initVal = new stnode::stnode*[static_cast<unsigned int>(subCount)];
					long long i;
					for (i = 0; p != pEnd && i < subCount; p++, errPtr++, i++)
					{
						initVal[i] = getNode(*p);
						nextToken(delete newVar; delete[] initVal;);
						if ((*p)->getType() != token::type::OP)
						{
							delete newVar; delete[] initVal; return errPtr;
						}
						else
						{
							token::ops::opType type = dynamic_cast<token::op *>(*p)->opType;
							if (type == token::ops::opType::BRACE_RIGHT)
							{
								nextToken(delete newVar; delete[] initVal;);
								break;
							}
							else if (type != token::ops::opType::COMMA)
							{
								delete newVar; delete[] initVal; return errPtr;
							}
						}
					}
					allocPtr->var.push_back(stnode::allocUnit(newVar, initVal));
				}
			}
			else
				allocPtr->var.push_back(stnode::allocUnit(newVar));
			if ((*p)->getType() == token::type::DELIM)
				break;
			else if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::COMMA)
				nextToken(;);
		}
		if (p == pEnd)
			return errPtr;
	}
	else
	{
		return errPtr;
	}
	return -1;
}

struct lvlInfo
{
	lvlInfo(stTree *_sTree, stnode::stnode *_ptr){ sTree = _sTree; ptr = _ptr; }
	stTree *sTree;
	stnode::stnode *ptr;
};

int parser(tokenList &tList, stTree *_sTree)
{
	stnode::stnode *ptr = NULL;
	std::list<lvlInfo>sTreeStk;
	sTreeStk.push_back(lvlInfo(_sTree, new stnode::stnode));
	tokenList::iterator p, pEnd = tList.end();
	int errPtr = 0;
	bool allowFunc = true;
	for (p = tList.begin(); p != pEnd;)
	{
		token::token *first = *p;
		switch (first->getType())
		{
			case token::type::DELIM:
				p++; errPtr++;
				ptr = NULL;
				break;
			case token::type::KEYWORD:
			{
				token::keyword *kw = dynamic_cast<token::keyword *>(first);
				switch (kw->word)
				{
					case token::keywords::keywords::CONST:
					{
						nextToken(;);
						stnode::alloc *allocPtr = new stnode::alloc(true);
						int err = parser_dim(tList, allocPtr, p, errPtr);
						if (err != -1)
						{
							delete allocPtr;
							return err;
						}
						ptr = allocPtr;
						break;
					}
					case token::keywords::keywords::DIM:
					{
						nextToken(;);
						stnode::alloc *allocPtr = new stnode::alloc(false);
						int err = parser_dim(tList, allocPtr, p, errPtr);
						if (err != -1)
						{
							delete allocPtr;
							return err;
						}
						ptr = allocPtr;
						break;
					}
					case token::keywords::keywords::FUNCTION:
						if (allowFunc)
						{
							stnode::func *funcPtr = new stnode::func;
							nextToken(delete funcPtr;);
							if ((*p)->getType() != token::type::KEYWORD)
							{
								delete funcPtr;
								return errPtr;
							}
							token::keyword *type = dynamic_cast<token::keyword *>(*p);
							nextToken(delete funcPtr;);
							bool isPtr = false;
							if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::DEREF)
							{
								isPtr = true;
								nextToken(delete funcPtr;)
							}
							stnode::varType varType = getVarType(type->word, isPtr);
							if (varType == stnode::varType::_ERROR)
							{
								delete funcPtr;
								return errPtr - (isPtr ? 2 : 1);
							}
							funcPtr->retType = varType;
							if ((*p)->getType() != token::type::ID)
							{
								delete funcPtr;
								return errPtr;
							}
							funcPtr->name = dynamic_cast<token::id *>(*p)->str;
							nextToken(;);
							if ((*p)->getType() != token::type::OP || dynamic_cast<token::op *>(*p)->opType != token::ops::opType::BRACKET_LEFT)
							{
								delete funcPtr;
								return errPtr;
							}
							nextToken(;);
							if ((*p)->getType() != token::type::KEYWORD)
							{
								delete funcPtr;
								return errPtr;
							}
							if (dynamic_cast<token::keyword *>(*p)->word != token::keywords::keywords::VOID)
							{
								stnode::id *newVar;
								std::string varName;
								for (; p != pEnd;)
								{
									type = dynamic_cast<token::keyword *>(*p);
									nextToken(delete funcPtr;);
									bool isPtr = false;
									if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::DEREF)
									{
										isPtr = true;
										nextToken(delete funcPtr;)
									}
									varType = getVarType(type->word, isPtr);
									if (varType == stnode::varType::_ERROR || varType == stnode::varType::VOID)
									{
										delete funcPtr;
										return errPtr - (isPtr ? 2 : 1);
									}
									if ((*p)->getType() != token::type::ID)
									{
										delete funcPtr;
										return errPtr;
									}
									varName = dynamic_cast<token::id *>(*p)->str;
									nextToken(delete funcPtr;)
									newVar = new stnode::id(varName, varType);
									funcPtr->args.push_back(newVar);
									if ((*p)->getType() == token::type::DELIM)
									{
										delete funcPtr;
										return errPtr;
									}
									else if ((*p)->getType() == token::type::OP)
									{
										token::ops::opType opType = dynamic_cast<token::op *>(*p)->opType;
										if (opType == token::ops::opType::COMMA)
										{
											nextToken(delete funcPtr;);
										}
										else if (opType == token::ops::opType::BRACKET_RIGHT)
										{
											nextToken(delete funcPtr;);
											break;
										}
									}
								}
							}
							funcPtr->block = NULL;
							stTree *block = new stTree;
							sTreeStk.push_back(lvlInfo(block, funcPtr));
							allowFunc = false;
						}
						else
						{
							return errPtr;
						}
						break;
					case token::keywords::keywords::RETURN:
					{
						nextToken(;);
						stnode::ret *retPtr = new stnode::ret;
						if ((*p)->getType() == token::type::DELIM)
						{
							retPtr->retVal = NULL;
							p++; errPtr++;
						}
						else
						{
							stnode::stnode *exp;
							int err = parser_exp(tList, &exp, p, errPtr);
							if (err != -1)
								return err;
							retPtr->retVal = exp;
						}
						ptr = retPtr;
						break;
					}
					case token::keywords::keywords::IF:
					{
						stnode::stnode *exp;
						nextToken(;);
						int err = parser_exp(tList, &exp, p, errPtr);
						if (err != -1)
							return err;
						stnode::ifelse *ifPtr = new stnode::ifelse;
						ifPtr->exp = exp;
						ifPtr->blockTrue = NULL;
						ifPtr->blockFalse = NULL;
						sTreeStk.push_back(lvlInfo(new stTree, ifPtr));
						break;
					}
					case token::keywords::keywords::ELSE:
					{
						nextToken(;);
						stTree *block = new stTree;
						lvlInfo info = sTreeStk.back();
						if (info.ptr->getType() != stnode::type::IF)
						{
							delete block;
							return errPtr;
						}
						stnode::ifelse *ifPtr = dynamic_cast<stnode::ifelse *>(info.ptr);
						ifPtr->blockTrue = info.sTree;
						break;
					}
					case token::keywords::keywords::END:
					{
						p++; errPtr++;
						lvlInfo info = sTreeStk.back();
						sTreeStk.pop_back();
						switch (info.ptr->getType())
						{
							case stnode::type::ERROR:
								return errPtr;
							case stnode::type::IF:
							{
								stnode::ifelse *ifPtr = dynamic_cast<stnode::ifelse *>(info.ptr);
								if (ifPtr->blockTrue == NULL)
									ifPtr->blockTrue = info.sTree;
								else
									ifPtr->blockFalse = info.sTree;
								ptr = ifPtr;
								break;
							}
							case stnode::type::FUNC:
							{
								stnode::func *funcPtr = dynamic_cast<stnode::func *>(info.ptr);
								funcPtr->block = info.sTree;
								ptr = funcPtr;
								allowFunc = true;
								break;
							}
						}
						break;
					}
					default:
						return errPtr;
				}
				break;
			}
			default:
			{
				stnode::stnode *ptrExp = NULL;
				int err = parser_exp(tList, &ptrExp, p, errPtr);
				if (err != -1)
					return err;
				ptr = ptrExp;
			}
		}
		if (ptr != NULL)
			sTreeStk.back().sTree->push_back(ptr);
	}
	return -1;
}