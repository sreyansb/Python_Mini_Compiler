%{
	#include <math.h>
	#include <string.h>
	#include "y.tab.h"
    #include "stack.c" 

	#define stack_size 100
	#define DEBUG 1
	
	
	int yycolumn = 1;
	int startFlag = 1;


	STACK indent;
	int pointer=0;
	int counter=0;
	int def_mode=0;




	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; 		\
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; 	\
    yycolumn += yyleng;	
    
 	static int sp=0, stack [stack_size];
 	
 	static void debug(const char *X)
 	{
 		#ifdef DEBUG
 		if(startFlag)
 		{
 			printf("********************************TOKENS*******************************\n1 ");
 			startFlag=0;
 		}
 		if(strcmp(X, "NL")==0)
 		{
 			printf("%s\n%d ", X, yylineno);
 		}
		else
		{
			printf("%s ", X);	
		}
 		#endif
 	}
 	
 	
 	static void push_1(int i) 
 	{
   		if (++sp<stack_size) stack[sp]= i;
   		else {printf ("error: stack overflow\n"); exit(1);}
 	}
 	
 	int pop_1() 
 	{
   		if (sp>-1) return stack[sp--];
   		else {printf ("error: stack underflow\n"); exit(1);}
 	}
 	
 	int top_1()
 	{
 		if(sp>-1) return stack[sp];
 		else return -1;
 	}
 	
 	static int indent_depth_1(const char *K)
	{
		int len = strlen(K), i, tab_count=1;
		for(i=0; i< len ; i++)
		{
			if(K[i]=='\t')
			{
				tab_count++;
			}	
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


"\n"["\t"" "]*"\n"                  {debug("Empty Line");
                                     yyless(yyleng-1);
                                     //yylineno++;
                                    }
"\n"["\t"]*"#"[^\n]*                {debug("This is a comment");
                                     //yylineno++;
                                    }
"\n"["\t"]*                         {
                                        
                                        if(def_mode == 0)
                                        {
                                            fprintf(yyout, "%s", yytext);
											yyless(0);
                                            def_mode = 1;
                                            //printf("\n\nNew line\n\n");
                                            yycolumn=1;
                                            debug("NL");
                                            return T_NL;
                                        }
                                        else
                                        {
                                        	
                                            if(yyleng-1 == peek(&indent)){
                                                def_mode = 0;
                                                //yylineno++;
                                                //return ND;//
                                            }
                                            else
                                            {
                                                if(peek(&indent) < yyleng-1)
                                                {

                                                	depth = indent_depth_1(yytext+1);
                                        			yylval.depth = depth;
                                        			push_1(depth);
                                        			
                                                    push(&indent, yyleng-1);
                                                    yyless(0);
                                                    debug("Indent");
                                                    return ID;
                                                }
                                                if(peek(&indent) > yyleng-1)
                                                {
                                        			
                                                	depth = indent_depth_1(yytext+1);
                                        			yylval.depth = depth;
                                        			while (depth < top_1()) pop_1();

                                        			
                                                    int removed_scope = pop(&indent);
                                                    //while (yyleng-1 < peek(&indent))pop(&indent);
                                                    //printTable(removed_scope);
                                                    //clearScope(removed_scope);
                                                    debug("Dedent");
                                                    yyless(0);
                                                    return DD;
                                                }
                                            }

                                        }
                                    }
                                    






"import" {debug("IMPT"); return T_Import;}
"print"  {debug("Print"); return T_Print;}
"len"	{debug("len"); return T_LEN;}
"for"	{debug("for"); return T_For;}
"in"	{debug("in"); return T_IN;}
"pass" {debug("Pass"); return T_Pass;}
"if" {debug("If"); return T_If;}
"while" {debug("While"); return T_While;}
"break" {debug("Break"); return T_Break;}
"and" {debug("And"); return T_And;}
"or" {debug("Or"); return T_Or;}
"not" {debug("Not"); return T_Not;}
"elif" {debug("Elif"); return T_Elif;}
"else" {debug("Else"); return T_Else;}
"def" {debug("Def"); return T_Def;}
"return" {debug("Return"); return T_Return;}
":" {debug("Cln"); return T_Cln;}

";" {debug("Cln"); return T_SCln;}

">" {debug("GT"); return T_GT;}
"<" {debug("LT"); return T_LT;}
">=" {debug("EGT"); return T_EGT;}
"<=" {debug("ELT"); return T_ELT;}
"==" {debug("EQ"); return T_EQ;}
"!=" {debug("NEQ"); return T_NEQ;}
"True" {debug("True"); return T_True;}
"False" {debug("False"); return T_False;}
"+" {debug("PL"); return T_PL;}
"-" {debug("MN"); return T_MN;}
"*" {debug("ML"); return T_ML;}
"/" {debug("DV"); return T_DV;}
"(" { debug("OP"); return T_OP;}
")" {debug("CP"); return T_CP;}
"[" {debug("OB"); return T_OB;}
"]" {debug("CB"); return T_CB;}
"," {debug("Comma"); return T_Comma;}
"=" {debug("EQL"); return T_EQL;}
"range" {debug("range"); return T_Range;}
"list" {debug("List"); return T_List;}
[0-9]+ {yylval.text = strdup(yytext); debug("Number"); return T_Number;}
[_a-zA-Z][_a-zA-Z0-9]* {yylval.text = strdup(yytext); debug(yylval.text); return T_ID;}
\"([^\"\n])*\" {yylval.text = strdup(yytext); debug("String"); return T_String;}
\'([^\'\n])*\' {yylval.text = strdup(yytext); debug("String"); return T_String;}
#(.*)\n ;
{start}.*{start}\n ; 
{Multiline_comment} ;
{whitespace} {} 
"\n" {yycolumn=1; debug("NL"); return T_NL;}
<<EOF>> {debug("EOF"); return T_EndOfFile;}
%%
  
int main()
{
	initStack(&indent, 1000);
   push(&indent, 0);
	yyparse();
	yylex();
	return 0;
	
	/*
	[\t]*		{
			depth = indent_depth(yytext);
			//printf("Depth : %d ",  depth);
			if(depth < top())
			{
				while (depth < top()) pop();
				yylval.depth = depth;
				debug("DD");
				return DD;
			}
			
			if(depth == top())
			{
				debug("ND");
				yylval.depth = depth;
				return ND;				
			}
			if(depth > top())
			{
				push(depth);
				debug("ID");
				yylval.depth = depth;
				return ID;
			}
			
		}
		*/
	
	
}
