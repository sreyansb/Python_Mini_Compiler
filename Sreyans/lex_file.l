%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include "y.tab.h"
    #define maxscope 100
    #define vinscope 100

    extern YYSTYPE yylval;

    int def_mode=1;

    static int tabstack[maxscope];
    static int st_ptr=-1;//to keep track of the topmost element in the stack
    
    //stores the details about a symbol.Doesn't store the type because type is run time evaluation in python
    typedef struct sym_node
 {
        char name[100];
        int dec_line;
        int last_line;
    }sym_node;

    //assuming that we have vinscope elements in every scope
    //top_ptr=points to the topmost element in each scope
    //basically scope stores all the variables defined in a scope
    typedef struct scope
    {
        sym_node scope_var[vinscope];
        int top_ptr;
    }scope;

    static int peek()
    {
      return tabstack[st_ptr];
    }

    typedef struct symtabnode
	  {
		  char name[100];
		  int scope;
    }symtabnode;

      //scopetable stores multiple scopes and hence the length of scopetable==length of tabstack
    static scope scopetable[maxscope];

    //when we go into a new scope, we need to initialise the scope and also scopetable of that 
    static void init_scope(int no_of_tabs)
    {
      //we need to initialise a scope with scope number=st_ptr
      tabstack[++st_ptr]=no_of_tabs;
      scopetable[no_of_tabs].top_ptr=-1;
    }

    //we need to add a new variable to the current scope
    static int add_to_scope(char* var_name,int linedeclared)
    {
      //when in a particular scope, we can only add to that scope
      ++scopetable[tabstack[st_ptr]].top_ptr;
      strcpy(scopetable[tabstack[st_ptr]].scope_var[scopetable[tabstack[st_ptr]].top_ptr].name,var_name);
      scopetable[tabstack[st_ptr]].scope_var[scopetable[tabstack[st_ptr]].top_ptr].dec_line=linedeclared;
      scopetable[tabstack[st_ptr]].scope_var[scopetable[tabstack[st_ptr]].top_ptr].last_line=linedeclared;
      return st_ptr;
    }

    //we need to first find if the element is declared before.
    //If used before, we update the last_line the variable was used.
    //If not used, initialise in current scope
    static int find_element(char* varname,int lineused)
    {
      for(int i=st_ptr;i>-1;--i)
      {
        for(int j=scopetable[tabstack[i]].top_ptr;j>-1;--j)
        {  
          if (strcmp(scopetable[tabstack[i]].scope_var[j].name,varname)==0)
          {
            scopetable[tabstack[i]].scope_var[j].last_line=lineused;
            return i;
          }
        }
      }
      return add_to_scope(varname,lineused);
    }
    
    void printscope(int sta_ptr)
    {
      printf("=======================================\n\n");
      printf("SCOPE: %d Number of Tabs: %d\n",sta_ptr,tabstack[sta_ptr]);
      printf("Name\t\t|LineDeclared\t|LastLineUsed|\n");
      for(int i=0;i<=scopetable[tabstack[sta_ptr]].top_ptr;++i)
        printf("%s\t\t|%d\t\t|%d\t|\n",scopetable[tabstack[sta_ptr]].scope_var[i].name,scopetable[tabstack[sta_ptr]].scope_var[i].dec_line,scopetable[tabstack[sta_ptr]].scope_var[i].last_line);
      printf("\n========================================\n");
    }

    void deletescope()
    {
      printscope(st_ptr);
      scopetable[tabstack[st_ptr]].top_ptr=-1;
      --st_ptr;
    }

    static int finddepth(char* line)
    {
      int line_depth=0;
      while(*line=='\n')
        line++;
      while(line)
      {
        if (*line=='\t')++line_depth;
        else
        {break;}
        ++line;
      }
      return line_depth;
    }
