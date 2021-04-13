%{
    #include<stdio.h>
    #include<stdlib.h>
    #include <string.h>
	#include <stdarg.h>
	#include <ctype.h>
	
	#define maxscopey 10000
	extern char* yytext;
	int tempvar=1;
	int labelvar=1;
	char temp[7000];
	char code_temp[5000];
	//FILE* fptr = fopen("icg.txt","w");
	//because size of each scope(100) is 100 elements
	
	typedef struct symtabnode
	{
		char name[100];//has the value of constants
		int scope;
	}symtabnode;

	typedef struct nodeyacc
	{
		int value;
		int start;
		int end;
		int step;
		int leng;
		char addr[32];
		char code[2000];
	}nodeyacc;
	
	//will store all the elements and their scopes
	symtabnode symtab[maxscopey];
	int indexy=0;//first index to be filled in the symbol table

	void addtotable(char* name,int scope)
	{
		strcpy(symtab[indexy].name,name);
		symtab[indexy].scope=scope;
		++indexy;
	}

	static int searchdo(char* name)
	{
		for(int i=0;i<indexy;++i)
		{
			if (strcmp(name,symtab[i].name)==0)
				{
					return 1;
				}
		}
		return 0;
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

	void slicestr(char*s,char* res,int start,int end)
	{
		int indexs=0;
		while(start+indexs<end)
			{res[indexs]=s[start+indexs];++indexs;}
		res[indexs]='\0';
	}

	int check(char s)
	{
		if (s=='='||s=='+'||s=='-'||s=='*'||s=='/'||s=='%'||s=='<'||s=='>')
			return 1;
		return 0;
	}

	void makequads(char* s)//based off many assumptions
	{
		FILE* fp=fopen("outputs/evalCSEfor.txt","w");
		fprintf(fp,"%s",s);
		fclose(fp);
		FILE* fptr=fopen("outputs/evalCSEfor.tsv","w");
		fprintf(fptr,"#\top\tA1\tA2\tRes\n");
		int linenoq=1;
		int index=0;
		char temp[501];
		char temp1[501];
		char temp2[501];

		while(s[index])
		{
			while(s[index]=='\n'||s[index]==' ')
				index++;
			if (s[index]=='L' && s[index+1] && s[index+1]>47 && s[index+1]<58)//L1 : 
				{
					int endindex=index+1;
					while(s[endindex]!=' ')
						++endindex;
					slicestr(s,temp,index,endindex);
					fprintf(fptr,"%d\tLABEL\t-\t-\t%s\n",linenoq,temp);
					++linenoq;
					index=endindex+2;
				}
			else if (s[index]=='G' && s[index+1] && s[index+2] && s[index+3] && s[index+1]=='O' && s[index+2]=='T' && s[index+3]=='O') // GOTO
			{
				index=index+5;
				int endindex=index;
				while(s[endindex]!='\n')
					++endindex;
				slicestr(s,temp,index,endindex);
				fprintf(fptr,"%d\tGOTO\t-\t-\t%s\n",linenoq,temp);
				++linenoq;
				index=endindex;
			}
			else if (s[index]=='I' && ((s[index+1] && s[index+1]=='F')||((s[index+1]&&s[index+2]&&s[index+3]&&s[index+4]&&s[index+5])&&(s[index+1]=='M'&&s[index+2]=='P'&&s[index+3]=='O'&&s[index+4]=='R'&&s[index+5]=='T'))))
			{
				if (s[index+1]=='F')
				{
					//printf("\nHERE\n");
					int endindex=index+4;
					while(s[endindex]!=')')
						++endindex;
					slicestr(s,temp,index+4,endindex);
					index=endindex+7;
					endindex+=7;
					while(s[endindex]!='\n')
						++endindex;
					slicestr(s,temp1,index,endindex);
					fprintf(fptr,"%d\tIF\t%s\t-\t%s\n",linenoq,temp,temp1);
					++linenoq;
					index=endindex;
				}
				else//for import
				{
					int endindex=index+7;
					while(s[endindex]!='\n')
						++endindex;
					slicestr(s,temp,index+7,endindex);
					//printf("\nHERE IMPORT %s\n",temp);
					fprintf(fptr,"%d\tIMPORT\t%s\t-\t-\n",linenoq,temp);
					++linenoq;
					index=endindex;
				}
			}
			else if (s[index]=='P'&&s[index+1]&&s[index+2]&&s[index+3]&&s[index+4]&&s[index+1]=='R'&&s[index+2]=='I'&&s[index+3]=='N'&&s[index+4]=='T')//for print statement
			{
				int endindex=index+6;
				while(s[endindex]!='\n')
					++endindex;
				slicestr(s,temp,index+6,endindex);
				fprintf(fptr,"%d\tPRINT\t%s\t-\t-\n",linenoq,temp);
				++linenoq;
				index=endindex;
			}
			else if (isalpha(s[index])||s[index]=='_')
			{
				int endindex=index;
				while(s[endindex]!='\n' && s[endindex]!='=')
					++endindex;
				if (s[endindex]=='\n')
					{
						index=endindex;
						continue;
					}
				slicestr(s,temp,index,endindex);//temp holds the name of the variable
				++endindex;index=endindex;
				int f=1;
				while(s[endindex]!='\n')
				{
					if (check(s[endindex]))
						{f=0;break;}
					endindex++;
				}
				if (f)
				{
					slicestr(s,temp1,index,endindex);
					fprintf(fptr,"%d\t=\t%s\t-\t%s\n",linenoq,temp1,temp);
					++linenoq;
					index=endindex;
				}
				else
				{
					endindex=index;
					while(!(check(s[endindex])))
						++endindex;
					slicestr(s,temp1,index,endindex);//holds the first arg
					char op[3];int i=0;
					while(check(s[endindex]))
						{op[i]=s[endindex];i++;endindex++;}
					op[i]='\0';
					index=endindex;
					while(s[endindex]!='\n')
						++endindex;
					slicestr(s,temp2,index,endindex);
					fprintf(fptr,"%d\t%s\t%s\t%s\t%s\n",linenoq,op,temp1,temp2,temp);
					++linenoq;
					index=endindex;
				}
			}
			else
			{
				while(s[index] && s[index]!='\n')
					index++;
			}
		}
		fclose(fptr);
		
	}

%}
%locations

%start start_maro
%union {struct symtabnode* data;struct nodeyacc* node;
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
	: start_karo {printf("\nBIG HERE\n");
						$<node>$=malloc(sizeof(struct nodeyacc));
                        strcpy($<node->code>$,$<node->code>1);
                        printf("\nAccepted Code : Valid\n\n");printTable();
                        makequads($<node->code>$);exit(0);} T_EOF

start_karo
	: T_NL start_karo {$<node>$=malloc(sizeof(struct nodeyacc));
						//strcat($<node->code>2,$<node->code>3);
                        strcpy($<node->code>$,$<node->code>2);
						//printf("\nENDL\n%s\nFIN\n",$<node->code>$);
						}
	| stmt start_karo {
					   $<node>$=malloc(sizeof(struct nodeyacc));
                       strcat($<node->code>1,$<node->code>2);
					   strcpy($<node->code>$,$<node->code>1);
					   //printf("\nWENDL\n%s\nFIN\n",$<node->code>$);
                    	}
	| %empty {$<node>$=malloc(sizeof(struct nodeyacc));
			  strcpy($<node->code>$,"");
			  printf("\nEND\n");
              }

term
	: T_String {$<node>$=malloc(sizeof(struct nodeyacc));
                temp_gen($<node->addr>$);
				snprintf(code_temp,4200,"%s=%s\n",$<node->addr>$,$<data->name>1);
				strcpy($<node->code>$,code_temp);
				//strcpy($<node->code>$,code_temp);
				$<node->leng>$=strlen($<data->name>1);
				$<node->value>$=(strcmp($<data->name>1,"\"\"") && strcmp($<data->name>1,"\'\'"));
				}

math_term
	: T_ID { 
			searchele($<data->name>1,$<data->scope>1);
            $<node>$=malloc(sizeof(struct nodeyacc));
			strcpy($<node->addr>$,$<data->name>1);
			$<node->value>$=$<node->value>1;
			strcpy($<node->code>$,$<data->name>1);
			
			}
	| T_Real {$<node>$=malloc(sizeof(struct nodeyacc));
			  $<node->value>$=atoi($<data->name>1);
			  temp_gen($<node->addr>$);
			  snprintf(code_temp,2000,"%s=%s\n",$<node->addr>$,$<data->name>1);
			  strcpy($<node->code>$,code_temp);
			  //printf("\nT_REAL\n %s\n",$<node->code>$);
			  }
	| T_Integer {
				$<node>$=malloc(sizeof(struct nodeyacc));
			  	$<node->value>$=atoi($<data->name>1);
			  	temp_gen($<node->addr>$);
			  	snprintf(code_temp,2000,"%s=%s\n",$<node->addr>$,$<data->name>1);
			  	strcpy($<node->code>$,code_temp);
				//printf("\nT_INT\n %s\n",$<node->code>$);
			  }

stmt
	: simple_stmt { $<node>$=malloc(sizeof(struct nodeyacc));
					strcpy($<node->code>$,$<node->code>1);}
	| compound_stmt {$<node>$=malloc(sizeof(struct nodeyacc));
					strcpy($<node->code>$,$<node->code>1);
					//printf("\nIN HERE\n%s\nFINISH COMPD\n",$<node->code>$);
					}

simple_stmt
	: base_stmt {
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,$<node->code>1);
				}

base_stmt
	: pass_stmt {
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,$<node->code>1);
				}
	| delete_stmt {
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,$<node->code>1);
				}
	| import_stmt {
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,$<node->code>1);
				}
	| cobr_stmt {
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,$<node->code>1);
				}
	| assign_stmt {
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,$<node->code>1);
				}
	| print_stmt {
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,$<node->code>1);
				}
	| printable_stmt {
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,$<node->code>1);
				}

