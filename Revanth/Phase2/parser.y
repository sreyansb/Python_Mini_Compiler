%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h>
	
	
	//#define MAXQUADS 1000
	
	
	void yyerror(const char *msg);
	int yylex();
	
	void for_code_before_suite(char *rtext, char *list);
	void for_code_after_suite();
	
	//--------------------------------BASIC VARIABLE DECLARATIONS----------------------------------------
	
	extern FILE *fptr;

	extern int yylineno;
	extern int depth;

	int currentScope = 1;
	int check_error = 0;
	
	int *arrayScope = NULL;
	
	int tempNo = 1;
	//int temp2;
	
	int templ;
	
	//char *loop_var;	
	// levels of for loop nesting
	char *loop_var[10];
	int li = 0;
	
	typedef struct quad
	{
		char *R;
		char *A1;
		char *A2;
		char *Op;
		int I;
	} quad;
	
	quad all_quads[300];	
	char* tString =NULL, *lString = NULL;
	void printQuads();

	int ICG_opt(); //optimization code
	int ICG_opt2(); //common sub expr eval
	void createCSV();

	//-------------------------------------STRUCTURE DEFINITIONS----------------------------------------------
	//typedef struct STable STable; //
	
	typedef struct record
	{
		char *type;
		char *name;
		int decLineNo;
		int lastUseLine;
		
		int value; //		
	} record;

	typedef struct STable
	{
		int no;
		int noOfElems;
		int scope;
		record *Elements;
		int Parent;	
	} STable;
	
	typedef struct mynode{
		char *addr;
		char *code;
	}mynode;

	struct mynode* get_node()
	{
		return (struct mynode*)malloc(sizeof(struct mynode));
	}

	//--------------------------------------------------------------------------------------------------------
	STable *symbolTables = NULL;
	int sIndex = -1;
	char *argsList = NULL;
	
	
	//--------------------------------------------------------------------------------------------------------
	
	//------------------------------------SYMBOL TABLE FUNCTIONS----------------------------------------------
	//#include "symboltable.h"
	
	int power(int base, int exp)
	{
		int res = 1;
		for(int i = 0; i < exp; i++)
		{
			res *= base;
		}
		return res;
	}
	
	void updateCScope(int scope)
	{
		currentScope = scope;
	}
	
	int scopeBasedTableSearch(int scope)
	{
		for(int i = sIndex; i > -1; i--)//sIndex indicates number of symbol tables i.e. length of symboltables array
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
		symbolTables[sIndex].Elements = (record*)calloc(200, sizeof(record));
		
		symbolTables[sIndex].Parent = scopeBasedTableSearch(currentScope); 
	}
	
	void init()
	{
		argsList = (char *)malloc(1000);
		strcpy(argsList, "");
		symbolTables = (STable*)calloc(1000, sizeof(STable));

		arrayScope = (int*)calloc(100, sizeof(int));
		initNewTable(1);
	}
	
	// searching in the particular symbol table
	int searchRecordInScope(const char* type, const char *name, int index)
	{
		for(int i = 0; i < symbolTables[index].noOfElems; i++)
		{
			if((strcmp(symbolTables[index].Elements[i].type, type)==0) && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				return i;
			}
		}
		return -1;
	}
		
	void insertRecord(const char* type, const char *name, int lineNo, int scope)
	{ 
		int FScope = power(scope, arrayScope[scope]);
		int index = scopeBasedTableSearch(FScope);
		int recordIndex = searchRecordInScope(type, name, index);

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
	
	void modifyRecordID(const char *type, const char *name, int lineNo, int scope)
	{		
		int FScope = power(scope, arrayScope[scope]);
		int index = scopeBasedTableSearch(FScope);
		
		if(index==0)
		{
			for(int i = 0;i < symbolTables[index].noOfElems; i++)
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
		
		for(int i = 0;i < symbolTables[index].noOfElems; i++)
		{
			if(strcmp(symbolTables[index].Elements[i].type, type)==0 && (strcmp(symbolTables[index].Elements[i].name, name)==0))
			{
				symbolTables[index].Elements[i].lastUseLine = lineNo;
				return;
			}	
		}
		return modifyRecordID(type, name, lineNo, symbolTables[symbolTables[index].Parent].scope);
	}
	
	
	void checkList(const char *name, int lineNo, int scope)
	{
		int index = scopeBasedTableSearch(scope);
		if(index==0)
		{
			for(int i=0; i<symbolTables[index].noOfElems; i++)
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
		
		for(int i=0; i<symbolTables[index].noOfElems; i++)
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
		printf("\n");

		printf("\n");

		for(int i=0; i<=sIndex; i++)
		{
			for(int j=0; j<symbolTables[i].noOfElems; j++)
			{
				free(symbolTables[i].Elements[j].name);
				free(symbolTables[i].Elements[j].type);	
			}
			free(symbolTables[i].Elements);
		}
		free(symbolTables);
	}
	//--------------------------------------------------------------------------------------------------------
	//-------------------------------------ICG FUNCTIONS/STRUCTS---------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	
	int qIndex = 0; //used to index the quads array
	int lIndex = 0; //used to index the labels
	void make_quad(char *Op, char *A1, char *A2, char *R)
	{
		all_quads[qIndex].R = (char*)malloc(strlen(R)+1);
		all_quads[qIndex].Op = (char*)malloc(strlen(Op)+1);
		all_quads[qIndex].A1 = (char*)malloc(strlen(A1)+1);
		all_quads[qIndex].A2 = (char*)malloc(strlen(A2)+1);
		strcpy(all_quads[qIndex].R, R);
		strcpy(all_quads[qIndex].A1, A1);
		strcpy(all_quads[qIndex].A2, A2);
		strcpy(all_quads[qIndex].Op, Op);
		all_quads[qIndex].I = qIndex;
 
		qIndex++;
		
		return;
	}

	void printQuads()
	{
		printf("\n********************************QUADS*********************************\n");
		int i = 0;
		printf("Line No\t\tOp\t\tArg1\t\tArg2\t\tRes\n");
		for(i=0; i<qIndex; i++)
		{
			if(all_quads[i].I > -1)
				printf("%d\t\t%s\t\t%s\t\t%s\t\t%s\n", all_quads[i].I, all_quads[i].Op, all_quads[i].A1, all_quads[i].A2, all_quads[i].R);
				//create_icg_csv(all_quads[i].Op,all_quads[i].A1, all_quads[i].A2, all_quads[i].R );

		}
		printf("**************************************************************************\n");
	}

	void Xitoa(int num, char *str)
	{
		if(str == NULL)
		{
			printf("Allocate Memory\n");
			return;
		}
		sprintf(str, "%d", num);
	}

	char *makeStr(int no, int flag)
	{
		char A[10];
		//Xitoa(no, A);
		sprintf(A, "%d", no);
		
		if(flag==1) //if it's a variable
		{
				char *tString = (char *)malloc(sizeof(char)*100);
				strcpy(tString, "t");
				strcat(tString, A);
				strcat(tString, "\0");
				insertRecord("ICGTempVar", tString, -1, 0);
				return tString;
		}
		else //if it's a label
		{
				char *lString = (char *)malloc(sizeof(char)*100);
				strcpy(lString, "L");
				strcat(lString, A);
				strcat(tString, "\0");
				insertRecord("ICGTempLabel", lString, -1, 0);
				return lString;
		}
	}

	void replaceAll( char CSE[100][10], char* replaceStr, int CSE_len, int start_index)
	{
		for(int i=0; i<CSE_len; i++)
		{
			for(int j=start_index; j<qIndex; j++)
			{
				if( strcmp(all_quads[j].A1, CSE[i])==0)
				{
					strcpy(all_quads[j].A1,replaceStr);
				}

				if( strcmp(all_quads[j].A2, CSE[i])==0)
				{
					strcpy(all_quads[j].A2,replaceStr);
				}

			}
		}
	}

	int ICG_opt()
	{
		
		int i = 0, j = 0, flag = 1, dont_remove=0;
		while(flag==1)
		{
			
			flag=0;
			for(i=0; i<qIndex; i++)
			{
				dont_remove=0;
				if(!((strcmp(all_quads[i].R, "-")==0) | (strcmp(all_quads[i].Op, "Call")==0) | (strcmp(all_quads[i].Op, "Label")==0) | (strcmp(all_quads[i].Op, "goto")==0) | (strcmp(all_quads[i].Op, "If False")==0)))
				{
					for(j=0; j<qIndex; j++)
					{
							if(((strcmp(all_quads[i].R, all_quads[j].A1)==0) && (all_quads[j].I!=-1)) | ((strcmp(all_quads[i].R, all_quads[j].A2)==0) && (all_quads[j].I!=-1)))
							{
								//if a particular 'T0' is used as arg1/arg2 in another quad entry
								dont_remove=1; //keep the quad entry, don't remove it
							}
					}
				
					if((dont_remove==0) & (all_quads[i].I != -1))
					{
						//you're eliminating the quad, by not actually removing it just making its index = -1 so that it can never be printed via the print function
						all_quads[i].I = -1;
						flag=1;
					}
				}
			}
		}
		return flag;
	}

	int ICG_opt2()
	{
		
		int i = 0, j = 0;
			
			for(i=0; i<qIndex; i++)
			{
				quad temp_quad = all_quads[i];
				char CSEs[100][10];
				int CSE_index=0;
				
				for(j=i+1; j<qIndex; j++)
				{
					if( (strcmp(temp_quad.Op,all_quads[j].Op )==0) && (strcmp(temp_quad.A1,all_quads[j].A1 )==0) && (strcmp(temp_quad.A2,all_quads[j].A2 )==0) && (strcmp(temp_quad.Op,"Print")!=0)&& (strcmp(temp_quad.Op,"Param")!=0) && (strcmp(temp_quad.Op,"Call")!=0) && (strcmp(temp_quad.Op,"Label")!=0))
					{
						//check if the 2 quads have the same Op, A1 and A2
						//Print stmts should be excluded to retain pgm logic
						//printf("temp_quad.R=%s\n", temp_quad.R);

						strcpy(CSEs[CSE_index],all_quads[j].R); 
						//keep a copy of the CSE to replace in the rest of the table

						all_quads[j].I=-1; //eliminate the CSE from all_quads (the entire row)

						//printf("CSE: %s\n", CSEs[CSE_index]);
						CSE_index++;


					}
					
					if(CSE_index>0)
					{
						
						//printf("CSE_index=%d\n", CSE_index);

						replaceAll(CSEs,temp_quad.R,CSE_index,j); //occuring as A1 or A2
						CSE_index=0;
					}
				}

			}
		
	} 

	void createCSV()
	{
		FILE *fp;
		char *filename;
		filename = "tac_quadruple.csv";
		fp = fopen(filename,"w+");
		printf("\nWriting to file\n");
		int i = 0;
		fprintf(fp, "Line No\tOp\tArg1\tArg2\tRes\n");
		for(i=0; i<qIndex; i++)
		{
			if(all_quads[i].I > -1)
				fprintf(fp, "%d\t%s\t%s\t%s\t%s\n", all_quads[i].I, all_quads[i].Op, all_quads[i].A1, all_quads[i].A2, all_quads[i].R);
				//create_icg_csv(all_quads[i].Op,all_quads[i].A1, all_quads[i].A2, all_quads[i].R );

		}
		fclose(fp);

		printf("\n%s file created\n",filename);
	}
	
%}

%locations
%union { char text[100]; int depth; char *addr; char * code; struct mynode* node;};
//%type <node> StartParse StartDebugger args suite finalStatements arith_exp bool_exp term constant basic_stmt cmpd_stmt func_def list_index import_stmt pass_stmt break_stmt print_stmt if_stmt /*else_stmt*/ while_stmt return_stmt assign_stmt bool_term bool_factor for_stmt func_call call_args list_stmt

%type<node> StartParse StartDebugger args suite finalStatements arith_exp bool_exp term constant simple_stmt basic_stmt cmpd_stmt func_def list_index import_stmt pass_stmt break_stmt print_stmt /*if_stmt else_stmt*/ while_stmt return_stmt assign_stmt bool_term bool_factor for_stmt /*func_call call_args*/ list_stmt

%token T_EndOfFile T_Cln T_SCln T_NL T_IN T_NEQ T_EQ T_GT T_LT T_EGT T_ELT T_Or T_And ID ND DD T_String Trip_Quote T_Import T_MN T_PL T_DV T_ML T_OP T_CP T_OB T_CB T_Def T_Comma T_Range T_List

%token<text> T_Number T_ID T_EQL T_LEN T_True T_False T_Not T_Pass T_Break T_Return T_Print T_If T_Elif T_Else T_For T_While

%right T_EQL 
%left T_PL T_MN
%left T_ML T_DV
%left T_OP T_CP
%nonassoc T_If
%nonassoc T_Elif
%nonassoc T_Else

%%
StartDebugger : {init();} 	
				StartParse  {
								if(check_error == 0)
									printf("\nValid Python Syntax\n"); 
								else
									printf("\nInvalid Python Syntax\n"); 
								printf("**************************************************************************\n");
								printSTable();// freeAll();  
								printQuads();
								createCSV();
								exit(0);
							} ;

constant : T_Number {
						insertRecord("Constant", $<text>1, @1.first_line, currentScope);
						fprintf(fptr,"t%d = %s\n", tempNo, $<text>1);
						make_quad("=", $<text>1,  "-", makeStr(tempNo, 1));
						$$ = get_node();
						$<node->addr>$ = strdup(makeStr(tempNo++, 1));
					}
		 | T_String {
			 			insertRecord("Constant", $<text>1, @1.first_line, currentScope);
			 			fprintf(fptr,"t%d = %s\n", tempNo,$<text>1);
						make_quad("=", $<text>1,  "-", makeStr(tempNo, 1));
						$$ = get_node();
						$<node->addr>$ = strdup(makeStr(tempNo++, 1));
					};

list_index : T_ID T_OB T_Number T_CB {checkList($<text>1, @1.first_line, currentScope);
				fprintf(fptr,"t%d = %s\n", tempNo, $<text>3);
				make_quad("=", $<text>3, "-", makeStr(tempNo++, 1));
				fprintf(fptr,"t%d = %s[t%d]\n", tempNo, $<text>1, tempNo-1);
				make_quad("=[]", $<text>1, makeStr(tempNo-1, 1), makeStr(tempNo, 1));
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));
			};


