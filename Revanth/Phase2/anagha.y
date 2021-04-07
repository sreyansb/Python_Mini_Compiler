%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h>



	//--------------------------------BASIC VARIABLE DECLARATIONS----------------------------------------
	extern int yylineno;
	extern int depth;
	extern int top();
	extern int pop();
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
			printf("\nIdentifier '%s' at line %d Not Declared\n", name, yylineno);

			//Error Recovery
			insertRecord(type, name, lineNo, scope);

			yyerror("Invalid Python Syntax");
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
				printf("(%d, %d)\t%s\t%s\t%d\t\t%d\n", symbolTables[i].Parent, symbolTables[i].scope-1, symbolTables[i].Elements[j].name, symbolTables[i].Elements[j].type, symbolTables[i].Elements[j].decLineNo,  symbolTables[i].Elements[j].lastUseLine);
			}
		}
	}
	
	void freeAll()
	{
		ICG_opt(); 
		printf("\nAfter dead code elimination");

		printQuads();
		printf("\n");
		printf("\nAfter common sub expression elimination");
		ICG_opt2(); //CSE elimination
		printQuads();
		printf("\n");
		createCSV();
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
	//--------------------------------------------------------------------------------------------------------
	//-------------------------------------AST FUNCTIONS/STRUCTS---------------------------------------------
	//--------------------------------------------------------------------------------------------------------
	int tempCount =0; //for ICG - global var that holds '0', '1' etc:-, 
				      //this val. is appended to each 'tempNo' of each node to make it T0, T1 etc:-
	typedef struct AST
	{ 
	int tempNo; //for ICG
	char* nodeType; //for ICG
	char lexeme[100];
	int NumChild;
	struct AST* left;
	struct AST **child;
	}AST_node;

	struct AST* make_for_node(char* root, char* type, AST_node* child1, AST_node* child2, AST_node* child3, AST_node* child4);
	//struct AST * make_node(char*, char*, AST_node*, AST_node*);
	struct AST* make_leaf(char* root, char* type);
	void AST_print(struct AST *t);

	void AST_print(struct AST *t)
	{
		static int ctr=0;
		//printf("\ninside print tree\n");
		int i;
		//printf("Current node = %s\n", t->lexeme);
		if(t->NumChild==0) 
			return;

		struct AST *t2=t;
		printf("\n%s  -->",t2->lexeme);
		for(i=0;i<t2->NumChild;++i) 
		{
			printf("%s ",t2->child[i]->lexeme);
			
		}
		for(i=0;i<t2->NumChild;++i)
		{
			AST_print(t->child[i]);
		}

				
	}



	struct AST* make_node(char* root, char* type, AST_node* child1, AST_node* child2)
	{
		//printf("Creating new node\n");
		struct AST * node = (struct AST*)malloc(sizeof(struct AST));
		node->child = (struct AST**)malloc(2*sizeof(struct AST *));
		node->NumChild = 2;
		strcpy(node->lexeme,root);
		//printf("Copied lexeme\n");
		//printf("%s\n",node->lexeme);
		//printf("Children = %s  %s\n", child1->lexeme, child2->lexeme);
		node->child[0] = child1;
		node->child[1] = child2;

		//for ICG:
		node->nodeType = (char*)malloc(strlen(type)+1);
		strcpy(node->nodeType,type);
		node->tempNo = tempCount++;
		return node;
	}

	struct AST* make_for_node(char* root, char* type, AST_node* child1, AST_node* child2, AST_node* child3, AST_node* child4)
	{
		//printf("Creating new node\n");
		struct AST * node = (struct AST*)malloc(sizeof(struct AST));
		node->child = (struct AST**)malloc(4*sizeof(struct AST *));
		node->NumChild = 4;
		strcpy(node->lexeme,root);
		node->child[0] = child1;
		node->child[1] = child2;
		node->child[2] = child3;
		node->child[3] = child4;

		//for ICG
		node->nodeType = (char*)malloc(strlen(type)+1);
		strcpy(node->nodeType,type);
		node->tempNo = tempCount++;

		return node;
	}


	struct AST* make_leaf(char* root, char* type)
	{
		//printf("Creating new leaf ");
		struct AST * node = (struct AST*)malloc(sizeof(struct AST));
		strcpy(node->lexeme,root);
		//printf("%s\n",node->lexeme);
		node->NumChild = 0;
		node->child = NULL;

		//for ICG
		node->nodeType = (char*)malloc(strlen(type)+1);
		strcpy(node->nodeType,type);
		node->tempNo = tempCount++;

		return node;
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
		Xitoa(no, A);
		
		if(flag==1) //if it's a variable
		{
				strcpy(tString, "t");
				strcat(tString, A);
				insertRecord("ICGTempVar", tString, -1, 0);
				return tString;
		}
		else //if it's a label
		{
				strcpy(lString, "L");
				strcat(lString, A);
				insertRecord("ICGTempLabel", lString, -1, 0);
				return lString;
		}
	}

	int checkIfBinOperator(char *Op)
	{
		if((!strcmp(Op, "+")) || (!strcmp(Op, "binary-"))|| (!strcmp(Op, "*")) || (!strcmp(Op, "/")) || (!strcmp(Op, ">=")) || (!strcmp(Op, "<=")) || (!strcmp(Op, "<")) || (!strcmp(Op, ">")) || 
			 (!strcmp(Op, "in")) || (!strcmp(Op, "==")) || (!strcmp(Op, "and")) || (!strcmp(Op, "or")))
			{
				return 1;
			}
			
			else 
			{
				return 0;
			}
	}

	void ICG_main(AST_node* node)
	{
		//printf("\n\nIn ICG_main\n");
		if(node == NULL || (!strcmp(node->nodeType, "")))
		{
			return;
		}
		
		if(!strcmp(node->nodeType, "BeginBlock") || !strcmp(node->nodeType, "EndBlock") || !strcmp(node->nodeType, "NewLine") || !strcmp(node->nodeType, "Next"))
		{
			ICG_main(node->child[0]);
			ICG_main(node->child[1]);		
			return;	
		}

		if((!strcmp(node->nodeType, "Identifier")) || (!strcmp(node->nodeType, "Constant")))
		{
			//printf("\n%s", node->nodeType);
			printf("t%d = %s\n", node->tempNo, node->lexeme);
			make_quad("=", node->lexeme,  "-", makeStr(node->tempNo, 1));
			return;
		}

		if(!(strcmp(node->nodeType, "Print")))
		{
			ICG_main(node->child[0]);
			printf("Print t%d\n", node->child[0]->tempNo);
			make_quad("Print", makeStr(node->child[0]->tempNo, 1), "-", "-");
		}

		if(!strcmp(node->nodeType, "import"))
		{
			printf("import %s\n", node->child[0]->lexeme);
			make_quad("import", node->child[0]->lexeme, "-", "-");
			return;
		}
	
		
		if(checkIfBinOperator(node->nodeType)==1)
		{
			ICG_main(node->child[0]);
			ICG_main(node->child[1]);
			char *res = (char*)malloc(10);
			char *arg1 = (char*)malloc(10);
			char *arg2 = (char*)malloc(10);
			
			strcpy(res, makeStr(node->tempNo, 1));
			strcpy(arg1, makeStr(node->child[0]->tempNo, 1));
			strcpy(arg2, makeStr(node->child[1]->tempNo, 1));

			printf("t%d = t%d %s t%d\n", node->tempNo, node->child[0]->tempNo, node->lexeme, node->child[1]->tempNo);
			//printf("\nTEST: node->lexeme =%s", node->lexeme);
			make_quad(node->lexeme, arg1, arg2, res);
			free(arg1);
			free(arg2);
			free(res);
			return;
		}

		if(!strcmp(node->nodeType, "unary-"))
		{
				ICG_main(node->child[1]);
				char *X1 = (char*)malloc(10);
				char *X2 = (char*)malloc(10);
				strcpy(X1, makeStr(node->tempNo, 1));
				strcpy(X2, makeStr(node->child[1]->tempNo, 1));
				printf("t%d = %s t%d\n", node->tempNo, node->nodeType, node->child[1]->tempNo);
				make_quad(node->nodeType , X2, "-", X1);	
			}
			

		

		if(!strcmp(node->nodeType, "="))
		{
			ICG_main(node->child[1]);
			printf("%s = t%d\n", node->child[0]->lexeme, node->child[1]->tempNo);
			make_quad(node->nodeType, makeStr(node->child[1]->tempNo, 1), "-",  node->child[0]->lexeme);
			return;
		}

		if(!strcmp(node->nodeType, "ListIndex"))
		{
			printf("t%d = %s[%s]\n", node->tempNo, node->child[0]->lexeme, node->child[1]->lexeme);
			make_quad("=[]", node->child[0]->lexeme, node->child[1]->lexeme, makeStr(node->tempNo, 1));
			return;
		}

		if(!strcmp(node->nodeType, "ListDecl"))
		{
			//ICG_main(node->child[1]);
			printf("%s = %s\n", node->child[0]->lexeme, node->child[1]->lexeme);
			make_quad(node->nodeType, node->child[1]->lexeme, "-",  node->child[0]->lexeme);
			return;

		}



	
		if((!strcmp(node->nodeType, "IF")))
		{			
			int temp = lIndex;
			ICG_main(node->child[0]);
			printf("If False t%d goto L%d\n", node->child[0]->tempNo, lIndex);
			make_quad("If False",  makeStr(node->child[0]->tempNo, 1), "-", makeStr(temp, 0));
			lIndex++;
			ICG_main(node->child[1]);
			lIndex--;
			printf("L%d: ", temp);
			lIndex++;
			make_quad("Label", "-", "-", makeStr(temp, 0));
			return;
		}

				
		if((!strcmp(node->nodeType, "IF-ELIF-ELSE")))
		{			

			
			ICG_main(node->child[0]);
			ICG_main(node->child[1]);
			
		}
			
		if((!strcmp(node->nodeType, "ELIF")))
		{

			int temp = lIndex;
			ICG_main(node->child[0]);
			printf("If False t%d goto L%d\n", node->child[0]->tempNo, lIndex);
			make_quad("If False", makeStr(node->child[0]->tempNo, 1), "-", makeStr(temp, 0));				
			ICG_main(node->child[1]);
			printf("goto L%d\n", temp+1);
			make_quad("goto", "-", "-", makeStr(temp+1, 0));
			printf("L%d: ", temp);
			make_quad("Label", "-", "-", makeStr(temp, 0));
			ICG_main(node->child[2]);
			printf("L%d: ", temp+1);
			make_quad("Label", "-", "-", makeStr(temp+1, 0));
			lIndex+=2;

		}
		if(!strcmp(node->nodeType, "ELSE"))
		{
			ICG_main(node->child[0]);
			return;
		}

		if(!strcmp(node->nodeType, "WHILE"))
		{
			int temp = lIndex;
			printf("L%d: If False t%d goto L%d\n", lIndex, node->child[0]->tempNo, lIndex+1);
			make_quad( "Label", "-", "-",makeStr(temp, 0));	
			ICG_main(node->child[0]);
				
			make_quad("If False", makeStr(node->child[0]->tempNo, 1), "-",makeStr(temp+1, 0) );								
			lIndex+=2;			
			ICG_main(node->child[1]);
			printf("goto L%d\n", temp);
			make_quad("goto", "-", "-", makeStr(temp, 0));
			//printf("\nHEREEE");
			printf("L%d: ", temp+1);
			make_quad("Label", "-", "-", makeStr(temp+1, 0)); 
			lIndex = lIndex+2;
			return;
		}

		if(!strcmp(node->nodeType, "FOR"))
		{
			int temp = lIndex;

			//printf("%s\n",node->child[0]->nodeType); 
			char* token = strtok(node->child[0]->nodeType, ","); 
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
		    //printf("\nrangeStart =%s rangeEnd =%s",rangeStart,rangeEnd);

		    char *loop_var=strtok(node->child[0]->lexeme, " ");
			make_quad("=", rangeStart, "-",  loop_var); //initializing i=0
			printf("%s = %s\n", loop_var, rangeStart);

			make_quad( "Label", "-", "-",makeStr(temp, 0));	//everything in the current loop is under a label
			printf("L%d: ", lIndex);

			make_quad("<", loop_var, rangeEnd, makeStr(node->child[0]->tempNo,1)); //t=i<n
			printf("t%d= %s<%s\n", node->child[0]->tempNo, loop_var, rangeEnd );
			
			make_quad("If False", makeStr(node->child[0]->tempNo, 1), "-",makeStr(temp+1, 0) );
			//if condition is false, goto exit label				
			printf("If False t%d goto L%d\n", node->child[0]->tempNo, lIndex+1);

			lIndex+=2;			
			ICG_main(node->child[1]);

			//increment loop variable
			printf("t%d = %s + 1\n", node->tempNo, loop_var);
			make_quad("+", loop_var, "1", makeStr(node->tempNo,1));
			printf("%s = t%d\n", loop_var, node->tempNo);
			make_quad("=", makeStr(node->tempNo,1), "-",  loop_var);
			
			printf("goto L%d\n", temp); //end of loop
			make_quad("goto", "-", "-", makeStr(temp, 0));

			printf("L%d: ", temp+1);
			make_quad("Label", "-", "-", makeStr(temp+1, 0)); 

			lIndex = lIndex+2;
			return;


		}

		if(!strcmp(node->nodeType, "Func_Name"))
		{
			printf("Begin Function %s\n", node->child[0]->lexeme);
			make_quad("BeginF", node->child[0]->lexeme, "-", "-");
			ICG_main(node->child[2]);
			printf("End Function %s\n", node->child[0]->lexeme);
			make_quad("EndF", node->child[0]->lexeme, "-", "-");
			return;
		}

		if(!strcmp(node->nodeType, "Func_Call"))
		{
			if(!strcmp(node->child[1]->nodeType, "Void"))
			{
				printf("(t%d)Call Function %s\n", node->tempNo, node->child[0]->lexeme);
				make_quad("Call", node->child[0]->lexeme, "-", makeStr(node->tempNo, 1));
			}
			else
			{
				char A[10];
				//printf("TEST: node->child[1]->lexeme = %s\n", node->child[1]->lexeme);

				char* token = strtok(node->child[1]->lexeme, ","); 
				  				int i = 0;
				while (token != NULL) 
				{
						i++; 
				    printf("Push Param %s\n", token);
				    make_quad("Param", token, "-", "-"); 
				    token = strtok(NULL, ","); 
				}
				
				printf("(t%d)Call Function %s, %d\n", node->tempNo, node->child[0]->lexeme, i);
				sprintf(A, "%d", i);
				make_quad("Call", node->child[0]->lexeme, A, makeStr(node->tempNo, 1));
				printf("Pop Params for Function %s, %d\n", node->child[0]->lexeme, i);
				strcpy(A, "");				
				return;
			}
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
		filename = "TAC.tsv";
		fp=fopen(filename,"w+");
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

		printf("\n %sfile created",filename);
	}

	



%}


%locations

%union { char *text; int depth; struct AST *node; };
%type <node> StartParse StartDebugger args start_suite suite end_suite finalStatements arith_exp bool_exp term constant basic_stmt cmpd_stmt func_def list_index import_stmt pass_stmt break_stmt print_stmt if_stmt elif_stmts else_stmt while_stmt return_stmt assign_stmt bool_term bool_factor for_stmt func_call call_args list_stmt


%token T_EndOfFile    T_Cln T_NL  T_IN T_NEQ T_EQ T_GT T_LT T_EGT T_ELT T_Or T_And  ID ND DD T_String Trip_Quote  T_Import   T_MN T_PL T_DV T_ML T_OP T_CP T_OB T_CB T_Def T_Comma T_Range T_List

%token <text> T_Number T_ID T_EQL T_LEN T_True T_False T_Not T_Pass T_Break T_Return T_Print T_If T_Elif T_Else T_For T_While

%right T_EQL 
%left T_PL T_MN
%left T_ML T_DV
%left T_OP T_CP
%nonassoc T_If
%nonassoc T_Elif
%nonassoc T_Else

 
 

%%
StartDebugger : {init();} StartParse T_EndOfFile {printf("\nValid Python Syntax\n" ); printf("**************************************************************************\n");
printf("\n\n*********************************AST***********************************"); AST_print($2); printf("\n**************************************************************************\n");
printf("\n\n***********************************ICG***********************************\n"); ICG_main($2);
printf("**************************************************************************\n");
printQuads(); printSTable(); freeAll();  exit(0);} ;

constant : T_Number {insertRecord("Constant", $<text>1, @1.first_line, currentScope);
					  $$ = make_leaf($<text>1, "Constant");}
		 | T_String {insertRecord("Constant", $<text>1, @1.first_line, currentScope);
		 			  $$ = make_leaf($<text>1, "Constant");};



list_index : T_ID T_OB T_Number T_CB {
									  checkList($<text>1, @1.first_line, currentScope);
									  $$ = make_node("ListIndex", "ListIndex", make_leaf($<text>1, "ListTypeID"), make_leaf($3, "Constant"));
									  };


term : T_ID { modifyRecordID("Identifier", $<text>1, @1.first_line, currentScope); 
		$$ = make_leaf($<text>1,"Identifier");} 
     | constant {$$ = $1;} 
     | list_index {$$ = $1;};

StartParse : T_NL StartParse {$$=$2;}
			| finalStatements T_NL {resetDepth();} StartParse {$$ = make_node("Start","NewLine", $1, $4);} 
			| finalStatements {$$=$1;};

basic_stmt : pass_stmt {$$=$1;} 
			| break_stmt {$$=$1;} 
			| import_stmt {$$=$1;} 
			| assign_stmt {$$=$1;} 
			| arith_exp {$$=$1;} 
			| bool_exp {$$=$1;} 
			| print_stmt {$$=$1;}
			| return_stmt {$$=$1;}
			| func_call {$$=$1;} ; 

arith_exp :  term { $$=$1;}
			| arith_exp  T_PL  arith_exp {$$ = make_node("+","+",$1, $3); }
			| arith_exp  T_MN  arith_exp {$$ = make_node("binary-","binary-",$1, $3);}
			| arith_exp  T_ML  arith_exp {$$ = make_node("*","*",$1, $3);}
 			| arith_exp  T_DV  arith_exp {$$ = make_node("/","/",$1, $3);}
 			| T_MN term {$$ = make_node("temp","unary-", make_leaf("-", ""), $2);}
 			| T_OP arith_exp T_CP {$$=$2;};

		    



bool_exp : bool_term T_Or bool_term {$$ = make_node("or", "or", $1, $3);}
		 | arith_exp T_Or arith_exp {$$ = make_node("or","or", $1, $3);}
         | arith_exp T_LT arith_exp {$$ = make_node("<", "<",$1, $3);}
         | bool_term T_And bool_term {$$ = make_node("and","and", $1, $3);}
         | arith_exp T_And arith_exp {$$ = make_node("and","and", $1, $3);}
         | arith_exp T_GT arith_exp {$$ = make_node(">",">",$1, $3);}
         | arith_exp T_ELT arith_exp {$$ = make_node("<=","<=", $1, $3);}
         | arith_exp T_EGT arith_exp {$$ = make_node(">=",">=",$1, $3);}
         | arith_exp T_IN T_ID {checkList($<text>3, @3.first_line, currentScope); 
         $$ = make_node("in", "in", $1, make_leaf($<text>3, "Constant"));}
         | bool_term {$$=$1;}; 

bool_term : bool_factor {$$ = $1;}
          | arith_exp T_EQ arith_exp {$$ = make_node("==", "==",$1, $3);}
          | T_True {insertRecord("Constant", "True", @1.first_line, currentScope); $$ = make_leaf("True", "Constant");}
          | T_False {insertRecord("Constant", "False", @1.first_line, currentScope); $$ = make_leaf("False", "Constant");}; 
          
bool_factor : T_Not bool_factor {$$ = make_node("not", "not", make_leaf("", ""), $2);}
            | T_OP bool_exp T_CP {$$ = $2;};

import_stmt : T_Import T_ID {insertRecord("PackageName", $<text>2, @2.first_line, currentScope); 
							  $$ = make_node("import", "import", make_leaf($<text>2, "PackageName"), make_leaf("", "") );};

pass_stmt   : T_Pass {$$=make_leaf("pass", "pass");};
break_stmt  : T_Break {$$=make_leaf("break", "break");};
return_stmt : T_Return {$$=make_leaf("", "return");} | T_Return T_ID { char return_val[100]; strcpy(return_val, "return "); strcat(return_val, $<text>2); $$=make_leaf(return_val, "return");}; 

assign_stmt : 
			T_ID  T_EQL func_call
			 {
			 	insertRecord("Identifier", $<text>1, @1.first_line, currentScope);
			 	$$ = make_node("=", "=",make_leaf($<text>1, "Identifier"), $3);
			 }

			 |T_ID T_EQL bool_exp 
			 {
			 	insertRecord("Identifier", $<text>1, @1.first_line, currentScope);
			 	$$ = make_node("=","=", make_leaf($<text>1, "Identifier"), $3);
			 }   
			 | 
			 T_ID T_EQL arith_exp 
			{
			  insertRecord("Identifier", $<text>1, @1.first_line, currentScope); 
			  $$=make_node("=","=",make_leaf($<text>1, "Identifier"),$3);
			  }  
			 |T_ID T_EQL list_stmt 
			 {
			 	insertRecord("ListTypeID", $<text>1, @1.first_line, currentScope);
			 	$$ = make_node("=", "ListDecl", make_leaf($<text>1, "ListTypeID"), $3);
			 } 

			 ;
	      
print_stmt : T_Print T_OP term T_CP {$$=make_node("Print", "Print", $3, make_leaf("", ""));};

finalStatements : basic_stmt  
				| cmpd_stmt 
				| func_def 
				;

cmpd_stmt : if_stmt {$$ = $1;} 
			| while_stmt {$$ = $1;}
			| for_stmt {$$ = $1;};


if_stmt : T_If bool_exp T_Cln start_suite {$$= make_node("IF", "IF", $2, $4);}  
		| T_If bool_exp T_Cln start_suite elif_stmts 
		{$$= make_node("IF-ELIF-ELSE", "IF-ELIF-ELSE", make_node("IF","IF", $2, $4), $5);}  ;


elif_stmts : else_stmt{$$= $1;} 
			| T_Elif bool_exp T_Cln start_suite elif_stmts {$$ = make_for_node("ELIF", "ELIF", $2, $4, $5, make_leaf("",""));} ;

else_stmt : T_Else T_Cln start_suite {$$ = make_node("ELSE","ELSE", $3, make_leaf("", ""));} ;

for_stmt: T_For T_ID T_IN range_stmt T_Cln start_suite 
			{
				insertRecord("Identifier", $<text>2, @2.first_line, currentScope); 
				char rangeNodeText[20] ="";
				strcat(rangeNodeText, $<text>2);
				strcat(rangeNodeText, " in range");
				//printf("%\n%s", rangeNodeText);
				$$ =make_node("FOR", "FOR", make_leaf(rangeNodeText, argsList), $6);  
				clearArgsList(); 
			}
		 | T_For T_ID T_IN T_ID T_Cln start_suite {
		 insertRecord("Identifier", $<text>2, @2.first_line, currentScope); 
		 //printSTable();
		 checkList($<text>4, @4.first_line, currentScope);
		 $$ =make_node("FOR", "FOR", make_leaf($<text>4, "Identifier"), $6);} ; 

while_stmt : T_While bool_exp T_Cln start_suite {$$ =make_node("WHILE","WHILE", $2, $4);}; 

range_stmt: T_Range T_OP T_Number T_CP {addToList("0", 1); addToList($<text>3, 0); }
			| T_Range T_OP T_Number T_Comma T_Number T_CP {addToList($<text>3, 1); addToList($<text>5, 0);}
			| T_Range T_OP T_Number T_Comma T_Number T_Comma T_Number T_CP {addToList($<text>3, 1); addToList($<text>5, 0);};
start_suite : basic_stmt {$$=$1;} 
			| T_NL ID { initNewTable($<depth>2); updateCScope($<depth>2);} finalStatements suite {$$ =make_node("start_suite", "BeginBlock", $4, $5);};

suite : T_NL ND finalStatements suite {$$ = make_node("Next", "Next", $3, $4);}
		| T_NL end_suite {$$ = $2;};

end_suite : DD {updateCScope($<depth>1);} finalStatements {$$ = make_node("", "EndBlock", $3, make_leaf("", ""));}
		|{$$ = make_leaf("", ""); resetDepth();};

args : T_ID {insertRecord("Identifier", $<text>1, @1.first_line, currentScope); addToList($<text>1, 1);}
 args_list {$$ = make_leaf(argsList, "argsList"); clearArgsList();} 
     | {$$ = make_leaf("", "Void"); clearArgsList();};

args_list : T_Comma T_ID {insertRecord("Identifier", $<text>2, @2.first_line, currentScope); addToList($<text>2, 0);} args_list 
			| {addToList("",0); clearArgsList();};

func_def : T_Def T_ID {insertRecord("Func_Name", $<text>2, @2.first_line, currentScope);} T_OP args
 T_CP T_Cln start_suite {clearArgsList(); $$ = make_for_node("Func_def", "Func_Name", make_leaf($<text>2, "Func_Name"), $5, $8, make_leaf("",""));};

list_stmt: T_OB T_CB { $$ = make_leaf("[]", ""); } 
		 |	
			 	T_OB call_args T_CB {
			 	char* str = (char *)malloc(102*sizeof(char));
			 	strcpy(str,"[");
			 	strcat(str, argsList);
			 	char close[2];
			 	strcpy(close,"]");
			 	strcat(str, close);
			 	$$ = make_leaf(str, ""); 
			 	clearArgsList(); 
			 	free(str);};

call_list : T_Comma term {addToList($2->lexeme, 0);} call_list | ;

call_args : T_ID {modifyRecordID("Identifier", $<text>1, @1.first_line, currentScope); addToList($<text>1, 1);} call_list {$$ = make_leaf(argsList,"argsList"); }
					| T_Number {addToList($<text>1, 1);} call_list {$$ = make_leaf(argsList,"argsList"); clearArgsList();}
					| T_String {addToList($<text>1, 1);} call_list {$$ = make_leaf(argsList,"argsList"); clearArgsList();}	
					| {$$ = make_leaf("","Void");};

func_call : T_ID {modifyRecordID("Func_Name", $<text>1, @1.first_line, currentScope);}T_OP call_args T_CP {$$ = make_node("Func_Call", "Func_Call",make_leaf($<text>1, "Func_Name"), $4);};
 
%%

void yyerror(const char *msg)
{
	//printSTable();
	//printf("\n\n%s", msg);
	printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.last_column);
	printf("**************************************************************************\n");
	//printSTable();
	printf("**************************************************************************\n");
	//exit(0);
}
