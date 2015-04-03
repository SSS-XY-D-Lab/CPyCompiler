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
std::mutex yacc_mutex;

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
				ops::ARRAY_SUB, "ARRAY_SUB",
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

		opType op::getOpType()
		{
			switch (opVal)
			{
				case ops::ARRAY_SUB:
				case ops::REF:
				case ops::DEREF:
					return opType::POINTER;
				case ops::POSI:
				case ops::NEGA:
				case ops::INC_POST:
				case ops::DEC_POST:
				case ops::INC_PRE:
				case ops::DEC_PRE:
				case ops::NOT:
				case ops::DIV:
				case ops::MUL:
				case ops::MOD:
				case ops::ADD:
				case ops::SUB:
				case ops::SHL:
				case ops::SHR:
				case ops::AND:
				case ops::XOR:
				case ops::BOR:
					return opType::ARITHMETIC;
				case ops::LGNOT:
				case ops::LGAND:
				case ops::LGOR:
				case ops::BIG:
				case ops::BIGEQU:
				case ops::LES:
				case ops::LESEQU:
				case ops::EQU:
				case ops::NEQU:
					return opType::LOGICAL;
				case ops::ASSIGN:
				case ops::MODASS:
				case ops::DIVASS:
				case ops::MULASS:
				case ops::ADDASS:
				case ops::SUBASS:
				case ops::SHLASS:
				case ops::SHRASS:
				case ops::ANDASS:
				case ops::XORASS:
				case ops::BORASS:
					return opType::ASSIGNMENT;
				default:
					return opType::OTHER;
			}
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
				if (p->subCount == 0)
					delete *(p->val);
				else
				{
					for (long long i = p->subCount - 1; i >= 0; i--)
						delete p->val[i];
					delete[] p->val;
				}
			}
		}
	}
}

dataType getVarType(token::keywords::keywords kw, int ptrLvl)
{
	dataType::type varType;
	switch (kw)
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
	}
	return dataType(varType, ptrLvl);
}

errInfo parser_exp(tokenList &tList, stnode::stnode **root, tokenList::iterator &p, int lineNumber)
{
	tokenList::iterator pBeg = p, pEnd = tList.end();
	for (; p != pEnd; p++)
		if ((*p)->getType() == token::type::DELIM)
			break;
	p++;
	yacc_mutex.lock();
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
		yacc_mutex.unlock();
		return errInfo(lineNumber, (*yacc_p)->pos, yacc_err);
	}
	*root = yacc_result;
	yacc_mutex.unlock();
	return noErr;
}

