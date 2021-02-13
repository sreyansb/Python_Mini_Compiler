%{
    #include<stdio.h>
    #include<stdlib.h>
    #include <string.h>
	#include <stdarg.h>

	extern int yylineno;
	extern int depth;
	extern int top();
	extern int pop();
	int currentScope = 1;

    
%}