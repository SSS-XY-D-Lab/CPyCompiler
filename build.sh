#!/bin/bash
g++ CPyCompiler/CPyCompiler.cpp -c -o unix-build/CPyCompiler.o -std=c++11 -I CPyCompiler/
g++ CPyCompiler/inter.cpp -c -o unix-build/inter.o -std=c++11 -I CPyCompiler/
g++ CPyCompiler/lexer.cpp -c -o unix-build/lexer.o -std=c++11 -I CPyCompiler/
g++ CPyCompiler/parser.cpp -c -o unix-build/parser.o -std=c++11 -I CPyCompiler/
g++ CPyCompiler/parser_yacc.cpp -c -o unix-build/parser_yacc.o -std=c++11 -I CPyCompiler/
g++ CPyCompiler/stdafx.cpp -c -o unix-build/stdafx.o -std=c++11 -I CPyCompiler/
g++ unix-build/CPyCompiler.o unix-build/inter.o unix-build/lexer.o unix-build/parser.o unix-build/parser_yacc.o unix-build/stdafx.o -o unix-build/CPyCompiler

