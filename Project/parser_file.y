%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h>
	extern int yylineno;
	extern int depth;
	extern int top();
	extern int pop();
	int currentScope = 1, previousScope = 1;
	
	int *arrayScope = NULL;

	typedef struct AST
	{ char lexeme[100];
	int NumChild;
	struct AST **child;
	}AST_node;

	typedef struct record
	{
		char *type;
		char *name;
		int decLineNo;
		int lastUseLine;
	} record;

	typedef struct STable
	{
		int no;
		int noOfElems;
		int scope;
		record *Elements;
		int Parent;
		
	} STable;

	STable *symbolTables = NULL;
	int sIndex = -1;
	char *argsList = NULL;
	
	int power(int base, int exp)
	{
		int i =0, res = 1;
		for(i; i<exp; i++)
		{
			res *= base;
		}
		return res;
	}
	
	void updateCScope(int scope)
	{
		currentScope = scope;
	}
	
	void resetDepth()
	{
		while(top()) pop();
		depth = 10;
	}
	
	int scopeBasedTableSearch(int scope)
	{
		int i = sIndex;
		for(i; i > -1; i--)
		{
			if(symbolTables[i].scope == scope)
			{
				return i;
			}
		}
		return -1;
	}
	void initNewTable(int scope)
	{
		arrayScope[scope]++;
		sIndex++;
		symbolTables[sIndex].no = sIndex;
		symbolTables[sIndex].scope = power(scope, arrayScope[scope]);
		symbolTables[sIndex].noOfElems = 0;		
		symbolTables[sIndex].Elements = (record*)calloc(20, sizeof(record));
		
		symbolTables[sIndex].Parent = scopeBasedTableSearch(currentScope); 
	}
	
	void init()
	{
		argsList = (char *)malloc(100);
		strcpy(argsList, "");
		symbolTables = (STable*)calloc(100, sizeof(STable));
		arrayScope = (int*)calloc(10, sizeof(int));
		initNewTable(1);
		
	}
	
	
	int searchRecordInScope(const char* type, const char *name, int index)
	{
		int i =0;
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if((strcmp(symbolTables[index].Elements[i].type, type)==0) && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				return i;
			}	
		}
		return -1;
	}
		
	void modifyRecordID(const char *type, const char *name, int lineNo, int scope)
	{
		int check_error = 0;
		int i =0;
		int index = scopeBasedTableSearch(scope);
		//printf("No Of Elems : %d\n", symbolTables[index].noOfElems);
		if(index==0) //WHEN is index actuallly =0? when you reach the outer-most scope 
		{
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					symbolTables[index].Elements[i].lastUseLine = lineNo;
					return;
				}	
			}
			//printf("Identifier '%s' at line %d Not Declared\n", name, yylineno);
			//printSTable();
			check_error =1;
			//exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			//printf("\t%d Name: %s\n", i, symbolTables[index].Elements[i].name);
			if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				symbolTables[index].Elements[i].lastUseLine = lineNo;
				return;
			}	
		}
		//printf("Parent : %d\n", symbolTables[index].Parent);
		return modifyRecordID(type, name, lineNo, symbolTables[symbolTables[index].Parent].scope);
	}
	
	void insertRecord(const char* type, const char *name, int lineNo, int scope)
	{ 
		int FScope = power(scope, arrayScope[scope]);
		int index = scopeBasedTableSearch(FScope);
		int recordIndex = searchRecordInScope(type, name, index);
		//printf("rIndex : %d, Name : %s\n", recordIndex, name);
		if(recordIndex==-1) //record doesnt exist in the scope
		{
			
			symbolTables[index].Elements[symbolTables[index].noOfElems].type = (char*)calloc(30, sizeof(char));
			symbolTables[index].Elements[symbolTables[index].noOfElems].name = (char*)calloc(20, sizeof(char));
		
			strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].type, type);	
			strcpy(symbolTables[index].Elements[symbolTables[index].noOfElems].name, name);
			symbolTables[index].Elements[symbolTables[index].noOfElems].decLineNo = lineNo;
			symbolTables[index].Elements[symbolTables[index].noOfElems].lastUseLine = lineNo;
			symbolTables[index].noOfElems++;

		}
		else
		{
			symbolTables[index].Elements[recordIndex].lastUseLine = lineNo;
		}
	}
	
	void checkList(const char *name, int lineNo, int scope)
	{
		int index = scopeBasedTableSearch(scope);
		int i;
		if(index==0)
		{
			
			for(i=0; i<symbolTables[index].noOfElems; i++)
			{
				
				if(strcmp(symbolTables[index].Elements[i].type, "ListTypeID")==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					symbolTables[index].Elements[i].lastUseLine = lineNo;
					return;
				}	

				else if(strcmp(symbolTables[index].Elements[i].name, name)==0)
				{
					printf("Identifier '%s' at line %d Not Indexable or has not been declared as a list\n", name, yylineno);
					yyerror("Invalid Python Syntax");
					return;
					//exit(1);

				}

			}
			printf("Identifier '%s' at line %d Not Declared as an Indexable Type or has not been declared as a list\n", name, yylineno);
			yyerror("Invalid Python Syntax");
			return;
			//exit(1);
		}
		
		for(i=0; i<symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, "ListTypeID")==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				symbolTables[index].Elements[i].lastUseLine = lineNo;
				return;
			}
			
			else if(strcmp(symbolTables[index].Elements[i].name, name)==0)
			{
				printf("Identifier '%s' at line %d Not Indexable or has not been declared as a list\n", name, yylineno);
				yyerror("Invalid Python Syntax");
				return;
				//exit(1);

			}
		}
		
		return checkList(name, lineNo, symbolTables[symbolTables[index].Parent].scope);

	}

	 void addToList(char *newVal, int flag)
  	{
	  	if(flag==0)
	  	{
			  strcat(argsList, ", ");
			  strcat(argsList, newVal);
			}
			else
			{
				strcat(argsList, newVal);
			}
	    printf("\n\t%s\n", newVal);
	  }
	  
	  void clearArgsList()
	  {
	    strcpy(argsList, "");
	  }
	
	void printSTable()
	{
		int i = 0, j = 0;
		
		printf("\n\n----------------------------All Symbol Tables----------------------------");
		printf("\nScope\tName\tType\t\tDeclaration\tLast Used Line\n");
		for(i=0; i<=sIndex; i++)
		{
			for(j=0; j<symbolTables[i].noOfElems; j++)
			{
				printf("(%d, %d)\t%s\t%s\t%d\t\t%d\n", symbolTables[i].Parent, symbolTables[i].scope, symbolTables[i].Elements[j].name, symbolTables[i].Elements[j].type, symbolTables[i].Elements[j].decLineNo,  symbolTables[i].Elements[j].lastUseLine);
			}
		}
	}
	
	void freeAll()
	{
		int i = 0, j = 0;
		for(i=0; i<=sIndex; i++)
		{
			for(j=0; j<symbolTables[i].noOfElems; j++)
			{
				free(symbolTables[i].Elements[j].name);
				free(symbolTables[i].Elements[j].type);	
			}
			free(symbolTables[i].Elements);
		}
		free(symbolTables);
	}

	de->child = NULL;
		return node;
	}

