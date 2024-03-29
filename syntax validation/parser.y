%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h>



	//--------------------------------BASIC VARIABLE DECLARATIONS----------------------------------------
	extern int yylineno;
	extern int depth;
	extern int top_1();
	extern int pop_1();
	int currentScope = 1, previousScope = 1;
	
	int *arrayScope = NULL;

	//quad stuff
	#define MAXQUADS 1000
	typedef struct quad
	{
		char *R;
		char *A1;
		char *A2;
		char *Op;
		int I;
	} quad;
	
	quad* all_quads = NULL;	
	char* tString =NULL, *lString = NULL;
	void printQuads();

	int ICG_opt(); //optimization code
	int ICG_opt2(); //common sub expr eval
	void createCSV(); //Write final ICG to a CSV for assembly code generation


	//-------------------------------------STRUCTURE DEFINITIONS----------------------------------------------
	
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

	//--------------------------------------------------------------------------------------------------------
	STable *symbolTables = NULL;
	int sIndex = -1;
	char *argsList = NULL;
	//--------------------------------------------------------------------------------------------------------
	
	//------------------------------------SYMBOL TABLE FUNCTIONS----------------------------------------------
	void insertRecord(const char* type, const char *name, int lineNo, int scope);

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
		while(top_1()) pop_1();
		depth = 1;
	}
	
	int scopeBasedTableSearch(int scope)
	{
		int i = sIndex;//sIndex indicates number of symbol tables i.e. length of symboltables array
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
		all_quads = (quad*)calloc(MAXQUADS, sizeof(quad));
		tString = (char*)calloc(10, sizeof(char));
		lString = (char*)calloc(10, sizeof(char));
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
		//printf("\n In modifyRecordID");
		int check_error = 0;
		int i = 0;
		int index = scopeBasedTableSearch(scope);
		//printf("No Of Elems : %d\n", symbolTables[index].noOfElems);
		if(index==0) //WHEN is index actuallly =0? when you reach the outer-most scope 
		{
			for(i = 0;i < symbolTables[index].noOfElems; i++)
			{	
				if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
				{
					symbolTables[index].Elements[i].lastUseLine = lineNo;
					return;
				}	
			}
			printf("\nIdentifier '%s' at line %d Not Declared\n", name, yylineno);

			//Error Recovery
			insertRecord(type, name, lineNo, scope);

			yyerror("Invalid Python Syntax");
			//printSTable();
			check_error = 1;
			//exit(1);
		}
		
		for(i = 0;i < symbolTables[index].noOfElems; i++)
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
					printf("\nIdentifier '%s' at line %d Not Indexable or has not been declared as a list\n", name, yylineno);
					insertRecord("ListTypeID", name, lineNo, currentScope);
					yyerror("Invalid Python Syntax");
					return;
					//exit(1);

				}

			}
			printf("\nIdentifier '%s' at line %d Not Declared as an Indexable Type or has not been declared as a list\n", name, yylineno);
			insertRecord("ListTypeID", name, lineNo, currentScope);
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
				insertRecord("ListTypeID", name, lineNo, currentScope);
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
	    //printf("\n\t%s\n", newVal);
	  }
	  
	void clearArgsList()
	{
	    strcpy(argsList, "");
	}
	
	void printSTable()
	{
		int i = 0, j = 0;
		
		printf("\n\n*************************SYMBOL TABLES**************************");
		printf("\nScope\tName\tType\t\tLine Decl\tLast Used Line\n");
		for(i=0; i<=sIndex; i++)
		{
			for(j=0; j<symbolTables[i].noOfElems; j++)
			{
				printf("(%d, %d)\t%s\t%s\t%d\t\t%d\n", symbolTables[symbolTables[i].Parent].scope, symbolTables[i].scope/*-1*/, symbolTables[i].Elements[j].name, symbolTables[i].Elements[j].type, symbolTables[i].Elements[j].decLineNo,  symbolTables[i].Elements[j].lastUseLine);
			}
		}
	}
	
	void freeAll()
	{
		//ICG_opt(); 
		//printf("\nAfter dead code elimination");

		//printQuads();
		printf("\n");
		//printf("\nAfter common sub expression elimination");
		//ICG_opt2(); //CSE elimination
		//printQuads();
		printf("\n");
		//createCSV();
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

%}


%locations

