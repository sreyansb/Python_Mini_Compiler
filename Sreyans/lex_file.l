%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include "y.tab.h"
    #define maxscope 100
    #define vinscope 100

    int yycol=1;
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
    }scope

    static int peek()
 {
      return tabstack[st_ptr];
    }

    //scopetable stores multiple scopes and hence the length of scopetable==length of tabstack
    static scope scopetable[maxscope];

    //when we go into a new scope, we need to initialise the scope and also scopetable of that 
    static void init_scope(int no_of_tabs)
 {
      //we need to initialise a scope with scope number=st_ptr
      tabstack[++st_ptr]=no_of_tabs
      scopetable[no_of_tabs].top_ptr=-1;
    }

    //we need to add a new variable to the current scope
    static void add_to_scope(char* var_name,int linedeclared)
 {
      //when in a particular scope, we can only add to that scope
      ++scopetable[tabstack[st_ptr]].top_ptr;
      strcpy(scopetable[tabstack[st_ptr]].scope_var[scopetable[tabstack[st_ptr]].top_ptr].name,var_name);
      scopetable[tabstack[st_ptr]].scope_var[scopetable[tabstack[st_ptr]].top_ptr].dec_line=linedeclared;
      scopetable[tabstack[st_ptr]].scope_var[scopetable[tabstack[st_ptr]].top_ptr].last_line=linedeclared;
    }

    //we need to first find if the element is declared before.
    //If used before, we update the last_line the variable was used.
    //If not used, initialise in current scope
    static void find_element(char* varname,int lineused)
 {
      for(int i=st_ptr;i>-1;--i)
 {
        for(int j=scopetable[tabstack[i]].top_ptr;j>-1;--j)
 {
          if (strcmp(scopetable[tabstack[i]][j].name,varname)==0)
 {
            scopetable[tabstack[i]][j].last_line=lineused;
            return;
          }
        }
      }
      add_to_scope(varname,lineused);
    }
    
    void printscope()
 {
      printf("=========SCOPE_KILLING==================\n")
      printf("SCOPE: %d Number of Tabs: %d\n",st_ptr,tabstack[st_ptr]);
      printf("Name\t\t|LineDeclared\t|LastLineUsed|\n");
      for(int i=0;i<=scopetable[tabstack[st_ptr]].top_ptr;++i)
        printf("%s\t\t|%d\t\t\t|%d|\n",scopetable[tabstack[st_ptr]][i].name,scopetable[tabstack[st_ptr]][i].dec_line,scopetable[tabstack[st_ptr]][i].last_line);
      printf("========================================\n");
    }

    void deletescope()
 {
      printscope();
      scopetable[tabstack[st_ptr]].top_ptr=-1;
      --st_ptr;
    }

    static int finddepth(char* line)
 {
      int line_depth=0;
      while(line)
 {
        *line=='t'?++line_depth:line_depth;
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
"\n"["\t"|" "]*"\n" {printf("%d Empty Line\n",yylineno);yyless(yyleng-1);++yylineno;}
"\n"["\t"]*"#"(.)* {printf("%d Single Line Comment",yylineno);++yylineno;}
"\n"\'\'\'[.|\n]*\'\'\'"\n" {printf("%d MultiLineComment",yylineno);++yylineno;yyless(yyleng-1);}
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
    flag=0;
    if (line_depth>ptr)
 {
      init_scope(line_depth);
      printf("Indented to %d tabs\n",line_depth);
      return T_IND;
    }
    else
 {
      if (ptr==-1)
 {yyerror("Wrong number of tabs\n");
        return 0;}
      printf("Deleting\n");
      deletescope();
      yyless(0);
      return T_DED;
    }
  }
  def_mode=1;
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
    yyterminate();
    return T_EOF;
  }
}

";" {printf( "%s", yytext);return T_semi_colon;}
"=" {printf( "%s", yytext);return T_eq;}
"," {printf( "%s", yytext);return T_comma;}
"+=" {printf( "%s", yytext);return T_plus_eq;}
"-=" {printf( "%s", yytext);return T_minus_eq;}
"*=" {printf( "%s", yytext);return T_star_eq;}
"/=" {printf( "%s", yytext);return T_div_eq;}
"%=" {printf( "%s", yytext);return T_mod_eq;}
"|=" {printf( "%s", yytext);return T_or_eq;}
"^=" {printf( "%s", yytext);return T_xor_eq;}
"del" {printf( "%s", yytext);return T_del;}
"pass" {printf( "%s", yytext);return T_pass;}
"break" {printf( "%s", yytext);return T_break;}
"continue" {printf( "%s", yytext);return T_continue;}
"return" {printf( "%s", yytext);return T_return;}
"print" {printf("%s",yytext);return T_print;}
"import" {printf( "%s", yytext);return T_import;}
"from" {printf( "%s", yytext);return T_from;}
"*" {printf( "%s", yytext);return T_star;}
"(" {printf( "%s", yytext);return T_Lparan;}
")" {printf( "%s", yytext);return T_Rparan;}
":" {printf( "%s", yytext);printf("Matched Colon\n"); return T_colon;}
"for" {printf( "%s", yytext);return T_for;}
"while" {printf( "%s", yytext);return T_while;}
"in" {printf( "%s", yytext);return T_in;}
"or" {printf( "%s", yytext);return T_or;}
"and" {printf( "%s", yytext);printf("Matched and\n"); return T_and;}
"not" {printf( "%s", yytext);return T_not;}
"<" {printf( "%s", yytext);return T_lt;}
">" {printf( "%s", yytext);return T_gt;}
"==" {printf( "%s", yytext);return T_deq;}
">=" {printf( "%s", yytext);return T_ge;}
"<=" {printf( "%s", yytext);return T_le;}
"!=" {printf( "%s", yytext);return T_noteq;}
"|" {printf( "%s", yytext);return T_bitwiseor;}
"&" {printf( "%s", yytext);return T_bitwiseand;}
"^" {printf( "%s", yytext);return T_bitwisexor;}
"<<" {printf( "%s", yytext);return T_leftshift;}
">>" {printf( "%s", yytext);return T_rightshift;}
"+" {printf( "%s", yytext);return T_plus;}
"-" {printf( "%s", yytext);return T_minus;}
"/" {printf( "%s", yytext);return T_divide;}
"%" {printf( "%s", yytext);return T_mod;}
"//" {printf( "%s", yytext);return T_double_divide;}
"~" {printf( "%s", yytext);return T_tilde;}
"**" {printf( "%s", yytext);return T_double_star;}
"[" {printf( "%s", yytext);return T_squarebleft;}
"]" {printf( "%s", yytext);return T_squarebright;}
"..." {printf( "%s", yytext);return T_ellipsis;}
"None" {printf( "%s", yytext);return T_none;}
"True" {printf( "%s", yytext);return T_true;}
"False" {printf( "%s", yytext);return T_false;}
" "+ {printf( " ");}
({letter}|_)({letter}|_|{digit})* {
                    printf("Matched : %s\n", yytext);
                    char* variable_name = yytext;
                    find_element(yytext,yylineno);
                    return T_name;
                    }
 [1-9]*{digit}+ {printf( "%s", yytext);return T_number;}
 [1-9]*{digit}+"."{digit}+ {printf( "%s", yytext);return T_float;}
 
\"(.)*\" {printf( "%s", yytext);return T_string;}

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