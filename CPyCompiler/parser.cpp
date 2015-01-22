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
		default:
			varType = stnode::varType::_ERROR;
	}
	return varType;
}

int parser(tokenList &tList, stTree *sTree)
{
	stnode::stnode *ptr = NULL;
	tokenList::iterator p, pEnd = tList.end();
	int errPtr = 0;
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
				bool isConst = false;
				switch (kw->word)
				{
					case token::keywords::keywords::CONST:
						isConst = true;
					case token::keywords::keywords::DIM:
					{
						stnode::alloc *allocPtr = new stnode::alloc(isConst);
						nextToken(delete allocPtr;);
						first = *p;
						if (first->getType() == token::type::KEYWORD)
						{
							//type
							token::keyword *type = dynamic_cast<token::keyword *>(first);
							nextToken(delete allocPtr;);
							bool isPtr = false;
							if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::DEREF)
							{
								isPtr = true;
								nextToken(delete allocPtr;);
							}
							stnode::varType varType = getVarType(type->word);
							if (varType == stnode::varType::_ERROR)
							{
								delete allocPtr;
								return errPtr;
							}
							stnode::id *newVar = NULL;
							token::id *varName = NULL;
							long long subCount = 1;
							for (; p != pEnd && (*p)->getType() != token::type::DELIM;)
							{
								//name
								varName = dynamic_cast<token::id *>(*p);
								if (varName == NULL)
								{
									delete allocPtr;
									return errPtr;
								}
								nextToken(delete allocPtr;);
								subCount = 0;
								if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::SUB_LEFT)
								{
									//array
									nextToken(delete allocPtr;);
									if ((*p)->getType() != token::type::NUMBER)
									{
										delete allocPtr;
										return errPtr;
									}
									subCount = dynamic_cast<token::number *>(*p)->val;
									nextToken(delete allocPtr;);
									if ((*p)->getType() != token::type::OP || dynamic_cast<token::op *>(*p)->opType != token::ops::opType::SUB_RIGHT)
									{
										delete allocPtr;
										return errPtr;
									}
									nextToken(delete allocPtr;);
								}
								newVar = new stnode::id(varName->str, varType, subCount);
								if ((*p)->getType() == token::type::OP && dynamic_cast<token::op *>(*p)->opType == token::ops::opType::ASSIGN)
								{
									//init val
									nextToken(delete allocPtr; delete newVar;);
									if (subCount == 0)
									{
										//init var
										if ((*p)->getType() != token::type::NUMBER)
										{
											delete allocPtr;
											delete newVar;
											return errPtr;
										}
										long long *initVal = new long long;
										*initVal = dynamic_cast<token::number *>(*p)->val;
										allocPtr->var.push_back(stnode::allocUnit(newVar, initVal));
										nextToken(delete allocPtr; delete newVar;);
									}
									else
									{
										//init array
										if ((*p)->getType() != token::type::OP || dynamic_cast<token::op *>(*p)->opType != token::ops::opType::BRACE_LEFT)
										{
											delete allocPtr;
											delete newVar;
											return errPtr;
										}
										nextToken(delete allocPtr; delete newVar;);
										long long *initVal = new long long[subCount];
										long long i;
										for (i = 0; p != pEnd && i < subCount; p++, errPtr++, i++)
										{
											if ((*p)->getType() != token::type::NUMBER)
											{
												delete allocPtr;
												delete newVar;
												delete[] initVal;
												return errPtr;
											}
											initVal[i] = dynamic_cast<token::number *>(*p)->val;
											nextToken(delete allocPtr; delete newVar; delete[] initVal;);
											if ((*p)->getType() != token::type::OP || dynamic_cast<token::op *>(*p)->opType != token::ops::opType::COMMA)
											{
												delete allocPtr;
												delete newVar;
												delete[] initVal;
												return errPtr;
											}
										}
										allocPtr->var.push_back(stnode::allocUnit(newVar, initVal));
									}
								}
								else
									allocPtr->var.push_back(stnode::allocUnit(newVar));
							}
							if (p == pEnd)
							{
								delete allocPtr;
								return errPtr;
							}
							ptr = allocPtr;
						}
						else
						{
							return errPtr;
						}
						break;
					}
					case token::keywords::keywords::FUNCTION:

						break;
					case token::keywords::keywords::IF:

						break;
				}
				break;
			}
		}
		if (ptr != NULL)
			sTree->push_back(ptr);
	}
	return -1;
}