pass_stmt
	: T_Pass {
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,"PASS\n");
			}

delete_stmt
	: T_Del T_ID {
				if (searchele($<data->name>2,$<data->scope>2)==0)
					{printf("Invalid Code\n");exit(0);}
				$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,"DELETE\n\0");
			}

import_stmt
	: T_Import T_ID {
					searchele($<data->name>2,$<data->scope>2);
					$<node>$=malloc(sizeof(struct nodeyacc));
					snprintf(code_temp,120,"IMPORT %s\n\0",$<data->name>2);
					strcpy($<node->code>$,code_temp);
					}
	| import_from

import_from
	: T_From T_ID T_Import T_ID end_import_from

end_import_from
	: T_Comma T_ID end_import_from
	| %empty

cobr_stmt
	: T_Break {$<node>$=malloc(sizeof(struct nodeyacc));
				strcpy($<node->code>$,"BREAK\n\0");
			  }
	| T_Continue {$<node>$=malloc(sizeof(struct nodeyacc));
				  strcpy($<node->code>$,"CONTINUE\n\0");
			    }

assign_stmt
	: T_ID T_EQ printable_stmt {
								searchele($<data->name>1,$<data->scope>1);
								$<node>$=malloc(sizeof(struct nodeyacc));
								snprintf(temp,3000,"\n%s=%s\n",$<data->name>1,$<node->addr>3);//generate function
								strcat($<node->code>3,temp);
								strcpy($<node->code>$,$<node->code>3);
								$<node->value>1=$<node->value>3;
								$<node->value>$=$<node->value>3;
								}