term : T_ID {
				modifyRecordID("Identifier", $<text>1, @1.first_line, currentScope);
				fprintf(fptr,"t%d = %s\n", tempNo,$<text>1);
				make_quad("=", $<text>1,  "-", makeStr(tempNo, 1));
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));
			}
     | constant {$$ = $1;}
     | list_index {$$ = $1;};

StartParse : T_NL StartParse {$$ = $2;}
			| finalStatements {/*resetDepth();*/ updateCScope(1);} StartParse {/*char *temp[200]; //sprintf(temp, "%s%s", $<node->code>1, $<node->code>2); $<node->code>$ = strdup(temp); 
			*/} 
			| T_EndOfFile {$$ = NULL;};

basic_stmt : pass_stmt {$$=$1;} 
			| break_stmt {$$=$1;} 
			| import_stmt {$$=$1;} 
			| assign_stmt {$$=$1;} 
			| arith_exp {$$=$1;} 
			| T_MN arith_exp {$$=$2;} //
			| bool_exp {$$=$1;} 
			| print_stmt {$$=$1;} 
			| return_stmt {$$=$1;} 
			;//| func_call {$$ = NULL;}; 

arith_exp :  term {$$=$1;} 
			| arith_exp  T_PL  arith_exp {
				fprintf(fptr, "t%d = %s + %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++,1));}
			| arith_exp  T_MN  arith_exp{
				fprintf(fptr, "t%d = %s - %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++,1));}
			| arith_exp  T_ML  arith_exp{
				fprintf(fptr, "t%d = %s * %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++,1));}
 			| arith_exp  T_DV  arith_exp {
				fprintf(fptr, "t%d = %s mod %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++,1));}
 			//| T_MN term
 			| T_OP arith_exp T_CP {$$ = $2;};


bool_exp : bool_term T_Or bool_term {
				fprintf(fptr, "t%d = %s or %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));}
		 | arith_exp T_Or arith_exp {
				fprintf(fptr, "t%d = %s or %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));}
         | arith_exp T_LT arith_exp {
				fprintf(fptr, "t%d = %s < %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));}
         | bool_term T_And bool_term {
				fprintf(fptr, "t%d = %s and %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));}
         | arith_exp T_And arith_exp {
				fprintf(fptr, "t%d = %s and %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));}
         | arith_exp T_GT arith_exp {
				fprintf(fptr, "t%d = %s > %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));
				$<node->code>$ = strdup("code");
				}
         | arith_exp T_ELT arith_exp {
				fprintf(fptr, "t%d = %s <= %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));}
         | arith_exp T_EGT arith_exp {
				fprintf(fptr, "t%d = %s >= %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));}
         | arith_exp T_IN T_ID {checkList($<text>3, @3.first_line, currentScope);
				fprintf(fptr, "t%d = %s in %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));}
         | bool_term {$$ = $1;};

bool_term : bool_factor {$$ = $1;}
          | arith_exp T_EQ arith_exp {
  				fprintf(fptr, "t%d = %s == %s\n", tempNo, $<node->addr>1, $<node->addr>3);
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));
			}
          | T_True {//insertRecord("Constant", "True", @1.first_line, currentScope);
				fprintf(fptr,"t%d = True\n", tempNo);
				make_quad("=", "True",  "-", makeStr(tempNo, 1));
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));
          }
          | T_False {//insertRecord("Constant", "False", @1.first_line, currentScope);
				fprintf(fptr,"t%d = False\n", tempNo);
				make_quad("=", "False",  "-", makeStr(tempNo, 1));
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));
          }; 
          
bool_factor : T_Not bool_factor {
				fprintf(fptr,"t%d = ! %s\n", tempNo, $<node->addr>1);
				make_quad("!", $<node->addr>1,  "-", makeStr(tempNo, 1));
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));
			}
            | T_OP bool_exp T_CP {$$ = $2;};

import_stmt : T_Import T_ID {insertRecord("PackageName", $<text>2, @2.first_line, currentScope);
				fprintf(fptr,"t%d = %s\n", tempNo, $<text>2);
				make_quad("=", $<text>2,  "-", makeStr(tempNo, 1));
				fprintf(fptr,"import t%d\n", tempNo);
				make_quad("import", makeStr(tempNo, 1),  "-", "-");
				$$ = get_node();
				$<node->addr>$ = strdup(makeStr(tempNo++, 1));
			};

pass_stmt   : T_Pass {$$ = NULL;};
break_stmt  : T_Break {fprintf(fptr, "goto L%d\n",lIndex);$$ = NULL;};
return_stmt : T_Return {$$ = NULL;}
			| T_Return term {$$ = NULL;};

assign_stmt : T_ID T_EQL func_call {insertRecord("Identifier", $<text>1, @1.first_line, currentScope);
				fprintf(fptr,"%s = %s\n", $<text>1, $<node->addr>3);
				make_quad("=", $<node->addr>3, "-", $<text>1);
			}
			 |T_ID T_EQL bool_exp {insertRecord("Identifier", $<text>1, @1.first_line, currentScope);
				fprintf(fptr,"%s = %s\n", $<text>1, $<node->addr>3);
				make_quad("=", $<node->addr>3, "-", $<text>1);
			 }
			 | T_ID T_EQL arith_exp {insertRecord("Identifier", $<text>1, @1.first_line, currentScope);
				fprintf(fptr,"%s = %s\n", $<text>1, $<node->addr>3);
				make_quad("=", $<node->addr>3, "-", $<text>1);
			 }
			 | T_ID T_EQL list_stmt {insertRecord("ListTypeID", $<text>1, @1.first_line, currentScope);
				fprintf(fptr,"%s = %s\n", $<text>1, $<node->addr>3);
				make_quad("=", $<node->addr>3, "-", $<text>1);
			 };

print_stmt : T_Print T_OP term T_CP {
				fprintf(fptr,"Print %s\n", $<node->addr>3);
				make_quad("Print", $<node->addr>3, "-", "-");
			};

finalStatements : simple_stmt //basic_stmt
				| cmpd_stmt 
				| func_def ;

simple_stmt
	: basic_stmt next_simple_stmt ;
	
next_simple_stmt
	: end_simple_stmt
	| T_SCln basic_stmt next_simple_stmt ;

end_simple_stmt
	: T_NL
	| T_SCln T_NL ;
	//| ;

cmpd_stmt : //if_stmt {$$ = $1;}
			 while_stmt {$$ = $1;} 
			| for_stmt {$$ = $1;};

/*
if_stmt : T_If bool_exp T_Cln suite elif_stmt optional_else ;

elif_stmt
	: T_Elif bool_exp T_Cln suite elif_stmt
	| ;
*/

optional_else
	: T_Else T_Cln suite
	| ; 

for_stmt
	: T_For T_ID T_IN range_stmt {
		insertRecord("Identifier", $<text>2, @2.first_line, currentScope);
			insertRecord("Identifier", $<text>2, @2.first_line, currentScope); 
			char rangeNodeText[20] ="";
			strcat(rangeNodeText, $<text>2);
			strcat(rangeNodeText, " in range");
			for_code_before_suite(rangeNodeText, argsList);
			clearArgsList(); 
		}
	T_Cln suite	optional_else {for_code_after_suite();}//
	
	| T_For T_ID T_IN T_ID {insertRecord("Identifier", $<text>2, @2.first_line, currentScope);} T_Cln suite optional_else 
		{checkList($<text>4, @4.first_line, currentScope);}
	
	| T_For T_ID T_IN list_stmt {insertRecord("Identifier", $<text>2, @2.first_line, currentScope);} T_Cln suite optional_else {$$ = NULL;};


while_stmt
	: T_While {fprintf(fptr, "L%d:\n",lIndex); 
			make_quad("Label", "-", "-", makeStr(lIndex, 0)); 
		}
		bool_exp { 	
			fprintf(fptr,"If False %s goto L%d\n", $<node->addr>3, lIndex+1); 
			make_quad("If False", $<node->addr>3, "-", makeStr(lIndex+1, 0)); 
		} 
		T_Cln suite {
			fprintf(fptr,"goto L%d\n",lIndex);
			make_quad("goto", "-", "-", makeStr(lIndex, 0));
			fprintf(fptr,"L%d:\n",lIndex+1);
			make_quad("Label", "-", "-", makeStr(lIndex+1, 0)); 
		} optional_else {lIndex += 2;};

range_stmt: T_Range T_OP T_Number T_CP {addToList("0", 1); addToList($<text>3, 0); }//
			| T_Range T_OP T_Number T_Comma T_Number T_CP {addToList($<text>3, 1); addToList($<text>5, 0);}//
			| T_Range T_OP T_Number T_Comma T_Number T_Comma T_Number T_CP  {addToList($<text>3, 1); addToList($<text>5, 0); addToList($<text>7, 0); };//


suite
	: simple_stmt
	| T_NL ID {initNewTable($<depth>2); updateCScope($<depth>2);} finalStatements repeat_stmt DD {updateCScope(currentScope-1); } ;//

repeat_stmt
	: finalStatements repeat_stmt
	| ;//	
	
args : T_ID {insertRecord("Identifier", $<text>1, @1.first_line, currentScope); addToList($<text>1, 1);} args_list {$$ = NULL;}
		| {clearArgsList();};

args_list : T_Comma T_ID {insertRecord("Identifier", $<text>2, @2.first_line, currentScope); addToList($<text>2, 0);} args_list 
			| {addToList("",0); clearArgsList();};

func_def : T_Def T_ID {insertRecord("Func_Name", $<text>2, @2.first_line, currentScope);} T_OP args T_CP T_Cln suite {clearArgsList();} ;

list_stmt: T_OB call_args T_CB {
		 		char* str = (char *)malloc(102*sizeof(char));
			 	strcpy(str,"[");
			 	strcat(str, argsList);
			 	char close[2];
			 	strcpy(close,"]");
			 	strcat(str, close);
			 	fprintf(fptr, "t%d = %s\n", tempNo, str);
			 	$$ = get_node();
			 	$<node->addr>$ = strdup(makeStr(tempNo, 1));
			 	make_quad("=", str, "-", makeStr(tempNo++, 1));
			 	clearArgsList(); 
			 	free(str);
			 };

call_list : T_Comma T_Number {addToList($<text>2, 0);} call_list 
			| T_Comma T_String {addToList($<text>2, 0);} call_list 
			| ;

call_args : T_ID {modifyRecordID("Identifier", $<text>1, @1.first_line, currentScope); addToList($<text>1, 1);} call_list
					| T_Number {addToList($<text>1, 1);} call_list {/*clearArgsList();*/}
					| T_String {addToList($<text>1, 1);} call_list {/*clearArgsList();*/}
					| {}; 

func_call : T_ID {modifyRecordID("Func_Name", $<text>1, @1.first_line, currentScope);} T_OP call_args T_CP ;

%%
void yyerror(const char *msg)
{
	printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.last_column);
	printf("**************************************************************************\n");
	printf("**************************************************************************\n");
}


void for_code_before_suite(char *rtext, char *list)
{
	//temp2 = tempNo;
	int temp = lIndex;
	char* token = strtok(list, ","); 
	char rangeStart[10];
	char rangeEnd[10];
    int flag =0;
    while (token != NULL) { 
    	//printf("token =%s\n",token);
    	if(flag==0)
    	{
    		strcpy(rangeStart,token);
    		flag=1;
    	}
        token = strtok(NULL, ","); 
        if(token!=NULL)
        	strcpy(rangeEnd,token);
    } 
 
    //loop_var = strtok(rtext, " ");
    loop_var[li] = strdup(strtok(rtext, " "));
    
	make_quad("=", rangeStart, "-",  loop_var[li]); //initializing i=0
	fprintf(fptr, "%s = %s\n", loop_var[li], rangeStart);

	make_quad( "Label", "-", "-",makeStr(lIndex, 0));	//everything in the current loop is under a label
	fprintf(fptr, "L%d:\n", lIndex);

	make_quad("<", loop_var[li], rangeEnd, makeStr(tempNo,1)); //t=i<n
	fprintf(fptr, "t%d = %s <%s\n", tempNo, loop_var[li++], rangeEnd);
	
	make_quad("If False", makeStr(tempNo, 1), "-",makeStr(lIndex+1, 0));			
	fprintf(fptr, "If False t%d goto L%d\n", tempNo++, lIndex+1);
	templ = lIndex;
	lIndex+=2;
}

void for_code_after_suite()
{	
	--li;
	int temp = templ;// - 2;//lIndex - 2;
	
	//changed temp2 to tempNo
	fprintf(fptr, "t%d = %s + 1\n", tempNo, loop_var[li]);
	make_quad("+", loop_var[li], "1", makeStr(tempNo,1));
	
	fprintf(fptr, "%s = t%d\n", loop_var[li], tempNo);
	make_quad("=", makeStr(tempNo++,1), "-",  loop_var[li]);
	//make_quad("=", makeStr(tempNo++,1), "-",  loop_var);
	
	fprintf(fptr, "goto L%d\n", temp); //end of loop
	make_quad("goto", "-", "-", makeStr(temp, 0));

	fprintf(fptr, "L%d:\n", temp+1);
	make_quad("Label", "-", "-", makeStr(temp+1, 0)); 
	
	templ = templ - 2;
	//lIndex = lIndex+2;
	return;
}
