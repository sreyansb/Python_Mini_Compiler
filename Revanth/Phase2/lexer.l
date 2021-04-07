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
	int def_mode=0;
	int depth = 1;

	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; 		\
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; 	\
    yycolumn += yyleng;	
    
 	static int sp=0, stack [stack_size];
 	
 	static void debug(const char *X)
 	{
 		#ifdef DEBUG
 		if(startFlag)
 		{
 			printf("********************************TOKENS*******************************\n");
 			printf("%d ",yylineno);
 			startFlag=0;
 		}
 		if(strcmp(X, "NL")==0)
 		{
 			printf("%s\n%d ", X, yylineno);
 			//printf("%d ", yylineno);
 		}
		else
		{
			printf("%s ", X);
		}
 		#endif
 		
 	}
 	
 	static int count_nl(const char *s)
 	{
 		int count = 0;
 		int len = strlen(s);
 		for(int i =0; i < len; ++i)
		{
			if(s[i] == '\n')
			{
				++count;
			}
		}
		return count;
 	}

%}

whitespace [ ]
digit [0-9]
Multiline_comment \'\'\'(.|\n)*\'\'\'\n

%%

"\n"["\t"" "]*"\n"               	{		
                                    	yyless(yyleng-1);
                                    	yylineno++;
                                    	debug("NL");
                                    	debug("Empty Line");
                                    }
                                    
["\n"]?["\t"]*"#"[^\n]*             {
                                    	if(yytext[0] == '\n')
                                    	{
                                    		yylineno++;
											debug("NL");
											debug("Comment");
										}
										else
											debug("Comment");
                                    }
                                    
"\n"["\t"]*                         {
                                        if(def_mode == 0)
                                        {
                                            //fprintf(yyout, "%s", yytext);
											yyless(0);
                                            def_mode = 1;
                                            yycolumn=1;
                                            yylineno++;
                                            debug("NL");
                                            return T_NL;
                                        }
                                        else
                                        {
                                            if(yyleng-1 == peek(&indent)){
                                                def_mode = 0;
                                                //yylineno++;
                                            }
                                            else
                                            {
                                                if(peek(&indent) < yyleng-1)
                                                {
                                        			yylval.depth = yyleng;
                                                    push(&indent, yyleng-1);
                                                    yyless(0);
                                                    debug("Indent");
                                                    return ID;
                                                }
                                                if(peek(&indent) > yyleng-1)
                                                {
                                        			yylval.depth = yyleng;
                                                    int removed_scope = pop(&indent);
                                                    debug("Dedent");
                                                    yyless(0);
                                                    return DD;
                                                }
                                            }
                                        }
                                    }

"import" {debug("Import"); return T_Import;}
"print"  {debug("Print"); return T_Print;}
"len"	{debug("len"); return T_LEN;}
"for"	{debug("For"); return T_For;}
"in"	{debug("IN"); return T_IN;}
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
";" {debug("SCln"); return T_SCln;}
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
"range" {debug("Range"); return T_Range;}
"list" {debug("List"); return T_List;}

[-]?[1-9]+{digit}*	{yylval.text = strdup(yytext); debug("Number"); return T_Number;}
[-]?{digit}*"."{digit}+	{yylval.text = strdup(yytext); debug("Number"); return T_Number;}

[_a-zA-Z][_a-zA-Z0-9]* {yylval.text = strdup(yytext); debug(yylval.text); return T_ID;}
\"([^\"\n])*\" {yylval.text = strdup(yytext); debug("String"); return T_String;}
\'([^\'\n])*\' {yylval.text = strdup(yytext); debug("String"); return T_String;}

{Multiline_comment} {yyless(yyleng-1); yylineno += (count_nl(yytext));debug("Multiline Comment");  }
{whitespace} ;

<<EOF>> {debug("EOF"); return T_EndOfFile;}
%%
  
int main()
{
	initStack(&indent, 1000);
   push(&indent, 0);
	yyparse();
	yylex();
	return 0;
}
