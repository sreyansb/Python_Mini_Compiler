%{
	#include <math.h>
#include <string.h>
	#include "y.tab.h"
	#define stack_size 100
	
	int yycolumn = 1;
	int startFlag = 1;
    
	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; 		\
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; 	\
    yycolumn += yyleng;	

 	static int sp=0, stack [stack_size];
 	
 	static void push (int i) 
 	{
   		if (++sp<stack_size) stack[sp]= i;
   		else {printf ("error: stack overflow\n"); exit(1);}
 	}
 	
 	int pop () 
 	{
   		if (sp>-1) return stack[sp--];
   		else {printf ("error: stack underflow\n"); exit(1);}
 	}
 	
 	int top()
 	{
 		if(sp>-1) return stack[sp];
 		else return 1;
 	}
 	
 	static int indent_depth(const char *K)
	{
		int len = strlen(K), i, tab_count=0;
		for(i=0; i< len ; i++)
		{
			if(K[i]=='\t')
			{
				tab_count++;
			}
			else
				break;	
		}
		return tab_count;
	}	
 	int depth = 1; 
%}
%option yylineno
whitespace [ ]
start \'\'\' 


Multiline_comment \'\'\'[\n].+[\n]\'\'\'\n
%%

[\n]+[\t]*		{
			depth = indent_depth(yytext);
			//printf("DEPTH %d\n",depth);
			if (depth==0)
				return T_NL;
			//printf("Depth : %d ",  depth);
			if(depth < top())
			{
				while (depth < top()) pop();
				yylval.depth = depth;
				return DD;
			}
			
			if(depth == top())
			{
				yylval.depth = depth;
				return ND;				
			}
			if(depth > top())
			{
				push(depth);
				yylval.depth = depth;
				return ID;
			}
			
		}
"import" { return T_Import;}
"print"  { return T_Print;}
"len"	{ return T_LEN;}
"for"	{ return T_For;}
"in"	{ return T_IN;}
"pass" { return T_Pass;}
"if" { return T_If;}
"while" { return T_While;}
"break" { return T_Break;}
"and" { return T_And;}
"or" { return T_Or;}
"not" { return T_Not;}
"elif" { return T_Elif;}
"else" { return T_Else;}
"def" { return T_Def;}
"return" { return T_Return;}
":" { return T_Cln;}
">" { return T_GT;}
"<" { return T_LT;}
">=" { return T_EGT;}
"<=" { return T_ELT;}
"==" { return T_EQ;}
"!=" { return T_NEQ;}
"True" { return T_True;}
"False" { return T_False;}
"+" { return T_PL;}
"-" { return T_MN;}
"*" { return T_ML;}
"/" { return T_DV;}
"(" {  return T_OP;}
")" { return T_CP;}
"[" { return T_OB;}
"]" { return T_CB;}
"," { return T_Comma;}
"=" { return T_EQL;}
"range" { return T_Range;}
"list" { return T_List;}
[0-9]+ {yylval.text = strdup(yytext);  return T_Number;}
[_a-zA-Z][_a-zA-Z0-9]* {yylval.text = strdup(yytext);  return T_ID;}
\"([^\"\n])*\" {yylval.text = strdup(yytext);  return T_String;}
\'([^\'\n])*\' {yylval.text = strdup(yytext);  return T_String;}
#(.*)\n ;
{start}.*{start}\n ; 
{Multiline_comment} ;
{whitespace} {}/*
"\n" {yycolumn=1;  return T_NL;}*/
<<EOF>> { return T_EndOfFile;}
%% 

int main()
{
	yyparse();
	yylex();
	
	return 0;
}