%union { char *text; int depth; struct AST *node; };
//%type <node> StartParse StartDebugger args start_suite suite end_suite finalStatements arith_exp bool_exp term constant basic_stmt cmpd_stmt func_def list_index import_stmt pass_stmt break_stmt print_stmt if_stmt elif_stmts else_stmt while_stmt return_stmt assign_stmt bool_term bool_factor for_stmt func_call call_args list_stmt


%token T_EndOfFile T_Cln T_SCln T_NL T_IN T_NEQ T_EQ T_GT T_LT T_EGT T_ELT T_Or T_And ID ND DD T_String Trip_Quote T_Import T_MN T_PL T_DV T_ML T_OP T_CP T_OB T_CB T_Def T_Comma T_Range T_List

%token <text> T_Number T_ID T_EQL T_LEN T_True T_False T_Not T_Pass T_Break T_Return T_Print T_If T_Elif T_Else T_For T_While

%right T_EQL 
%left T_PL T_MN
%left T_ML T_DV
%left T_OP T_CP
%nonassoc T_If
%nonassoc T_Elif
%nonassoc T_Else
//%glr-parser
 
 

%%
StartDebugger : {init();} 	StartParse  {printf("\nValid Python Syntax\n" ); 
printf("**************************************************************************\n");

//printSTable();// freeAll();  
exit(0);} ;

constant : T_Number /*{insertRecord("Constant", $<text>1, @1.first_line, currentScope);}*/
		 | T_String /*{insertRecord("Constant", $<text>1, @1.first_line, currentScope);}*/;



list_index : T_ID T_OB T_Number T_CB /*{checkList($<text>1, @1.first_line, currentScope);}*/;


term : T_ID /*{modifyRecordID("Identifier", $<text>1, @1.first_line, currentScope);}*/
     | constant 
     | list_index ;

StartParse : T_NL StartParse 
			| finalStatements {/*resetDepth(); updateCScope(1);*/} StartParse 
			| T_EndOfFile;

basic_stmt : pass_stmt 
			| break_stmt 
			| import_stmt 
			| assign_stmt 
			| arith_exp 
			| bool_exp 
			| print_stmt 
			| return_stmt 
			| func_call ; 

arith_exp :  term 
			| arith_exp  T_PL  arith_exp 
			| arith_exp  T_MN  arith_exp
			| arith_exp  T_ML  arith_exp
 			| arith_exp  T_DV  arith_exp 
 			| T_MN term
 			| T_OP arith_exp T_CP;

		    



bool_exp : bool_term T_Or bool_term 
		 | arith_exp T_Or arith_exp 
         | arith_exp T_LT arith_exp 
         | bool_term T_And bool_term 
         | arith_exp T_And arith_exp 
         | arith_exp T_GT arith_exp 
         | arith_exp T_ELT arith_exp 
         | arith_exp T_EGT arith_exp 
         | arith_exp T_IN T_ID {checkList($<text>3, @3.first_line, currentScope);}
         | bool_term ; 

bool_term : bool_factor
          | arith_exp T_EQ arith_exp 
          | T_True {insertRecord("Constant", "True", @1.first_line, currentScope);}
          | T_False {insertRecord("Constant", "False", @1.first_line, currentScope);}; 
          
bool_factor : T_Not bool_factor 
            | T_OP bool_exp T_CP;

import_stmt : T_Import T_ID {insertRecord("PackageName", $<text>2, @2.first_line, currentScope);};

pass_stmt   : T_Pass ;
break_stmt  : T_Break ;
return_stmt : T_Return | T_Return T_ID {char return_val[100]; strcpy(return_val, "return "); strcat(return_val, $<text>2);}; 

assign_stmt :  T_ID  T_EQL func_call {insertRecord("Identifier", $<text>1, @1.first_line, currentScope);}
			 | T_ID T_EQL bool_exp {insertRecord("Identifier", $<text>1, @1.first_line, currentScope);}
			 | T_ID T_EQL arith_exp {insertRecord("Identifier", $<text>1, @1.first_line, currentScope);}
			 | T_ID T_EQL list_stmt {insertRecord("ListTypeID", $<text>1, @1.first_line, currentScope);};
	      
print_stmt : T_Print T_OP term T_CP ;

finalStatements : simple_stmt//basic_stmt
				| cmpd_stmt ;
				//| func_def ;

simple_stmt
	: basic_stmt next_simple_stmt ;
	
next_simple_stmt
	: end_simple_stmt
	| T_SCln basic_stmt next_simple_stmt ;

