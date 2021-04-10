%{
    #include<stdio.h>
    #include<stdlib.h>
    #include <string.h>
	#include <stdarg.h>
	
	#define maxscopey 10000
	extern char* yytext;
	int tempvar=1;
	int labelvar=1;
	char temp[4005];
	char code_temp[5000];
	//FILE* fptr = fopen("icg.txt","w");
	//because size of each scope(100) is 100 elements
	
	typedef struct symtabnode
	{
		char name[100];//has the value of constants
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

	static int searchele(char* name,int scope)
	{
		//printf("\nSEARCH ELE: %s\n",name);
		for(int i=0;i<indexy;++i)
		{
			if (strcmp(name,symtab[i].name)==0)
				{
					symtab[i].scope=scope;
					return 1;
				}
		}
		addtotable(name,scope);
		//printf("\nFINISHED\n");
		//cur_scope=scope;
		return 0;
	}

	void printTable()
	{
		printf("Name\t|Scope\t\n");
		for(int i=0;i<indexy;++i)
			printf("%s\t|%d\t\n",symtab[i].name,symtab[i].scope);
	}  

	void temp_gen(char* name)
	{
		//snprintf(s,n,format,args_to_format) is a function which writes into s, n number of characters(should give space for NULL), the format.
		snprintf(name,5,"t%d",tempvar++);//I have given name atmost 999 temporaries(1 to 999)-t+(1-999) 4 characters+NULL;
		addtotable(name,-1);
	}

	void label_gen(char* name)
	{
		snprintf(name,5,"L%d",labelvar++);
	}

	void makequads(char* s)
	{
		printf("\n****************************\n");
		printf("%s",s);
		printf("\n****************************\n");
	}

%}
%locations

%start start_maro
%union {struct symtabnode* data;char code[500]; char addr[32];int value;
		int start;int end;int step;int leng;
		};//mainly for loop variables
%token T_NL T_IND T_DED T_SAI T_EOF T_EQ T_Comma T_Del T_Pass T_Break T_Continue T_In
%token T_Print T_Import T_From T_Star T_LP T_RP T_Cln T_For T_While T_Or T_Range
%token T_And T_Not T_Lt T_Gt T_Lte T_Gte T_Deq T_Plus T_Minus T_Divide T_Mod T_DDiv
%token T_Power T_Ls T_Rs T_True T_False T_ID T_Integer T_Real T_String

%left T_Plus T_Minus
%left T_Star T_Divide T_DDiv T_Mod
%left T_LP T_RP
%right T_EQ

%%

start_maro
	: start_karo T_EOF {strcpy($<code>$,$<code>1);
		printf("\nAccepted Code:Valid\n\n");printTable();
		makequads($<code>$);}

start_karo
	: T_NL start_karo {strcpy($<code>$,$<code>2);}
	| stmt start_karo {strcat($<code>1,$<code>2);strcpy($<code>$,$<code>1);}
	| %empty {strcpy($<code>$,"");}

term
	: T_String {temp_gen($<addr>$);
				snprintf(code_temp,4200,"%s=%s",$<addr>$,$<data->name>1);
				strncpy($<code>$,code_temp,450);
				$<leng>$=strlen($<data->name>1);
				$<value>$=(strcmp($<data->name>1,"\"\"") && strcmp($<data->name>1,"\'\'"));}

math_term
	: T_ID {
			searchele($<data->name>1,$<data->scope>1);
			printf("ID HERE\n");
			strcpy($<addr>$,$<data->name>1);
			printf("\nDONE\n");
			$<value>$=$<value>1;
			strcpy($<code>$,$<data->name>1);
			}
	| T_Real {
			  temp_gen($<addr>$);
			  snprintf(code_temp,4200,"(%s = %s)\n",$<addr>$,$<data->name>1);
			  strncpy($<code>$,code_temp,450);
			  $<value>$=atoi($<data->name>1);
			  }
	| T_Integer {
				temp_gen($<addr>$);
				snprintf(code_temp,4200,"(%s = %s)\n",$<addr>$,$<data->name>1);
				strncpy($<code>$,code_temp,450);
				$<value>$=atoi($<data->name>1);	
				printf("\nSTRINGH %s \n",$<code>$);			
			  }

stmt
	: simple_stmt {
					strcpy($<code>$,$<code>1);}
	| compound_stmt {
					strcpy($<code>$,$<code>1);}

simple_stmt
	: base_stmt {
				strcpy($<code>$,$<code>1);}

base_stmt
	: pass_stmt {strcpy($<code>$,$<code>1);}
	| delete_stmt {strcpy($<code>$,$<code>1);}
	| import_stmt {strcpy($<code>$,$<code>1);}
	| cobr_stmt {strcpy($<code>$,$<code>1);}
	| assign_stmt {strcpy($<code>$,$<code>1);}
	| print_stmt {strcpy($<code>$,$<code>1);}
	| printable_stmt {strcpy($<code>$,$<code>1);}

pass_stmt
	: T_Pass {strcpy($<code>$,"PASS\n\0");}

delete_stmt
	: T_Del T_ID {strcpy($<code>$,"DELETE\n\0");}

import_stmt
	: T_Import T_ID {searchele($<data->name>2,$<data->scope>2);
					strcpy($<code>$,"IMPORT\n\0");}
	| import_from

import_from
	: T_From T_ID T_Import T_ID end_import_from

end_import_from
	: T_Comma T_ID end_import_from
	| %empty

cobr_stmt
	: T_Break {strcpy($<code>$,"BREAK\n\0");}
	| T_Continue {strcpy($<code>$,"CONTINUE\n\0");}

assign_stmt
	: T_ID T_EQ printable_stmt {
								searchele($<data->name>1,$<data->scope>1);
								printf("\n%s\n",$<addr>3);
								snprintf(temp,3000,"%s=%s\n",$<data->name>1,$<addr>3);
								printf("\nTEMPSTR %s\n",temp);
								$<value>1=$<value>3;
								//printf("\n%s\n",$<data->name>1);
								//generate function
								printf("TEMP %s\n",temp);
								strcat($<code>3,temp);
								strcpy($<code>$,$<code>3);
								$<value>$=$<value>1;

								}

print_stmt
	: T_Print T_LP printable_stmt T_RP {snprintf(code_temp,4200,"PRINT %s\n",$<addr>3);
										strncpy($<code>$,code_temp,450);}

printable_stmt
	: arith_stmt {$<value>$=$<value>1;strcpy($<code>$,$<code>1);strcpy($<addr>$,$<addr>1);}
	| bool_stmt {$<value>$=$<value>1;strcpy($<code>$,$<code>1);strcpy($<addr>$,$<addr>1);}
	| list_stmt {$<value>$=$<value>1;strcpy($<code>$,$<code>1);strcpy($<addr>$,$<addr>1);}

arith_stmt
	: arith_stmt T_Plus arith_stmt {temp_gen($<addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s=%s+%s\n",$<addr>$,$<addr>1,$<addr>3);
									strncpy($<code>$,code_temp,450);
									$<value>$=$<value>1+$<value>3;
									}
	| arith_stmt T_Minus arith_stmt {temp_gen($<addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope
									snprintf(code_temp,4200,"%s=%s-%s\n",$<addr>$,$<addr>1,$<addr>3);
									strncpy($<code>$,code_temp,450);
									$<value>$=$<value>1-$<value>3;
									}
	| arith_stmt T_Star arith_stmt {temp_gen($<addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope
									snprintf(code_temp,4200,"%s=%s*%s\n",$<addr>$,$<addr>1,$<addr>3);
									strncpy($<code>$,code_temp,450);
									$<value>$=$<value>1*$<value>3;
									}
	| arith_stmt T_Divide arith_stmt {temp_gen($<addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s=%s/%s\n",$<addr>$,$<addr>1,$<addr>3);
									strncpy($<code>$,code_temp,450);
									$<value>$=$<value>1/$<value>3;
									}
	| arith_stmt T_DDiv arith_stmt {temp_gen($<addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s=%s//%s\n",$<addr>$,$<addr>1,$<addr>3);
									strncpy($<code>$,code_temp,450);
									$<value>$=$<value>1/$<value>3;
									}
	| arith_stmt T_Mod arith_stmt {temp_gen($<addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s=%s%%%s\n",$<addr>$,$<addr>1,$<addr>3);
									strncpy($<code>$,code_temp,450);
									$<value>$=$<value>1%$<value>3;
									}
	| T_LP arith_stmt T_RP {$<value>$=$<value>2;
							strcpy($<code>$,$<code>2);
							strcpy($<addr>$,$<addr>2);
							}
	| math_term {
				 $<value>$=$<value>1;
				 printf("\nMATH HERE\n");
				 strcpy($<code>$,$<code>1);
				 strcpy($<addr>$,$<addr>1);
				 printf("VAL HERE %s\n",$<code>$);
				 }

bool_stmt
	: bool_term T_Or bool_term {$<value>$=$<value>1||$<value>3;
								temp_gen($<addr>$);
								snprintf(code_temp,4200,"%s=%s OR %s\n",$<addr>$,$<addr>1,$<addr>3);
								strncpy($<code>$,code_temp,450);
								}
	| bool_term T_And bool_term {$<value>$=$<value>1&&$<value>3;
								temp_gen($<addr>$);
								snprintf(code_temp,4200,"%s=%s AND %s\n",$<addr>$,$<addr>1,$<addr>3);
								strncpy($<code>$,code_temp,450);
								}
	| bool_term {$<value>$=$<value>1;
				strcpy($<code>$,$<code>1);
				strcpy($<addr>$,$<addr>1);
				}
	| T_Not bool_stmt {$<value>$=!($<value>2);
					   temp_gen($<addr>$);
					   snprintf(code_temp,4200,"%s=NOT %s\n",$<addr>$,$<addr>2);
					   strncpy($<code>$,code_temp,450);
						}
	| T_LP bool_stmt T_RP {$<value>$=$<value>2;
							strcpy($<code>$,$<code>2);
							strcpy($<addr>$,$<addr>1);
							}
	| arith_stmt comp_op arith_stmt {$<value>$=0;
									temp_gen($<addr>$);
									switch($<value>2)
									{
										case 1:{$<value>$=($<value>1 < $<value>3);break;}
										case 2:{$<value>$=($<value>1 > $<value>3);break;}
										case 3:{$<value>$=($<value>1 == $<value>3);break;}
										case 4:{$<value>$=($<value>1 <= $<value>3);break;}
										case 5:{$<value>$=($<value>1 >= $<value>3);break;}
									}
									snprintf(temp,3000,"%s=%s %s %s\n",$<addr>$,$<addr>1,$<code>2,$<addr>3);
									strncpy($<code>$,code_temp,450);
									}

bool_term
	: term {$<value>$=$<value>1;
			strcpy($<code>$,$<code>1);
			strcpy($<addr>$,$<addr>1);
			}
	| T_True {$<value>$=1;strcpy($<code>$,"TRUE\n");}
	| T_False {$<value>$=0;strcpy($<code>$,"FALSE\n");}

comp_op
	: T_Lt {strcpy($<code>$,"<");$<value>$=1;}
	| T_Gt {strcpy($<code>$,">");$<value>$=2;}
	| T_Deq {strcpy($<code>$,"==");$<value>$=3;}
	| T_Lte {strcpy($<code>$,"<=");$<value>$=4;}
	| T_Gte {strcpy($<code>$,">=");$<value>$=5;}

compound_stmt
	: for_stmt {strcpy($<code>$,$<code>1);}
	| while_stmt {strcpy($<code>$,$<code>1);}

for_stmt
	: T_For T_ID T_In range_stmt T_Cln block_code {searchele($<data->name>2,$<data->scope>2);
													snprintf(code_temp,4200,"%s = %d\n",$<data->name>2,$<start>4);//make the initial value
													strncpy($<code>$,code_temp,450);

													char labelif[10];char labelb[10];//labels for if and true block
													label_gen(labelif);
													label_gen(labelb);

													//print the loop condition
													snprintf(temp,3000,"%s: IF (%s < %d) GOTO %s\n",labelif,$<data->name>2,$<end>4,labelb);
													strcat($<code>$,temp);

													//print the loop block
													snprintf(temp,3000,"%s: %s\n",labelb,$<code>6);
													strcat($<code>$,temp);

													//make the increment function													
													char tempinc[10];
													temp_gen(tempinc);
													snprintf(temp,3000,"%s=%s+%d\n%s=%s\nGOTO %s\n",tempinc,$<data->name>2,$<step>4,$<data->name>2,tempinc,labelif);

													strcat($<code>$,temp);
													}

	| T_For T_ID T_In list_stmt T_Cln block_code {searchele($<data->name>2,$<data->scope>2);
													snprintf(code_temp,4200,"%s\n%s = %s\n",$<code>4,$<data->name>2,$<addr>4);
													strncpy($<code>$,code_temp,450);
													char labelif[10];char labelb[10];//labels for if and true block
													label_gen(labelif);
													label_gen(labelb);

													snprintf(temp,3000,"%s: IF (%s < %d) GOTO %s\n",labelif,$<data->name>2,$<leng>4,labelb);
													strcat($<code>$,temp);

													//print the loop block
													snprintf(temp,3000,"%s: %s\n",labelb,$<code>6);
													strcat($<code>$,temp);

													char tempinc[10];
													temp_gen(tempinc);
													snprintf(temp,3000,"%s=%s+1\n%s=%s\nGOTO %s\n",tempinc,$<data->name>2,$<data->name>2,tempinc,labelif);
													strcat($<code>$,temp);													
													}
	
	| T_For T_ID T_In term T_Cln block_code {searchele($<data->name>2,$<data->scope>2);
													snprintf(code_temp,4200,"%s\n%s = %s\n",$<code>4,$<data->name>2,$<addr>4);
													strncpy($<code>$,code_temp,450);
													char labelif[10];char labelb[10];//labels for if and true block
													label_gen(labelif);
													label_gen(labelb);

													snprintf(temp,3000,"%s: IF (%s < %d) GOTO %s\n",labelif,$<data->name>2,$<leng>4,labelb);
													strcat($<code>$,temp);

													//print the loop block
													snprintf(temp,3000,"%s: %s\n",labelb,$<code>6);
													strcat($<code>$,temp);

													char tempinc[10];
													temp_gen(tempinc);
													snprintf(temp,3000,"%s=%s+1\n%s=%s\nGOTO %s\n",tempinc,$<data->name>2,$<data->name>2,tempinc,labelif);
													strcat($<code>$,temp);													
													}


range_stmt
	: T_Range T_LP T_Integer T_RP {$<start>$=0;$<end>$=$<value>3;$<step>$=1;}
	| T_Range T_LP T_ID T_RP {$<start>$=0;$<end>$=$<value>3;$<step>$=1;}
	| T_Range T_LP T_Integer T_Comma T_Integer T_RP {$<start>$=$<value>3;$<end>$=$<value>5;$<step>$=1;}
	| T_Range T_LP T_ID T_Comma T_ID T_RP {$<start>$=$<value>3;$<end>$=$<value>5;$<step>$=1;}
	| T_Range T_LP T_Integer T_Comma T_Integer T_Comma T_Integer T_RP {$<start>$=$<value>3;$<end>$=$<value>5;$<step>$=$<value>7;}
	| T_Range T_LP T_ID T_Comma T_ID T_Comma T_ID T_RP {$<start>$=$<value>3;$<end>$=$<value>5;$<step>$=$<value>7;}

list_stmt
	: T_Ls T_Rs {$<leng>$=0;
				 $<value>$=0;temp_gen($<addr>$);
				 snprintf(code_temp,4200,"%s = []\n",$<addr>$);
				 strncpy($<code>$,code_temp,450);
				 }
	| T_Ls args T_Rs {$<leng>$=$<leng>2;
					  temp_gen($<addr>$);$<value>$=$<leng>2;
					  snprintf(code_temp,4200,"%s = [%s]\n",$<addr>$,$<code>2);
					  strncpy($<code>$,code_temp,450);
					  }

args
	: T_String items {$<leng>$=$<leng>2+1;snprintf(code_temp,4200,"%s%s",$<code>1,$<code>2);
	strncpy($<code>$,code_temp,450);}
	| T_Real items {$<leng>$=$<leng>2+1;snprintf(code_temp,4200,"%s%s",$<data->name>1,$<code>2);
	strncpy($<code>$,code_temp,450);}
	| T_Integer items {$<leng>$=$<leng>2+1;snprintf(code_temp,4200,"%s%s",$<data->name>1,$<code>2);
	strncpy($<code>$,code_temp,450);}
	| T_ID items{if (searchele($<data->name>1,$<data->scope>1)==0)
			{printf("\nERROR: %s Not Defined\n",yytext);return 0;}
			$<leng>$=$<leng>2+1;snprintf(code_temp,4200,"%s%s",$<data->name>1,$<code>2);
			strncpy($<code>$,code_temp,450);}

items
	: T_Comma T_String items {$<leng>$=$<leng>2+1;snprintf(code_temp,4200,",%s%s",$<data->name>2,$<code>3);
	strncpy($<code>$,code_temp,450);} 
	| T_Comma T_Real items {$<leng>$=$<leng>2+1;snprintf(code_temp,4200,",%s%s",$<data->name>2,$<code>3);
	strncpy($<code>$,code_temp,450);}
	| T_Comma T_Integer items {$<leng>$=$<leng>2+1;snprintf(code_temp,4200,",%s%s",$<data->name>2,$<code>3);
	strncpy($<code>$,code_temp,450);}
	| T_Comma T_ID items {if (searchele($<data->name>2,$<data->scope>2)==0)
						   {printf("\nERROR: %s Not Defined\n",yytext);return 0;}
						    $<leng>$=$<leng>2+1;snprintf(code_temp,4200,",%s%s",$<data->name>2,$<code>3);
							strncpy($<code>$,code_temp,450);}
	| %empty {$<leng>$=0;strcpy($<code>$,"");}

while_stmt
	: T_While bool_stmt T_Cln block_code {char labelif[10];char labelb[10];//labels for if and true block
										  label_gen(labelif);
										  label_gen(labelb);
										  snprintf(code_temp,4200,"%s: IF (%s) GOTO %s\n%s: %s\nGOTO %s\n",labelif,$<code>2,labelb,labelb,$<code>4,labelif);
										  strncpy($<code>$,code_temp,450);
										  }

block_code
	: base_stmt {strcpy($<code>$,$<code>1);}
	| T_NL T_IND stmt repeater T_DED {snprintf(code_temp,4200,"%s%s\n",$<code>3,$<code>4);strncpy($<code>$,code_temp,450);}

repeater
	: stmt repeater {snprintf(code_temp,4200,"%s%s\n",$<code>1,$<code>2);strncpy($<code>$,code_temp,450);}
	| %empty {snprintf(code_temp,4200,"");strncpy($<code>$,code_temp,450);}
%%

