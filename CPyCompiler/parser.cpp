#include "stdafx.h"
#include "parser.h"

#define errPtr ((*p)->pos)
#define nextToken(x) p++; if (p == pEnd) { x p--; return errInfo(lineNumber, errPtr, "Need more token"); }
#define nextTokenD p++
#define prevToken p--

stnode::stnode *yacc_result;
tokenList::iterator yacc_p, yacc_pEnd;
const char *yacc_err;
int yacc_lineN;
int yyparse();

namespace stnode
{
	namespace op
	{
		struct opItem
		{
			ops val;
			std::string str;
		};

		std::string op2Str(ops op)
		{
			static const opItem opMap[] = {
				ops::ERROR, "ERROR",
				ops::COMMA, "COMMA",
				ops::ARRAY_SUB, "ARRAY_SUB",
				ops::CAST, "CAST",
				ops::MEMBER, "MEMBER",
				ops::POSI, "POSI",
				ops::NEGA, "NEGA",
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
	    if (convert)
            return;
		std::list<allocUnit>::iterator p, pEnd = var.end();
		for (p = var.begin(); p != pEnd; p++)
		{
			if (p->init)
			{
				if (p->var->subCount == 0)
					delete *(p->val);
				else
				{
					for (long long i = p->var->subCount - 1; i >= 0; i--)
						delete p->val[i];
					delete[] p->val;
				}
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

errInfo parser_exp(tokenList &tList, stnode::stnode **root, tokenList::iterator &p, int lineNumber)
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
	yacc_lineN = lineNumber;
	yyparse();
	if (yacc_err != NULL)
	{
		if (yacc_p == pEnd)
			yacc_p--;
		return errInfo(lineNumber, (*yacc_p)->pos, yacc_err);
	}
	*root = yacc_result;
	return noErr;
}

errInfo parser_dim(tokenList &tList, stnode::alloc *allocPtr, tokenList::iterator &p, int lineNumber)
{
	tokenList::iterator pEnd = tList.end();
	if ((*p)->getType() == token::type::KEYWORD)
	{
		//type
		token::keyword *type = dynamic_cast<token::keyword *>(*p);
		nextToken(;);
		bool isPtr = false;
		if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == stnode::op::ops::DEREF)
		{
			isPtr = true;
			nextToken(;);
		}
		stnode::varType varType = getVarType(type->word, isPtr);
		if (varType == stnode::varType::_ERROR || varType == stnode::varType::VOID)
			return errInfo(lineNumber, errPtr, "Invalid type");
		stnode::id *newVar = NULL;
		token::id *varName = NULL;
		long long subCount = 1;
		for (; p != pEnd;)
		{
			//name
			int varPos = (*p)->pos;
			varName = dynamic_cast<token::id *>(*p);
			if (varName == NULL)
				return errInfo(lineNumber, errPtr, "Need Variable name");
			nextToken(;);
			subCount = 0;
			if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::SUB_LEFT)
			{
				//array
				nextToken(;);
				if ((*p)->getType() != token::type::NUMBER)
					return errInfo(lineNumber, errPtr, "Number expected");
				subCount = dynamic_cast<token::number *>(*p)->val;
				nextToken(;);
				if ((*p)->getType() != token::type::OP || dynamic_cast<token::op *>(*p)->opType != token::ops::opType::SUB_RIGHT)
					return errInfo(lineNumber, errPtr, "] excepted");
				nextToken(;);
			}
			newVar = new stnode::id(varName->str, varType, subCount);
			newVar->pos = varPos;
			newVar->lineN = lineNumber;
			if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::ASSIGN)
			{
				//init val
				nextToken(delete newVar;);
				if (subCount == 0)
				{
					//init var
					stnode::stnode **initVal = new stnode::stnode*;
					tokenList::iterator pBeg = p;
					for (; p != pEnd; nextTokenD)
						if (((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::COMMA) || (*p)->getType() == token::type::DELIM)
							break;
					p = tList.insert(p, new token::delim(-2));
					assert(pEnd == tList.end());
					errInfo expError = parser_exp(tList, initVal, pBeg, lineNumber);
					p = tList.erase(p);
					if (*initVal == NULL)
					{
						delete newVar; return expError;
					}
					allocPtr->var.push_back(stnode::allocUnit(newVar, initVal));
				}
				else
				{
					//init array
					if ((*p)->getType() != token::type::OP || dynamic_cast<token::op *>(*p)->opType != token::ops::opType::BRACE_LEFT)
					{
						delete newVar;
						return errInfo(lineNumber, errPtr, "{ excepted");
					}
					nextToken(delete newVar;);
					stnode::stnode **initVal = new stnode::stnode*[subCount];
					long long i;
					tokenList::iterator pBeg;
					errInfo expError = noErr;
					for (i = 0; p != pEnd && i < subCount; nextTokenD, i++)
					{
						pBeg = p;
						for (; p != pEnd; nextTokenD)
							if ((*p)->getType() == token::type::OP && (static_cast<token::op *>(*p)->opType == token::ops::opType::COMMA || static_cast<token::op *>(*p)->opType == token::ops::opType::BRACE_RIGHT))
								break;
						p = tList.insert(p, new token::delim(-2));
						assert(pEnd == tList.end());
						expError = parser_exp(tList, initVal + i, pBeg, lineNumber);
						p = tList.erase(p);
						if (initVal[i] == NULL)
						{
							delete newVar; delete[] initVal; return expError;
						}
						if ((*p)->getType() != token::type::OP)
						{
							delete newVar; delete[] initVal; return errInfo(lineNumber, errPtr, "Unexpected token");
						}
						else
						{
							token::ops::opType type = dynamic_cast<token::op *>(*p)->opType;
							if (type == token::ops::opType::BRACE_RIGHT)
							{
								nextToken(delete newVar; delete[] initVal;);
								break;
							}
							else if (i == subCount - 1 && type == token::ops::opType::COMMA)
							{
								delete newVar; delete[] initVal; return errInfo(lineNumber, errPtr, "Unexpected token");
							}
							else if (type != token::ops::opType::COMMA)
							{
								delete newVar; delete[] initVal; return errInfo(lineNumber, errPtr, "} expected");
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
			return errInfo(lineNumber, errPtr, "Need more token");
	}
	else
	{
		return errInfo(lineNumber, errPtr, "Type expected");
	}
	return noErr;
}

struct lvlInfo
{
	lvlInfo(stTree *_sTree, stnode::stnode *_ptr){ sTree = _sTree; ptr = _ptr; }
	stTree *sTree;
	stnode::stnode *ptr;
};

errInfo parser(tokenList &tList, stTree *_sTree)
{
	stnode::stnode *ptr = NULL;
	std::list<lvlInfo>sTreeStk;
	sTreeStk.push_back(lvlInfo(_sTree, new stnode::stnode));
	tokenList::iterator p, pEnd = tList.end();
	bool allowFunc = true;
	int lineNumber = 1;
	for (p = tList.begin(); p != pEnd;)
	{
		token::token *first = *p;
		switch (first->getType())
		{
			case token::type::BEGIN:
				lineNumber = dynamic_cast<token::begin *>(first)->lineN;
				ptr = NULL;
				nextTokenD;
				break;
			case token::type::DELIM:
				ptr = NULL;
				nextTokenD;
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
						allocPtr->pos = first->pos;
						errInfo err = parser_dim(tList, allocPtr, p, lineNumber);
						if (err.err != NULL)
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
						allocPtr->pos = first->pos;
						errInfo err = parser_dim(tList, allocPtr, p, lineNumber);
						if (err.err != NULL)
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
							funcPtr->pos = first->pos;
							nextToken(delete funcPtr;);
							if ((*p)->getType() != token::type::KEYWORD)
							{
								delete funcPtr;
								return errInfo(lineNumber, errPtr, "Type expected");
							}
							token::keyword *type = dynamic_cast<token::keyword *>(*p);
							nextToken(delete funcPtr;);
							bool isPtr = false;
							if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == stnode::op::ops::DEREF)
							{
								isPtr = true;
								nextToken(delete funcPtr;)
							}
							stnode::varType varType = getVarType(type->word, isPtr);
							if (varType == stnode::varType::_ERROR)
							{
								delete funcPtr;
								p--;
								if (isPtr)
									p--;
								return errInfo(lineNumber, errPtr, "Type expected");
							}
							funcPtr->retType = varType;
							if ((*p)->getType() != token::type::ID)
							{
								delete funcPtr;
								return errInfo(lineNumber, errPtr, "Function name expected");
							}
							funcPtr->name = dynamic_cast<token::id *>(*p)->str;
							nextToken(;);
							if ((*p)->getType() != token::type::OP || dynamic_cast<token::op *>(*p)->opType != token::ops::opType::BRACKET_LEFT)
							{
								delete funcPtr;
								return errInfo(lineNumber, errPtr, "( expected");
							}
							nextToken(;);
							if ((*p)->getType() != token::type::KEYWORD)
							{
								delete funcPtr;
								return errInfo(lineNumber, errPtr, "Parameter type expected");
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
									if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == stnode::op::ops::DEREF)
									{
										isPtr = true;
										nextToken(delete funcPtr;)
									}
									varType = getVarType(type->word, isPtr);
									if (varType == stnode::varType::_ERROR || varType == stnode::varType::VOID)
									{
										delete funcPtr;
										return errInfo(lineNumber, errPtr - (isPtr ? 2 : 1), "Parameter type expected");
									}
									if ((*p)->getType() != token::type::ID)
									{
										delete funcPtr;
										return errInfo(lineNumber, errPtr, "Parameter name expected");
									}
									varName = dynamic_cast<token::id *>(*p)->str;
									newVar = new stnode::id(varName, varType);
									newVar->pos = (*p)->pos;
									nextToken(delete funcPtr; delete newVar;)
									funcPtr->args.push_back(newVar);
									if ((*p)->getType() == token::type::DELIM)
									{
										delete funcPtr;
										return errInfo(lineNumber, errPtr, "Need more token");
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
							return errInfo(lineNumber, errPtr, "Can't define function inside function");
						}
						break;
					case token::keywords::keywords::RETURN:
					{
						nextToken(;);
						stnode::ret *retPtr = new stnode::ret;
						retPtr->pos = first->pos;
						if ((*p)->getType() == token::type::DELIM)
						{
							retPtr->retVal = NULL;
							nextTokenD;
						}
						else
						{
							stnode::stnode *exp;
							errInfo err = parser_exp(tList, &exp, p, lineNumber);
							if (err.err != NULL)
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
						errInfo err = parser_exp(tList, &exp, p, lineNumber);
						if (err.err != NULL)
							return err;
						stnode::ifelse *ifPtr = new stnode::ifelse;
						ifPtr->pos = first->pos;
						ifPtr->exp = exp;
						ifPtr->blockTrue = NULL;
						ifPtr->blockFalse = NULL;
						sTreeStk.push_back(lvlInfo(new stTree, ifPtr));
						break;
					}
					case token::keywords::keywords::ELSE:
					{
						nextToken(;);
						lvlInfo &info = sTreeStk.back();
						if (info.ptr->getType() != stnode::type::IF)
							return errInfo(lineNumber, errPtr, "Missing if");
						stnode::ifelse *ifPtr = dynamic_cast<stnode::ifelse *>(info.ptr);
						if (ifPtr->blockTrue != NULL)
                            return errInfo(lineNumber, errPtr, "Too many else");
						ifPtr->blockTrue = info.sTree;
                        info.sTree = new stTree;
						break;
					}
					case token::keywords::keywords::END:
					{
						nextTokenD;
						lvlInfo info = sTreeStk.back();
						sTreeStk.pop_back();
						switch (info.ptr->getType())
						{
							case stnode::type::ERROR:
								return errInfo(lineNumber, errPtr, "Missing code block beginning");
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
						return errInfo(lineNumber, errPtr, "Unexpected token");
				}
				break;
			}
			default:
			{
				stnode::stnode *ptrExp = NULL;
				errInfo err = parser_exp(tList, &ptrExp, p, lineNumber);
				if (err.err != NULL)
					return err;
				ptr = ptrExp;
			}
		}
		if (ptr != NULL)
			sTreeStk.back().sTree->push_back(ptr);
	}
	return noErr;
}