%}

%locations

%union { char *text; int depth; struct AST *node; };
%type <node> StartParse StartDebugger args start_suite suite end_suite finalStatements arith_exp bool_exp term constant basic_stmt cmpd_stmt func_def list_index import_stmt pass_stmt break_stmt print_stmt if_stmt elif_stmts else_stmt while_stmt return_stmt assign_stmt bool_term bool_factor for_stmt func_call call_args list_stmt

%token T_EndOfFile  T_Number  T_Cln T_NL  T_IN T_NEQ T_EQ T_GT T_LT T_EGT T_ELT T_Or T_And  ID ND DD T_String Trip_Quote  T_Import   T_MN T_PL T_DV T_ML T_OP T_CP T_OB T_CB T_Def T_Comma T_Range T_List
%token <text> T_ID T_EQL T_LEN T_True T_False T_Not T_Pass T_Break T_Return T_Print T_If T_Elif T_Else T_For T_While

%right T_EQL 
%left T_PL T_MN
%left T_ML T_DV
%left T_OP T_CP
%nonassoc T_If
%nonassoc T_Elif
%nonassoc T_Else

%%
StartDebugger : {init();} StartParse T_EndOfFile {printf("\nValid Python Syntax\n"); 
printf("\n\n---------------------------------AST-----------------------------------"); AST_print($2);   
printSTable(); freeAll(); exit(0);} ;

