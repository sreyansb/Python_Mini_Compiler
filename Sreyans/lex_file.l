%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include "y.tab.h"
    #define maxscope 100
    #define vinscope 100

    int yycol=1;
    int def_mode=0;

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
    }scope

    //scopetable stores multiple scopes and hence the length of scopetable==length of tabstack
    static scope scopetable[maxscope];

    //when we go into a new scope, we need to initialise the scope and also scopetable of that 
    static void init_scope()
    {
      //we need to initialise a scope with scope number=st_ptr
      scopetable[++st_ptr].top_ptr=-1;
    }

    //we need to add a new variable to the current scope
    static void add_to_scope(char* var_name,int linedeclared)
    {
      //when in a particular scope, we can only add to that scope
      ++scopetable[st_ptr].top_ptr;
      strcpy(scopetable[st_ptr].scope_var[scopetable[st_ptr].top_ptr].name,var_name);
      scopetable[st_ptr].scope_var[scopetable[st_ptr].top_ptr].dec_line=linedeclared;
      scopetable[st_ptr].scope_var[scopetable[st_ptr].top_ptr].last_line=linedeclared;
    }

    //we need to first find if the element is declared before.
    //If used before, we update the last_line the variable was used.
    //If not used, initialise in current scope
    static void find_element(char* varname,int lineused)
    {
      for(int i=st_ptr;i>-;--i)
      {
        for(int j=scopetable[i].top_ptr;j>-1;--j)
        {
          if (strcmp(scopetable[i][j].name,varname)==0)
          {
            scopetable[i][j].last_line=lineused;
            return;
          }
        }
      }
      add_to_scope(varname,lineused);
    }
    
    void printscope()
    {
      printf("=========SCOPE_KILLING==================\n")
      printf("SCOPE: %d\n",st_ptr);
      printf("Name\t\t|LineDeclared\t|LastLineUsed|\n");
      for(int i=0;i<=scopetable[st_ptr].top_ptr;++i)
        printf("%s\t\t|%d\t\t\t|%d|\n",scopetable[st_ptr][i].name,scopetable[st_ptr][i].dec_line,scopetable[st_ptr][i].last_line);
      printf("========================================\n");
    }

    void deletescope()
    {
      --st_ptr;
    }
%}
letter [a-zA-Z]
digit [0-9]
space [ ]
tab [\t]
%%
/* I used yyless(yyleng-1) because we need to use /n for the next line */

"\n"["\t"|" "]*"\n" {printf("%d Empty Line\n",yylineno);yyless(yyleng-1);++yylineno;}
"\n"["\t"]*"#"(.)* {printf("%d Single Line Comment",yylineno);++yylineno;}
"\n"["\t"]* {}
%%

int yywrap(void)
{
    return 1;
}

void yyerror(char *s)
{
  printf("Line no: %d %s\n",yylineno,s);
}

int main()
{
  init_scope();
}