end_simple_stmt
	: T_NL
	| T_SCln T_NL
	| %empty ;

cmpd_stmt : if_stmt 
			| while_stmt 
			| for_stmt ;


if_stmt : T_If bool_exp T_Cln suite elif_stmt optional_else ;

elif_stmt
	: %empty
	| T_Elif bool_exp T_Cln suite elif_stmt;


optional_else
	: %empty
	| T_Else T_Cln suite ;


for_stmt
	: T_For T_ID T_IN range_stmt T_Cln suite optional_else {insertRecord("Identifier", $<text>2, @2.first_line, currentScope); 
				char rangeNodeText[20] ="";
				strcat(rangeNodeText, $<text>2);
				strcat(rangeNodeText, " in range");
				clearArgsList(); }
	| T_For T_ID T_IN T_ID T_Cln suite optional_else {insertRecord("Identifier", $<text>2, @2.first_line, currentScope); 
		 //printSTable();
		 checkList($<text>4, @4.first_line, currentScope);};


/*for_stmt:  T_For T_ID T_IN range_stmt T_Cln start_suite {insertRecord("Identifier", $<text>2, @2.first_line, currentScope); 
				char rangeNodeText[20] ="";
				strcat(rangeNodeText, $<text>2);
				strcat(rangeNodeText, " in range");
				clearArgsList(); }
		 | T_For T_ID T_IN T_ID T_Cln start_suite {insertRecord("Identifier", $<text>2, @2.first_line, currentScope); 
		 //printSTable();
		 checkList($<text>4, @4.first_line, currentScope);}; */

/*while_stmt : T_While bool_exp T_Cln start_suite ; */

while_stmt
	: T_While bool_exp T_Cln suite optional_else ;

range_stmt: T_Range T_OP T_Number T_CP {addToList("0", 1); addToList($<text>3, 0);}
			| T_Range T_OP T_Number T_Comma T_Number T_CP {addToList($<text>3, 1); addToList($<text>5, 0);}
			| T_Range T_OP T_Number T_Comma T_Number T_Comma T_Number T_CP {addToList($<text>3, 1); addToList($<text>5, 0); addToList($<text>7,0);};
			
/*start_suite : basic_stmt
			| T_NL ID {initNewTable($<depth>2); updateCScope($<depth>2);} finalStatements suite;

suite : T_NL ND finalStatements suite 
		| T_NL end_suite ;

end_suite : DD {updateCScope($<depth>1);} finalStatements
			| {resetDepth(); updateCScope(1);}; */
			

suite
	: simple_stmt
	| T_NL ID /*{initNewTable($<depth>2); updateCScope($<depth>2);}*/ finalStatements repeat_stmt DD /*{updateCScope($<depth>5);}*/;

repeat_stmt
	: finalStatements repeat_stmt
	| ;	

args : T_ID {insertRecord("Identifier", $<text>1, @1.first_line, currentScope); addToList($<text>1, 1);} args_list 
		| {clearArgsList();};

args_list : T_Comma T_ID {insertRecord("Identifier", $<text>2, @2.first_line, currentScope); addToList($<text>2, 0);} args_list 
			| {addToList("",0); clearArgsList();};

//func_def : T_Def T_ID {insertRecord("Func_Name", $<text>2, @2.first_line, currentScope);} T_OP args T_CP T_Cln start_suite {clearArgsList();} ;

list_stmt: T_OB T_CB 
		 | T_OB call_args T_CB {
		 		char* str = (char *)malloc(102*sizeof(char));
			 	strcpy(str,"[");
			 	strcat(str, argsList);
			 	char close[2];
			 	strcpy(close,"]");
			 	strcat(str, close);
			 	clearArgsList(); 
			 	free(str);};

call_list : T_Comma term /*{addToList($2->lexeme, 0);}*/ call_list 
			| ;

call_args : T_ID {modifyRecordID("Identifier", $<text>1, @1.first_line, currentScope); addToList($<text>1, 1);} call_list
					| T_Number {addToList($<text>1, 1);} call_list {clearArgsList();}
					| T_String {addToList($<text>1, 1);} call_list {clearArgsList();}
					| ;

func_call : T_ID {modifyRecordID("Func_Name", $<text>1, @1.first_line, currentScope);} T_OP call_args T_CP ;
 
 
%%

void yyerror(const char *msg)
{
	printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.last_column);
	printf("**************************************************************************\n");
	printf("**************************************************************************\n");
}