constant : T_Number {insertRecord("Constant", $<text>1, @1.first_line, currentScope);
					  }
		 | T_String {insertRecord("Constant", $<text>1, @1.first_line, currentScope);
		 			  };



list_index : T_ID T_OB T_Number T_CB {
									  checkList($<text>1, @1.first_line, currentScope);
									  };

term : T_ID {modifyRecordID("Identifier", $<text>1, @1.first_line, currentScope); 
		} 
     | constant {$$ = $1;} 
     | list_index {$$ = $1;};

StartParse : T_NL StartParse {$$=$2;}
			| finalStatements T_NL {resetDepth();} StartParse {} 
			| finalStatements T_NL {$$=$1;};

basic_stmt : pass_stmt {$$=$1;} 
			| break_stmt {$$=$1;} 
			| import_stmt {$$=$1;} 
			| assign_stmt {$$=$1;} 
			| arith_exp {$$=$1;} 
			| bool_exp {$$=$1;} 
			| print_stmt {$$=$1;}
			| return_stmt {$$=$1;} ;

arith_exp :  term {printf("\nIn arith_exp->term");}
			| arith_exp  T_PL  arith_exp {}
			| arith_exp  T_MN  arith_exp {}
			| arith_exp  T_ML  arith_exp {}
 			| arith_exp  T_DV  arith_exp {}
 			| T_MN term {}
 			| T_OP arith_exp T_CP {$$=$2;};

bool_exp : bool_term T_Or bool_term {}
         | arith_exp T_LT arith_exp {}
         | bool_term T_And bool_term {}
         | arith_exp T_GT arith_exp {}
         | arith_exp T_ELT arith_exp {}
         | arith_exp T_EGT arith_exp {}
         | arith_exp T_IN T_ID {checkList($<text>3, @3.first_line, currentScope); }
         | bool_term {$$=$1}; 

bool_term : bool_factor {$$=$1}
          | arith_exp T_EQ arith_exp {}
          | T_True {insertRecord("Constant", "True", @1.first_line, currentScope); }
          | T_False {insertRecord("Constant", "False", @1.first_line, currentScope);}; 
          
bool_factor : T_Not bool_factor {}
            | T_OP bool_exp T_CP {$$=$2}; 

import_stmt : T_Import T_ID {insertRecord("PackageName", $<text>2, @2.first_line, currentScope); 
							  };

pass_stmt   : T_Pass {};
break_stmt  : T_Break {};
return_stmt : T_Return {}; 

assign_stmt : T_ID T_EQL arith_exp 
			{
			  insertRecord("Identifier", $<text>1, @1.first_line, currentScope); 
			  //$1 = make_leaf($<text>1);
			  }  
			 |T_ID T_EQL bool_exp 
			 {
			 	insertRecord("Identifier", $<text>1, @1.first_line, currentScope);
			 	
			 }   
			 |T_ID  T_EQL func_call
			 {
			 	insertRecord("Identifier", $<text>1, @1.first_line, currentScope);
			 	
			 } 
			 |T_ID T_EQL list_stmt 
			 {
			 	insertRecord("ListTypeID", $<text>1, @1.first_line, currentScope);
			 } 

			 ;
	      
print_stmt : T_Print T_OP term T_CP {};

finalStatements : basic_stmt 
				| cmpd_stmt 
				| func_def ;

cmpd_stmt : while_stmt {$$=$1}
			| for_stmt {$$=$1};

for_stmt: T_For T_ID T_IN range_stmt T_Cln start_suite {}
		 | T_For T_ID T_IN T_ID T_Cln start_suite {checkList($<text>4, @4.first_line, currentScope);} ; 

while_stmt : T_While bool_exp T_Cln start_suite {$$ =make_node("WHILE", $2, $4);}; 

range_stmt: T_Range T_OP T_Number T_CP 
			| T_Range T_OP T_Number T_Comma T_Number T_CP 
			| T_Range T_OP T_Number T_Comma T_Number T_Comma T_Number T_CP ;
start_suite : basic_stmt {} 
			| ID { initNewTable($<depth>1); updateCScope($<depth>1);} finalStatements suite {};

suite : ND finalStatements suite {}
		| end_suite {};

end_suite : DD {updateCScope($<depth>1);} finalStatements {}
		|{};
 
%%

void yyerror(const char *msg)
{
	//printSTable();
	printf("\n\n%s", msg);
	printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.first_line);
	printSTable();
	exit(0);
}