%}
letter [a-zA-Z]
digit [0-9]
space [ ]
tab [\t]
%%
"\n"["\t"|" "]*"\n" {printf("Empty Line\n");yyless(yyleng-1);++yylineno;}
["\n"]*["\t"]*"#"(.)* {printf("\n Single Line Comment ");++yylineno;}
"\n"\'\'\'[.|\n]*\'\'\'"\n" {printf("MultiLineComment ");yyless(yyleng-1);++yylineno;}
["\n"]+["\t"]* {
  if (def_mode)
 {
    yyless(0);
    def_mode=0;
    printf("NL\n");
    return T_NL;
  }
  int line_depth=finddepth(yytext);
  int flag=1;int ptr;
  while(line_depth!=(ptr=peek()))
 {
    if (line_depth>ptr)
    {
      init_scope(line_depth);
      printf("Indented to %d tabs\n",line_depth);
      return T_IND;
    }
    else
    {
      if (ptr==-1)
      {
        printf("Wrong number of tabs\n");
        exit(1);
        return 0;
      }
      printf("Deleting\n");
      deletescope();
      yyless(0);
      return T_DED;
    }
  }
  def_mode=1;
  printf("%d)",yylineno);
  ++yylineno;
  
}
<<EOF>> {
  if (peek()!=0)
 {
    deletescope();
    printf("Dedenting\n");
    return T_DED;}
  else
 {
    printf("EOF-all at scope 0 tab 0\n");
    printscope(0);
    return T_EOF;
    yyterminate();
    
  }
}

"=" {printf( "%s ", yytext);return T_EQ;}
"," {printf( "%s ", yytext);return T_Comma;}
"del" {printf( "%s ", yytext);return T_Del;}
"pass" {printf( "%s ", yytext);return T_Pass;}
"break" {printf( "%s ", yytext);return T_Break;}
"continue" {printf( "%s ", yytext);return T_Continue;}
"print" {printf("%s ",yytext);return T_Print;}
"import" {printf( "%s ", yytext);return T_Import;}
"from" {printf( "%s ", yytext);return T_From;}
"range" {printf( "%s ", yytext);return T_Range;}
"in" {printf("%s ",yytext);return T_In;}
"(" {printf( "%s ", yytext);return T_LP;}
")" {printf( "%s ", yytext);return T_RP;}
":" {printf( "%s ", yytext);return T_Cln;}
"for" {printf( "%s ", yytext);return T_For;}
"while" {printf( "%s ", yytext);return T_While;}
"or" {printf( "%s ", yytext);return T_Or;}
"and" {printf( "%s ", yytext);return T_And;}
"not" {printf( "%s ", yytext);return T_Not;}
"<" {printf( "%s ", yytext);return T_Lt;}
">=" {printf( "%s ", yytext);return T_Gte;}
"<=" {printf( "%s ", yytext);return T_Lte;}
">" {printf( "%s ", yytext);return T_Gt;}
"==" {printf( "%s ", yytext);return T_Deq;}
"+" {printf( "%s ", yytext);return T_Plus;}
"-" {printf( "%s ", yytext);return T_Minus;}
"*" {printf( "%s ", yytext);return T_Star;}
"/" {printf( "%s ", yytext);return T_Divide;}
"%" {printf( "%s ", yytext);return T_Mod;}
"//" {printf( "%s ", yytext);return T_DDiv;}
"**" {printf( "%s ", yytext);return T_Power;}
"[" {printf( "%s ", yytext);return T_Ls;}
"]" {printf( "%s ", yytext);return T_Rs;}
"True" {printf( "%s ", yytext);return T_True;}
"False" {printf( "%s ", yytext);return T_False;}
" "+ {;}
({letter}|_)({letter}|_|{digit})* {
                    printf("Matched : %s ", yytext);
                    yylval.data=(symtabnode*)malloc(sizeof(symtabnode));
                    strcpy(yylval.data->name,yytext);
                    int scopei = find_element(yytext,yylineno);
                    yylval.data->scope=scopei;
                    return T_ID;
                    }
[1-9]*{digit}+ {
  yylval.data=(symtabnode*)malloc(sizeof(symtabnode));strcpy(yylval.data->name,yytext);
  yylval.data->scope=-1;printf( "%s", yytext);return T_Integer;}
[1-9]*{digit}+"."{digit}+ {
  yylval.data=(symtabnode*)malloc(sizeof(symtabnode));strcpy(yylval.data->name,yytext);
  yylval.data->scope=-1;printf( "%s", yytext);return T_Real;} 
\"([^\"\n])*\" {
  yylval.data=(symtabnode*)malloc(sizeof(symtabnode));strcpy(yylval.data->name,yytext);
  yylval.data->scope=-1;printf( "%s", yytext);return T_String;}
\'([^\'\n])*\' {
  yylval.data=(symtabnode*)malloc(sizeof(symtabnode));strcpy(yylval.data->name,yytext);
  yylval.data->scope=-1;printf( "%s", yytext);return T_String;}
%%

int yywrap(void)
 {
    return 1;
}

void yyerror(char* s)
{
  printf("\nLine no: %d %s %s\n",yylineno,s,yytext);
}


int main()
 {
  init_scope(0);
  yyparse();
  return 0;
}