print_stmt
	: T_Print T_LP printable_stmt T_RP {
										snprintf(code_temp,4200,"\nPRINT %s\n",$<node->addr>3);
										$<node>$=malloc(sizeof(struct nodeyacc));
										strcat($<node->code>3,code_temp);
										strcpy($<node->code>$,$<node->code>3);
									   }

printable_stmt
	: arith_stmt {
				  $<node>$=malloc(sizeof(struct nodeyacc));
				  $<node->value>$=$<node->value>1;
				  strcpy($<node->code>$,$<node->code>1);
				  strcpy($<node->addr>$,$<node->addr>1);
				  }
	| bool_stmt {$<node>$=malloc(sizeof(struct nodeyacc));
				  $<node->value>$=$<node->value>1;
				  strcpy($<node->code>$,$<node->code>1);
				  strcpy($<node->addr>$,$<node->addr>1);
				  }
	| list_stmt {$<node>$=malloc(sizeof(struct nodeyacc));
				  $<node->value>$=$<node->value>1;
				  strcpy($<node->code>$,$<node->code>1);
				  strcpy($<node->addr>$,$<node->addr>1);
				  }

arith_stmt
	: arith_stmt T_Plus arith_stmt {$<node>$=malloc(sizeof(struct nodeyacc));
									temp_gen($<node->addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s\n%s\n%s=%s+%s\n",$<node->code>1,$<node->code>3,$<node->addr>$,$<node->addr>1,$<node->addr>3);
									strcpy($<node->code>$,code_temp);
									$<node->value>$=$<node->value>1+$<node->value>3;
									}
	| arith_stmt T_Minus arith_stmt {$<node>$=malloc(sizeof(struct nodeyacc));
									temp_gen($<node->addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope
									snprintf(code_temp,4200,"%s\n%s\n%s=%s-%s\n",$<node->code>1,$<node->code>3,$<node->addr>$,$<node->addr>1,$<node->addr>3);
									strcpy($<node->code>$,code_temp);
									$<node->value>$=$<node->value>1-$<node->value>3;
									}
	| arith_stmt T_Star arith_stmt {$<node>$=malloc(sizeof(struct nodeyacc));temp_gen($<node->addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope
									snprintf(code_temp,4200,"%s\n%s\n%s=%s*%s\n",$<node->code>1,$<node->code>3,$<node->addr>$,$<node->addr>1,$<node->addr>3);
									strcpy($<node->code>$,code_temp);
									$<node->value>$=$<node->value>1*$<node->value>3;
									}
	| arith_stmt T_Divide arith_stmt {$<node>$=malloc(sizeof(struct nodeyacc));temp_gen($<node->addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s\n%s\n%s=%s/%s\n",$<node->code>1,$<node->code>3,$<node->addr>$,$<node->addr>1,$<node->addr>3);
									strcpy($<node->code>$,code_temp);
									$<node->value>$=$<node->value>1/$<node->value>3;
									}
	| arith_stmt T_DDiv arith_stmt {$<node>$=malloc(sizeof(struct nodeyacc));temp_gen($<node->addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s\n%s\n%s=%s//%s\n",$<node->code>1,$<node->code>3,$<node->addr>$,$<node->addr>1,$<node->addr>3);
									strcpy($<node->code>$,code_temp);
									$<node->value>$=$<node->value>1/$<node->value>3;
									}
	| arith_stmt T_Mod arith_stmt {$<node>$=malloc(sizeof(struct nodeyacc));temp_gen($<node->addr>$);//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s\n%s\n%s=%s%%%s\n",$<node->code>1,$<node->code>3,$<node->addr>$,$<node->addr>1,$<node->addr>3);
									strcpy($<node->code>$,code_temp);
									$<node->value>$=$<node->value>1%$<node->value>3;
									}
	| T_LP arith_stmt T_RP {$<node>$=malloc(sizeof(struct nodeyacc));
							$<node->value>$=$<node->value>2;
							strcpy($<node->code>$,$<node->code>2);
							strcpy($<node->addr>$,$<node->addr>2);
							}
	| math_term {
				 
				 $<node>$=malloc(sizeof(struct nodeyacc));
				 $<node->value>$=$<node->value>1;
				 strcpy($<node->code>$,$<node->code>1);
				 strcpy($<node->addr>$,$<node->addr>1);
				 }

bool_stmt
	: bool_term T_Or bool_term {$<node>$=malloc(sizeof(struct nodeyacc));
								$<node->value>$=$<node->value>1||$<node->value>3;
								temp_gen($<node->addr>$);
								snprintf(code_temp,4200,"%s=%s OR %s\n",$<node->addr>$,$<node->addr>1,$<node->addr>3);
								strcpy($<node->code>$,code_temp);
								}
	| bool_term T_And bool_term {$<node>$=malloc(sizeof(struct nodeyacc));
								$<node->value>$=$<node->value>1&&$<node->value>3;
								temp_gen($<node->addr>$);
								snprintf(code_temp,4200,"%s=%s AND %s\n",$<node->addr>$,$<node->addr>1,$<node->addr>3);
								strcpy($<node->code>$,code_temp);
								}
	| bool_term {$<node>$=malloc(sizeof(struct nodeyacc));
				$<node->value>$=$<node->value>1;
				strcpy($<node->code>$,$<node->code>1);
				strcpy($<node->addr>$,$<node->addr>1);
				}
	| T_Not bool_stmt {$<node>$=malloc(sizeof(struct nodeyacc));
						$<node->value>$=!($<node->value>2);
					   temp_gen($<node->addr>$);
					   snprintf(code_temp,4200,"%s=NOT(%s)\n",$<node->addr>$,$<node->addr>2);
					   strcpy($<node->code>$,code_temp);
					   //strncpy($<code>$,code_temp,1999);
						}
	| T_LP bool_stmt T_RP { $<node>$=malloc(sizeof(struct nodeyacc));
							$<node->value>$=($<node->value>2);
							strcpy($<node->code>$,$<node->code>2);
							strcpy($<node->addr>$,$<node->addr>1);
							}
	| arith_stmt comp_op arith_stmt {$<node>$=malloc(sizeof(struct nodeyacc));
									$<node->value>$=0;
									temp_gen($<node->addr>$);
									switch($<node->value>2)
									{
										case 1:{$<node->value>$=($<node->value>1 < $<node->value>3);break;}
										case 2:{$<node->value>$=($<node->value>1 > $<node->value>3);break;}
										case 3:{$<node->value>$=($<node->value>1 == $<node->value>3);break;}
										case 4:{$<node->value>$=($<node->value>1 <= $<node->value>3);break;}
										case 5:{$<node->value>$=($<node->value>1 >= $<node->value>3);break;}
									}
									snprintf(temp,3000,"%s\n%s\n%s=%s%s%s\n",$<node->code>1,$<node->code>3,$<node->addr>$,$<node->addr>1,$<node->code>2,$<node->addr>3);
									strcpy($<node->code>$,temp);
									}

bool_term
	: term {
			$<node>$=malloc(sizeof(struct nodeyacc));
			$<node->value>$=$<node->value>1;
			strcpy($<node->code>$,$<node->code>1);
			//strcpy($<code>$,$<code>1);
			strcpy($<node->addr>$,$<node->addr>1);
			
			}
	| T_True {$<node>$=malloc(sizeof(struct nodeyacc));$<node->value>$=1;strcpy($<node->code>$,"TRUE\0");strcpy($<node->addr>$,"TRUE\0");}
	| T_False {$<node>$=malloc(sizeof(struct nodeyacc));$<node->value>$=0;strcpy($<node->code>$,"FALSE\0");strcpy($<node->addr>$,"FALSE\0");}

comp_op
	: T_Lt {$<node>$=malloc(sizeof(struct nodeyacc));strcpy($<node->code>$,"<\0");$<node->value>$=1;}
	| T_Gt {$<node>$=malloc(sizeof(struct nodeyacc));strcpy($<node->code>$,">\0");$<node->value>$=2;}
	| T_Deq {$<node>$=malloc(sizeof(struct nodeyacc));strcpy($<node->code>$,"==\0");$<node->value>$=3;}
	| T_Lte {$<node>$=malloc(sizeof(struct nodeyacc));strcpy($<node->code>$,"<=\0");$<node->value>$=4;}
	| T_Gte {$<node>$=malloc(sizeof(struct nodeyacc));strcpy($<node->code>$,">=\0");$<node->value>$=5;}

list_stmt
	: T_Ls T_Rs {$<node>$=malloc(sizeof(struct nodeyacc));
				 $<node->leng>$=0;
				 $<node->value>$=0;
				 temp_gen($<node->addr>$);
				 snprintf(code_temp,4200,"%s=[]\n",$<node->addr>$);
				 strcpy($<node->code>$,code_temp);
				 }
	| T_Ls args T_Rs {$<node>$=malloc(sizeof(struct nodeyacc));
					  $<node->leng>$=$<node->leng>2;
					  temp_gen($<node->addr>$);
					  $<node->value>$=$<node->leng>2;
					  snprintf(code_temp,4200,"%s=addr([%s])\n",$<node->addr>$,$<node->code>2);
					  strcpy($<node->code>$,code_temp);
					  }

args
	: T_String items {
						$<node>$=malloc(sizeof(struct nodeyacc));
						$<node->leng>$=$<node->leng>2+1;
						snprintf(code_temp,4200,"%s%s",$<node->code>1,$<node->code>2);
						strcpy($<node->code>$,code_temp);
					 }
	| T_Real items 	 {
						$<node>$=malloc(sizeof(struct nodeyacc));
						$<node->leng>$=$<node->leng>2+1;
						snprintf(code_temp,4200,"%s%s",$<data->name>1,$<node->code>2);
						strcpy($<node->code>$,code_temp);
					 }
	| T_Integer items {
						$<node>$=malloc(sizeof(struct nodeyacc));
						$<node->leng>$=$<node->leng>2+1;
						snprintf(code_temp,4200,"%s%s",$<data->name>1,$<node->code>2);
						strcpy($<node->code>$,code_temp);
					 }
	| T_ID items{
				 if (searchele($<data->name>1,$<data->scope>1)==0)
				 	{printf("\nERROR : %s Not Defined\n",yytext);return 0;}
				 $<node>$=malloc(sizeof(struct nodeyacc));
				 $<node->leng>$=$<node->leng>2+1;
				 snprintf(code_temp,4200,"%s%s",$<data->name>1,$<node->code>2);
				 strcpy($<node->code>$,code_temp);
				 }

items
	: T_Comma T_String items {
								$<node>$=malloc(sizeof(struct nodeyacc));
								$<node->leng>$=$<node->leng>2+1;
								snprintf(code_temp,4200,",%s%s",$<data->name>2,$<node->code>3);
								strcpy($<node->code>$,code_temp);
					 		 }
	| T_Comma T_Real items   {
								$<node>$=malloc(sizeof(struct nodeyacc));
								$<node->leng>$=$<node->leng>2+1;
								snprintf(code_temp,4200,",%s%s",$<data->name>2,$<node->code>3);
								strcpy($<node->code>$,code_temp);
					 		 }
	| T_Comma T_Integer items {
								$<node>$=malloc(sizeof(struct nodeyacc));
								$<node->leng>$=$<node->leng>2+1;
								snprintf(code_temp,4200,",%s%s",$<data->name>2,$<node->code>3);
								strcpy($<node->code>$,code_temp);
					 		  }
	| T_Comma T_ID items {
							if (searchele($<data->name>2,$<data->scope>2)==0)
						   		{printf("\nERROR : %s Not Defined\n",yytext);return 0;}
							$<node>$=malloc(sizeof(struct nodeyacc));
							$<node->leng>$=$<node->leng>2+1;
							snprintf(code_temp,4200,",%s%s",$<data->name>2,$<node->code>3);
							strcpy($<node->code>$,code_temp);
						 }
	| %empty {
			  $<node>$=malloc(sizeof(struct nodeyacc));
			  $<node->leng>$=0;
			  strcpy($<node->code>$,"\0");
			  }
compound_stmt
	: for_stmt {$<node>$=malloc(sizeof(struct nodeyacc));strcpy($<node->code>$,$<node->code>1);
				// printf("\n******FOR*********\n");
				// printf("%s",$<node->code>$);
				// printf("\n*******END FOR********\n");
				}
	| while_stmt {$<node>$=malloc(sizeof(struct nodeyacc));strcpy($<node->code>$,$<node->code>1);
					// printf("\n******WHILE*********\n");
					// printf("%s",$<node->code>$);
					// printf("\n*******END WHILE********\n");
					}

for_stmt
	: T_For T_ID T_In range_stmt T_Cln block_code {
													searchele($<data->name>2,$<data->scope>2);
													snprintf(code_temp,4200,"%s=%d\n",$<data->name>2,$<node->start>4);//make the initial value
													$<node>$=malloc(sizeof(struct nodeyacc));
													strcpy($<node->code>$,code_temp);

													char labelif[10];char labelb[10];char labeln[10];//labels for if and true block
													label_gen(labelif);
													label_gen(labelb);
													label_gen(labeln);
													char tempcre[10];temp_gen(tempcre);

													//print the loop condition
													//printf("\nRANGE STMT : %s\n",labelb);
													snprintf(temp,3000,"%s :\n %s=%s<%d\nIF (%s) GOTO %s\nGOTO %s\n",labelif,tempcre,$<data->name>2,$<node->end>4,tempcre,labelb,labeln);
													//printf("\nLABEL B : %s",labelb);
													strcat($<node->code>$,temp);

													//print the loop block
													
													snprintf(temp,3000,"%s :\n %s",labelb,$<node->code>6);
													strcat($<node->code>$,temp);

													//make the increment function													
													char tempinc[10];
													temp_gen(tempinc);
													snprintf(temp,3000,"%s=%s+%d\n%s=%s\nGOTO %s\n%s :\n",tempinc,$<data->name>2,$<node->step>4,$<data->name>2,tempinc,labelif,labeln);
													//printf("\nRANGE\n%s\n",$<node->code>$);
													strcat($<node->code>$,temp);
													//printf("\nRANGE : \n %s \nFINISHED\n",$<node->code>$);
													//printf("\nDRANGE : \n %s \nDFINISHED\n",$<node->code>$);
													}

	| T_For T_ID T_In list_stmt T_Cln block_code {	searchele($<data->name>2,$<data->scope>2);
													snprintf(code_temp,4200,"%s\n%s=%s\n",$<node->code>4,$<data->name>2,$<node->addr>4);
													$<node>$=malloc(sizeof(struct nodeyacc));
													strcpy($<node->code>$,code_temp);

													char labelif[10];char labelb[10];char labeln[10];//labels for if and true block
													label_gen(labelif);
													label_gen(labelb);
													label_gen(labeln);
													char tempx[10];temp_gen(tempx);
													char tempcre[10];temp_gen(tempcre);
													snprintf(temp,3000,"%s :\n%s=%s+%d\n%s=%s<%s\nIF (%s) GOTO %s\nGOTO %s\n",labelif,tempx,$<node->addr>4,$<node->leng>4,tempcre,$<data->name>2,tempx,tempcre,labelb,labeln);
													//snprintf(temp,3000,"%s : %s=%s<%s+%d\nIF (%s) GOTO %s\nGOTO %s\n\0",labelif,tempcre,$<data->name>2,$<node->addr>4,$<node->leng>4,tempcre,labelb,labeln);
													strcat($<node->code>$,temp);

													//print the loop block
													char tempg[10];temp_gen(tempg);
													snprintf(temp,3000,"%s :\n%s=%s[%s]\n%s\n",labelb,tempg,$<node->addr>4,$<data->name>2,$<node->code>6);
													strcat($<node->code>$,temp);

													char tempinc[10];
													temp_gen(tempinc);
													snprintf(temp,3000,"%s=%s+1\n%s=%s\nGOTO %s\n%s :\n",tempinc,$<data->name>2,$<data->name>2,tempinc,labelif,labeln);
													strcat($<node->code>$,temp);
													//printf("\n%s\n",$<node->code>$);															
													}
	
	| T_For T_ID T_In term T_Cln block_code {		searchele($<data->name>2,$<data->scope>2);
													snprintf(code_temp,4200,"%s\n%s=%s\n",$<node->code>4,$<data->name>2,$<node->addr>4);
													$<node>$=malloc(sizeof(struct nodeyacc));
													strcpy($<node->code>$,code_temp);

													char labelif[10];char labelb[10];char labeln[10];//labels for if and true block
													label_gen(labelif);
													label_gen(labelb);
													label_gen(labeln);
													char tempcre[10];temp_gen(tempcre);
													snprintf(temp,3000,"%s :\n %s=%s<%d\nIF (%s) GOTO %s\nGOTO %s\n",labelif,tempcre,$<data->name>2,$<node->leng>4,tempcre,labelb,labeln);
													strcat($<node->code>$,temp);

													//print the loop block
													snprintf(temp,3000,"%s :\n%s\n",labelb,$<node->code>6);
													strcat($<node->code>$,temp);

													char tempinc[10];
													temp_gen(tempinc);
													snprintf(temp,3000,"%s=%s+1\n%s=%s\nGOTO %s\n%s :\n",tempinc,$<data->name>2,$<data->name>2,tempinc,labelif,labeln);
													strcat($<node->code>$,temp);
																								
													}


range_stmt
	: T_Range T_LP T_Integer T_RP {$<node>$=malloc(sizeof(struct nodeyacc));
									$<node->start>$=0;
									$<node->end>$=atoi($<data->name>3);
									$<node->step>$=1;}
	| T_Range T_LP T_ID T_RP {		
									if (searchdo($<data->name>3)==0)
										{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
									$<node>$=malloc(sizeof(struct nodeyacc));
									$<node->start>$=0;
									$<node->end>$=$<node->value>3;
									$<node->step>$=1;
							 }
	| T_Range T_LP T_Integer T_Comma T_Integer T_RP {	
														$<node>$=malloc(sizeof(struct nodeyacc));
														$<node->start>$=atoi($<data->name>3);
														$<node->end>$=atoi($<data->name>5);
														$<node->step>$=1;
														//printf("\nRANGE : %d %d %d \n",$<node->start>$,$<node->end>$,$<node->step>$);
														}
	| T_Range T_LP T_ID T_Comma T_ID T_RP {
											if (searchdo($<data->name>3)==0)
												{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
											if (searchdo($<data->name>5)==0)
												{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
											$<node>$=malloc(sizeof(struct nodeyacc));
											$<node->start>$=$<node->value>3;
											$<node->end>$=$<node->value>5;
											$<node->step>$=1;}
	| T_Range T_LP T_Integer T_Comma T_Integer T_Comma T_Integer T_RP {$<node>$=malloc(sizeof(struct nodeyacc));
																		$<node->start>$=atoi($<data->name>3);
																		$<node->end>$=atoi($<data->name>5);
																		$<node->step>$=atoi($<data->name>7);
																	  }
	| T_Range T_LP T_ID T_Comma T_ID T_Comma T_ID T_RP {
														if (searchdo($<data->name>3)==0)
															{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
														if (searchdo($<data->name>5)==0)
															{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
														if (searchdo($<data->name>7)==0)
															{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
														$<node>$=malloc(sizeof(struct nodeyacc));
														$<node->start>$=$<node->value>3;
														$<node->end>$=$<node->value>5;
														$<node->step>$=$<node->value>7;}

while_stmt
	: T_While bool_stmt T_Cln block_code {
										  char labelif[10];char labelb[10];char labeln[10];//labels for if and true block
										  label_gen(labelif);
										  label_gen(labelb);
										  label_gen(labeln);
										  $<node>$=malloc(sizeof(struct nodeyacc));
										  char tempkar[10];int i=0;
										  
										  while($<node->code>2[i]!='=')
										  	{tempkar[i]=$<node->code>2[i];++i;}
											tempkar[i]='\0';
										
										  snprintf(code_temp,4200,"%s :\n%s\nIF (%s) GOTO %s\nGOTO %s\n%s :\n%s\nGOTO %s\n%s :\n",labelif,$<node->code>2,tempkar,labelb,labeln,labelb,$<node->code>4,labelif,labeln);
										  strcpy($<node->code>$,code_temp);
										  }

block_code
	: base_stmt {
				 $<node>$=malloc(sizeof(struct nodeyacc));
				 strcpy($<node->code>$,$<node->code>1);
				 //printf("\nBLOCK CODE :%s\n",$<node->code>$);
				}
	| T_NL T_IND stmt repeater T_DED {
										//printf("\nIN BLOCK CODDE\n");
										// printf("%s\n",$<node->code>3);
										$<node>$=malloc(sizeof(struct nodeyacc));
										snprintf(code_temp,4200,"%s%s\n",$<node->code>3,$<node->code>4);
										strcpy($<node->code>$,code_temp);
										//printf("%s",$<node->code>$);
										//printf("FINISH\n");
									 }

repeater
	: stmt repeater 	{
						//printf("\nHERE\n");
						$<node>$=malloc(sizeof(struct nodeyacc));
						snprintf(code_temp,4200,"%s%s\n",$<node->code>1,$<node->code>2);
						strcpy($<node->code>$,code_temp);
						//printf("\nREPEATER\n%s \nDONE\n %s\nFIN\n",$<node->code>1,$<node->code>2);
						}
	| T_NL stmt repeater {
						//printf("\nHERE\n");
						$<node>$=malloc(sizeof(struct nodeyacc));
						snprintf(code_temp,4200,"%s%s\n",$<node->code>2,$<node->code>3);
						strcpy($<node->code>$,code_temp);
						//printf("\nREPEATER NL\n%s \nDONE\n %s\nFIN\n",$<node->code>1,$<node->code>2);
						}
	| %empty {$<node>$=malloc(sizeof(struct nodeyacc));strcpy($<node->code>$,"\0");
	}
%%