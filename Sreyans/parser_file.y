%{
    #include<stdio.h>
    #include<stdlib.h>
    #include <string.h>
	#include <stdarg.h>
	
	#define maxscopey 10000
	extern char* yytext;
	//because size of each scope(100) is 100 elements
	
	typedef struct symtabnode
	{
		char name[100];
		int scope;
	}symtabnode;
	
	//will store all the elements and their scopes
	symtabnode symtab[maxscopey];
	int indexy=0;//first index to be filled in the symbol table

	void addtotable(char* name,int scope)
	{
		strcpy(symtab[indexy].name,name);
		symtab[indexy].scope=scope;
		++indexy;
	}

	static void searchele(char* name,int scope)
	{
		for(int i=0;i<indexy;++i)
		{
			if (strcmp(name,symtab[i].name)==0)
				{
					symtab[i].scope=scope;
					return;
				}
		}
		addtotable(name,scope);
	}

	void printTable()
	{
		printf("Name\t|Scope\t\n");
		for(int i=0;i<indexy;++i)
			printf("%s\t|%d\t\n",symtab[i].name,symtab[i].scope);
	}  
%}
%locations

%start start_karo
%union {struct symtabnode* data;};
%token T_NL T_IND T_DED T_EOF T_EQ T_Comma T_Del T_Pass T_Break T_Continue T_In
%token T_Print T_Import T_From T_Star T_LP T_RP T_Cln T_For T_While T_Or T_Range
%token T_And T_Not T_Lt T_Gt T_Lte T_Gte T_Deq T_Plus T_Minus T_Divide T_Mod T_DDiv
%token T_Power T_Ls T_Rs T_True T_False T_ID T_Integer T_Real T_String

%left T_Plus T_Minus
%left T_Star T_Divide T_DDiv T_Mod
%left T_LP T_RP
%right T_EQ

%%

start_karo
	: T_NL start_karo
	| stmt start_karo
	| T_EOF {printf("\nAccepted Code:Valid\n\n");printTable();exit(0);}

term
	: T_String

math_term
	: T_ID {searchele($<data->name>1,$<data->scope>1);}
	| T_Real
	| T_Integer

stmt
	: simple_stmt
	| compound_stmt

simple_stmt
	: base_stmt

base_stmt
	: pass_stmt
	| delete_stmt
	| import_stmt
	| cobr_stmt
	| assign_stmt
	| print_stmt
	| printable_stmt

pass_stmt
	: T_Pass

delete_stmt
	: T_Del T_ID

import_stmt
	: T_Import T_ID {searchele($<data->name>2,$<data->scope>2);}
	| import_from

import_from
	: T_From T_ID T_Import T_ID end_import_from

end_import_from
	: T_Comma T_ID end_import_from
	| %empty

cobr_stmt
	: T_Break
	| T_Continue

assign_stmt
	: T_ID T_EQ printable_stmt {searchele($<data->name>1,$<data->scope>1);}

print_stmt
	: T_Print T_LP printable_stmt T_RP

printable_stmt
	: arith_stmt
	| bool_stmt
	| list_stmt

arith_stmt
	: arith_stmt T_Plus arith_stmt
	| arith_stmt T_Minus arith_stmt
	| arith_stmt T_Star arith_stmt
	| arith_stmt T_Divide arith_stmt
	| arith_stmt T_DDiv arith_stmt
	| arith_stmt T_Mod arith_stmt
	| T_LP arith_stmt T_RP
	| math_term
	//| T_Minus arith_stmt %prec UMINUS

bool_stmt
	: bool_term T_Or bool_term
	| bool_term T_And bool_term
	| bool_term
	| T_Not bool_stmt
	| T_LP bool_stmt T_RP
	| arith_stmt comp_op arith_stmt

bool_term
	: term
	| T_True
	| T_False

comp_op
	: T_Lt
	| T_Gt
	| T_Deq
	| T_Lte
	| T_Gte

compound_stmt
	: for_stmt
	| while_stmt

for_stmt
	: T_For T_ID T_In range_stmt T_Cln block_code {searchele($<data->name>2,$<data->scope>2);}
	| T_For T_ID T_In list_stmt T_Cln block_code {searchele($<data->name>2,$<data->scope>2);}

range_stmt
	: T_Range T_LP T_Integer T_RP
	| T_Range T_LP T_ID T_RP
	| T_Range T_LP T_Integer T_Comma T_Integer T_RP
	| T_Range T_LP T_ID T_Comma T_ID T_RP
	| T_Range T_LP T_Integer T_Comma T_Integer T_Comma T_Integer T_RP
	| T_Range T_LP T_ID T_Comma T_ID T_Comma T_ID T_RP

list_stmt
	: T_Ls T_Rs
	| T_Ls args T_Rs

args
	: term items
	| math_term items

items
	: T_Comma term items
	| T_Comma math_term items
	| %empty

while_stmt
	: T_While bool_stmt T_Cln block_code

block_code
	: base_stmt
	| T_NL T_IND stmt repeater T_DED

repeater
	: %empty
	| stmt repeater
%%

