%{
    #include<stdio.h>
    #include<stdlib.h>
    #include <string.h>
	#include <stdarg.h>
	
	#define maxscopey 10000
	extern char* yytext;
	int k=1;
	FILE* fptr = fopen("icg.txt","w");
	//because size of each scope(100) is 100 elements
	
	typedef struct symtabnode
	{
		char name[100];//has the value of constants
		int scope;
		int value;
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

	static int searchele(char* name,int scope)
	{
		for(int i=0;i<indexy;++i)
		{
			if (strcmp(name,symtab[i].name)==0)
				{
					symtab[i].scope=scope;
					return 1;
				}
		}
		addtotable(name,scope);
		return 0;
	}

	void printTable()
	{
		printf("Name\t|Scope\t\n");
		for(int i=0;i<indexy;++i)
			printf("%s\t|%d\t\n",symtab[i].name,symtab[i].scope);
	}  

	void generate(char* name)
	{
		//snprintf(s,n,format,args_to_format) is a function which writes into s, n number of characters(should give space for NULL), the format.
		snprintf(name,5,"t%d",k++);//I have given name atmost 999 temporaries(1 to 999)-t+(1-999) 4 characters+NULL;
	}

%}
%locations

%start start_karo
%union {struct symtabnode* data;};
%token T_NL T_IND T_DED T_SAI T_EOF T_EQ T_Comma T_Del T_Pass T_Break T_Continue T_In
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
	: T_ID {searchele($<data->name>1,$<data->scope>1);strcpy($<data->name>$,$<data->name>1);}
	| T_Real {strcpy($<data->name>$,$<data->name>1);}
	| T_Integer {strcpy($<data->name>$,$<data->name>1);}

stmt
	: simple_stmt {strcpy($<data->name>$,$<data->name>1);}
	| compound_stmt {strcpy($<data->name>$,$<data->name>1);}

simple_stmt
	: base_stmt {strcpy($<data->name>$,$<data->name>1);}

base_stmt
	: pass_stmt {strcpy($<data->name>$,$<data->name>1);}
	| delete_stmt {strcpy($<data->name>$,$<data->name>1);}
	| import_stmt {strcpy($<data->name>$,$<data->name>1);}
	| cobr_stmt {strcpy($<data->name>$,$<data->name>1);}
	| assign_stmt {strcpy($<data->name>$,$<data->name>1);}
	| print_stmt {strcpy($<data->name>$,$<data->name>1);}
	| printable_stmt {strcpy($<data->name>$,$<data->name>1);}

pass_stmt
	: T_Pass {strcpy($<data->name>$,"PASS\0");fprintf(fptr,"PASS\n");}

delete_stmt
	: T_Del T_ID

import_stmt
	: T_Import T_ID {searchele($<data->name>2,$<data->scope>2);{strcpy($<data->name>$,"IMPORT\0");fprintf(fptr,"IMPORT %s\n",$<data->name>1);}}
	| import_from

import_from
	: T_From T_ID T_Import T_ID end_import_from

end_import_from
	: T_Comma T_ID end_import_from
	| %empty

cobr_stmt
	: T_Break {strcpy($<data->name>$,"BREAK\0");fprintf(fptr,"BREAK\n");}
	| T_Continue {strcpy($<data->name>$,"BREAK\0");fprintf(fptr,"BREAK\n");}

assign_stmt
	: T_ID T_EQ printable_stmt {searchele($<data->name>1,$<data->scope>1);strcpy($<data->name>$,$<data->name>1);fprintf(fptr,"%s = %s",$<data->name>1,$<data->name>3);}

print_stmt
	: T_Print T_LP printable_stmt T_RP

printable_stmt
	: arith_stmt {strcpy($<data->name>$,$<data->name>1);}
	| bool_stmt {strcpy($<data->name>$,$<data->name>1);}
	| list_stmt {strcpy($<data->name>$,$<data->name>1);}

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
	: T_String items
	| T_Real items
	| T_Integer items
	| T_ID {if (searchele($<data->name>1,$<data->scope>1)==0)
	printf("\nERROR: %s Not Defined\n",yytext);return 0;}

items
	: T_Comma T_String items
	| T_Comma T_Real items
	| T_Comma T_Integer items
	| T_Comma T_ID items {if (searchele($<data->name>2,$<data->scope>2)==0)
	printf("\nERROR: %s Not Defined\n",yytext);return 0;}
	| %empty

while_stmt
	: T_While bool_stmt T_Cln block_code

block_code
	: base_stmt
	| T_NL T_IND stmt repeater T_DED

repeater
	: stmt repeater
	| %empty
%%