errInfo parser_dim(tokenList &tList, stnode::alloc *allocPtr, tokenList::iterator &p, int lineNumber)
{
	tokenList::iterator pEnd = tList.end();
	if ((*p)->getType() == token::type::KEYWORD)
	{
		//type
		token::keyword *type = static_cast<token::keyword *>(*p);
		nextToken(;);
		int ptrLvl = 0;
		while ((*p)->getType() == token::type::OP && static_cast<token::op *>(*p)->opType == token::ops::opType::MUL)
		{
			ptrLvl++;
			nextToken(;);
		}
		dataType varType = getVarType(type->word, ptrLvl);
		if (varType.dType == dataType::ERROR || (varType.dType == dataType::VOID && varType.ptrLvl == 0))
		{
			prevToken;
			while (ptrLvl)
			{
				ptrLvl--;
				prevToken;
			}
			return errInfo(lineNumber, errPtr, "Invalid type");
		}
		stnode::id *newVar = NULL;
		token::id *varName = NULL;
		size_t subCount = 0;
		for (; p != pEnd;)
		{
			//name
			int varPos = (*p)->pos;
			if ((*p)->getType() != token::type::ID)
				return errInfo(lineNumber, errPtr, "Need Variable name");
			varName = static_cast<token::id *>(*p);
			nextToken(;);
			subCount = 0;
			if ((*p)->getType() == token::type::OP && static_cast<token::op *>(*p)->opType == token::ops::opType::SUB_LEFT)
			{
				//array
				nextToken(;);
				if ((*p)->getType() != token::type::NUMBER)
					return errInfo(lineNumber, errPtr, "Number expected");
				subCount = static_cast<size_t>(static_cast<token::number *>(*p)->val);
				nextToken(;);
				if ((*p)->getType() != token::type::OP || static_cast<token::op *>(*p)->opType != token::ops::opType::SUB_RIGHT)
					return errInfo(lineNumber, errPtr, "] excepted");
				nextToken(;);
			}
			newVar = new stnode::id(varName->str, varType);
			newVar->lineN = lineNumber;
			newVar->pos = varPos;
			if ((*p)->getType() == token::type::OP && static_cast<token::op *>(*p)->opType == token::ops::opType::ASSIGN)
			{
				//init val
				nextToken(delete newVar;);
				if (subCount == 0)
				{
					//init var
					stnode::stnode **initVal = new stnode::stnode*;
					tokenList::iterator pBeg = p;
					for (; p != pEnd; nextTokenD)
						if (((*p)->getType() == token::type::OP && static_cast<token::op *>(*p)->opType == token::ops::opType::COMMA) || (*p)->getType() == token::type::DELIM)
							break;
					p = tList.insert(p, new token::delim(-2));
					assert(pEnd == tList.end());
					errInfo expError = parser_exp(tList, initVal, pBeg, lineNumber);
					p = tList.erase(p);
					if (*initVal == NULL)
					{
						delete newVar; return expError;
					}
					allocPtr->var.push_back(stnode::allocUnit(newVar, initVal, 0));
				}
				else
				{
					//init array
					if ((*p)->getType() != token::type::OP || static_cast<token::op *>(*p)->opType != token::ops::opType::BRACE_LEFT)
					{
						delete newVar;
						return errInfo(lineNumber, errPtr, "{ excepted");
					}
					nextToken(delete newVar;);
					stnode::stnode **initVal = new stnode::stnode*[subCount];
					size_t i;
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
							token::ops::opType type = static_cast<token::op *>(*p)->opType;
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
					for (; i < subCount; i++)
						initVal[i] = NULL;
					allocPtr->var.push_back(stnode::allocUnit(newVar, initVal, subCount));
				}
			}
			else
				allocPtr->var.push_back(stnode::allocUnit(newVar, subCount));
			if ((*p)->getType() == token::type::DELIM)
				break;
			else if ((*p)->getType() == token::type::OP && static_cast<token::op *>(*p)->opType == token::ops::opType::COMMA)
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
	std::list<lvlInfo> sTreeStk;
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
				lineNumber = static_cast<token::begin *>(first)->lineN;
				ptr = NULL;
				nextTokenD;
				break;
			case token::type::DELIM:
				ptr = NULL;
				nextTokenD;
				break;
			case token::type::KEYWORD:
			{
				token::keyword *kw = static_cast<token::keyword *>(first);
				switch (kw->word)
				{
					case token::keywords::keywords::GLOBAL:
					{
						nextToken(;);
						bool isConst;
						if ((*p)->getType() != token::type::KEYWORD)
							return errInfo(lineNumber, errPtr, "dim or const expected");
						kw = static_cast<token::keyword*>(*p);
						switch (kw->word)
						{
							case token::keywords::keywords::CONST:
								isConst = true;
								break;
							case token::keywords::keywords::DIM:
								isConst = false;
								break;
							default:
								return errInfo(lineNumber, errPtr, "dim or const expected");
						}
						nextToken(;);
						stnode::alloc *allocPtr = new stnode::alloc(isConst, true);
						allocPtr->lineN = lineNumber;
						allocPtr->pos = first->pos;
						errInfo err = parser_dim(tList, allocPtr, p, lineNumber);
						if (err.err)
						{
							delete allocPtr;
							return err;
						}
						ptr = allocPtr;
						break;
					}
					case token::keywords::keywords::CONST:
					{
						nextToken(;);
						stnode::alloc *allocPtr = new stnode::alloc(true, false);
						allocPtr->lineN = lineNumber;
						allocPtr->pos = first->pos;
						errInfo err = parser_dim(tList, allocPtr, p, lineNumber);
						if (err.err)
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
						stnode::alloc *allocPtr = new stnode::alloc(false, false);
						allocPtr->lineN = lineNumber;
						allocPtr->pos = first->pos;
						errInfo err = parser_dim(tList, allocPtr, p, lineNumber);
						if (err.err)
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
							funcPtr->lineN = lineNumber;
							funcPtr->pos = first->pos;
							nextToken(delete funcPtr;);
							if ((*p)->getType() != token::type::KEYWORD)
							{
								delete funcPtr;
								return errInfo(lineNumber, errPtr, "Type expected");
							}
							token::keyword *type = static_cast<token::keyword *>(*p);
							nextToken(delete funcPtr;);
							int ptrLvl = 0;
							while ((*p)->getType() == token::type::OP && static_cast<token::op *>(*p)->opType == token::ops::opType::MUL)
							{
								ptrLvl++;
								nextToken(delete funcPtr;);
							}
							dataType varType = getVarType(type->word, ptrLvl);
							if (varType.dType == dataType::ERROR)
							{
								delete funcPtr;
								prevToken;
								while (ptrLvl)
								{
									ptrLvl--;
									prevToken;
								}
								return errInfo(lineNumber, errPtr, "Type expected");
							}
							funcPtr->retType = varType;
							if ((*p)->getType() != token::type::ID)
							{
								delete funcPtr;
								return errInfo(lineNumber, errPtr, "Function name expected");
							}
							funcPtr->name = static_cast<token::id *>(*p)->str;
							nextToken(delete funcPtr;);
							if ((*p)->getType() != token::type::OP || static_cast<token::op *>(*p)->opType != token::ops::opType::BRACKET_LEFT)
							{
								delete funcPtr;
								return errInfo(lineNumber, errPtr, "( expected");
							}
							nextToken(delete funcPtr;);
							if ((*p)->getType() == token::type::OP && static_cast<token::op*>(*p)->opType == token::ops::opType::BRACKET_RIGHT)
							{
								nextToken(delete funcPtr;);
							}
							else
							{
								stnode::id *newVar;
								std::string varName;
								for (; p != pEnd;)
								{
									if ((*p)->getType() != token::type::KEYWORD)
									{
										delete funcPtr;
										return errInfo(lineNumber, errPtr, "Parameter type expected");
									}
									type = static_cast<token::keyword *>(*p);
									nextToken(delete funcPtr;);
									int ptrLvl = 0;
									while ((*p)->getType() == token::type::OP && static_cast<token::op *>(*p)->opType == token::ops::opType::MUL)
									{
										ptrLvl++;
										nextToken(delete funcPtr;);
									}
									varType = getVarType(type->word, ptrLvl);
									if (varType.dType == dataType::ERROR || (varType.dType == dataType::VOID && ptrLvl == 0))
									{
										delete funcPtr;
										prevToken;
										while (ptrLvl)
										{
											ptrLvl--;
											prevToken;
										}
										return errInfo(lineNumber, errPtr, "Invalid type");
									}
									if ((*p)->getType() != token::type::ID)
									{
										delete funcPtr;
										return errInfo(lineNumber, errPtr, "Parameter name expected");
									}
									varName = static_cast<token::id *>(*p)->str;
									newVar = new stnode::id(varName, varType);
									newVar->lineN = lineNumber;
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
										token::ops::opType opType = static_cast<token::op *>(*p)->opType;
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
						retPtr->lineN = lineNumber;
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
							if (err.err)
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
						if (err.err)
							return err;
						stnode::ifelse *ifPtr = new stnode::ifelse;
						ifPtr->lineN = lineNumber;
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
						stnode::ifelse *ifPtr = static_cast<stnode::ifelse *>(info.ptr);
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
								stnode::ifelse *ifPtr = static_cast<stnode::ifelse *>(info.ptr);
								if (ifPtr->blockTrue == NULL)
									ifPtr->blockTrue = info.sTree;
								else
									ifPtr->blockFalse = info.sTree;
								ptr = ifPtr;
								break;
							}
							case stnode::type::FUNC:
							{
								stnode::func *funcPtr = static_cast<stnode::func *>(info.ptr);
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
				if (err.err)
					return err;
				ptr = ptrExp;
			}
		}
		if (ptr != NULL)
			sTreeStk.back().sTree->push_back(ptr);
	}
	if (sTreeStk.size() > 1)
		return errInfo(lineNumber + 1, 0, "Missing code block ending");
	return noErr;
}
