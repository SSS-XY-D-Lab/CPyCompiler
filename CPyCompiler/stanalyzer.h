#pragma once

#ifndef _H_STANLYZER
#define _H_STANLYZER

#include "parser.h"
#include <unordered_map>

typedef std::unordered_map<std::string, stnode::id*> idListLayer;
typedef std::list<idListLayer*> idList;
int stanalyzer(stTree &ret);

#endif