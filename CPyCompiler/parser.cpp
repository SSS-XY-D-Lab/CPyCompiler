#include "stdafx.h"
#include "parser.h"

#define nextToken(x) p++; errPtr++; if (p == pEnd) { x return errPtr - 1; }
#define prevToken p--; errPtr--

namespace stnode
{
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

int parser_dim(tokenList &tList, stnode::alloc *allocPtr, tokenList::iterator &p, int &errPtr)
{
	tokenList::iterator pEnd = tList.end();
	nextToken(;);
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
		stnode::varType varType = getVarType(type->word);
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
					if ((*p)->getType() != token::type::NUMBER)
					{
						delete newVar;
						return errPtr;
					}
					long long *initVal = new long long;
					*initVal = dynamic_cast<token::number *>(*p)->val;
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
					long long *initVal = new long long[subCount];
					long long i;
					for (i = 0; p != pEnd && i < subCount; p++, errPtr++, i++)
					{
						if ((*p)->getType() != token::type::NUMBER)
						{
							delete newVar; delete[] initVal; return errPtr;
						}
						initVal[i] = dynamic_cast<token::number *>(*p)->val;
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

int parser_exp(tokenList &tList, stnode::stnode **root, tokenList::iterator &p, int &errPtr)
{
	return -1;
}

struct lvlInfo
{
	lvlInfo(stTree *_sTree, stnode::stnode *_ptr){ sTree = _sTree; ptr = _ptr; }
	stTree *sTree;
	stnode::stnode *ptr;
};

int parser(tokenList &tList, stTree *_sTree, bool inFunc)
{
	stnode::stnode *ptr = NULL;
	std::list<lvlInfo>sTreeStk;
	sTreeStk.push_back(lvlInfo(_sTree, new stnode::stnode));
	tokenList::iterator p, pEnd = tList.end();
	int errPtr = 0;
	bool allowFunc = true;
	for (p = tList.begin(); p != pEnd; p++, errPtr++)
	{
		token::token *first = *p;
		switch (first->getType())
		{
			case token::type::DELIM:
				ptr = NULL;
				break;
			case token::type::KEYWORD:
			{
				token::keyword *kw = dynamic_cast<token::keyword *>(first);
				switch (kw->word)
				{
					case token::keywords::keywords::CONST:
					{
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
							stnode::varType varType = getVarType(type->word);
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
									varType = getVarType(type->word);
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
											break;
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
					case token::keywords::keywords::IF:
					{
						stTree *block = new stTree;
						stnode::ifelse *ifPtr = new stnode::ifelse;
						ifPtr->blockTrue = NULL;
						ifPtr->blockFalse = NULL;
						sTreeStk.push_back(lvlInfo(block, ifPtr));
						break;
					}
					case token::keywords::keywords::ELSE:
					{
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
							}
							case stnode::type::FUNC:
							{
								stnode::func *funcPtr = dynamic_cast<stnode::func *>(info.ptr);
								funcPtr->block = info.sTree;
								ptr = funcPtr;
								allowFunc = true;
							}
						}
						break;
					}
				}
				break;
			}
		}
		if (ptr != NULL)
			sTreeStk.back().sTree->push_back(ptr);
	}
	return -1;
}