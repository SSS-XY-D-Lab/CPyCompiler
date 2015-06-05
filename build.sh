#!/bin/bash
g++ CPyCompiler/stdafx.cpp -c -o unix-build/stdafx.o -std=c++11
g++ CPyCompiler/lexer.cpp -c -o unix-build/lexer.o -std=c++11
g++ CPyCompiler/type.cpp -c -o unix-build/type.o -std=c++11
g++ CPyCompiler/parser.cpp -c -o unix-build/parser.o -std=c++11
g++ CPyCompiler/parser_yacc.cpp -c -o unix-build/parser_yacc.o -std=c++11
g++ CPyCompiler/inter.cpp -c -o unix-build/inter.o -std=c++11
g++ CPyCompiler/CPyCompiler.cpp -c -o unix-build/CPyCompiler.o -std=c++11
g++ unix-build/stdafx.o unix-build/lexer.o unix-build/type.o unix-build/parser.o unix-build/parser_yacc.o unix-build/inter.o unix-build/CPyCompiler.o -o unix-build/CPyCompiler
