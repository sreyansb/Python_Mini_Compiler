/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.h>

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
	
	int temp2;
	char *loop_var;	
	
	
	#define MAXQUADS 1000
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
	void insertRecord(const char* type, const char *name, int lineNo, int scope);

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
		Xitoa(no, A);
		
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
	

#line 580 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_EndOfFile = 258,
    T_Cln = 259,
    T_SCln = 260,
    T_NL = 261,
    T_IN = 262,
    T_NEQ = 263,
    T_EQ = 264,
    T_GT = 265,
    T_LT = 266,
    T_EGT = 267,
    T_ELT = 268,
    T_Or = 269,
    T_And = 270,
    ID = 271,
    ND = 272,
    DD = 273,
    T_String = 274,
    Trip_Quote = 275,
    T_Import = 276,
    T_MN = 277,
    T_PL = 278,
    T_DV = 279,
    T_ML = 280,
    T_OP = 281,
    T_CP = 282,
    T_OB = 283,
    T_CB = 284,
    T_Def = 285,
    T_Comma = 286,
    T_Range = 287,
    T_List = 288,
    T_Number = 289,
    T_ID = 290,
    T_EQL = 291,
    T_LEN = 292,
    T_True = 293,
    T_False = 294,
    T_Not = 295,
    T_Pass = 296,
    T_Break = 297,
    T_Return = 298,
    T_Print = 299,
    T_If = 300,
    T_Elif = 301,
    T_Else = 302,
    T_For = 303,
    T_While = 304
  };
#endif
/* Tokens.  */
#define T_EndOfFile 258
#define T_Cln 259
#define T_SCln 260
#define T_NL 261
#define T_IN 262
#define T_NEQ 263
#define T_EQ 264
#define T_GT 265
#define T_LT 266
#define T_EGT 267
#define T_ELT 268
#define T_Or 269
#define T_And 270
#define ID 271
#define ND 272
#define DD 273
#define T_String 274
#define Trip_Quote 275
#define T_Import 276
#define T_MN 277
#define T_PL 278
#define T_DV 279
#define T_ML 280
#define T_OP 281
#define T_CP 282
#define T_OB 283
#define T_CB 284
#define T_Def 285
#define T_Comma 286
#define T_Range 287
#define T_List 288
#define T_Number 289
#define T_ID 290
#define T_EQL 291
#define T_LEN 292
#define T_True 293
#define T_False 294
#define T_Not 295
#define T_Pass 296
#define T_Break 297
#define T_Return 298
#define T_Print 299
#define T_If 300
#define T_Elif 301
#define T_Else 302
#define T_For 303
#define T_While 304

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 512 "parser.y"
 char text[100]; int depth; char *addr; char * code; struct mynode* node;

#line 733 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   238

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  107
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  190

#define YYUNDEFTOK  2
#define YYMAXUTOK   304


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   530,   530,   530,   542,   549,   557,   560,   567,   568,
     570,   571,   571,   573,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   586,   587,   591,   595,   599,   604,   607,
     611,   615,   619,   623,   627,   633,   637,   641,   645,   647,
     648,   653,   659,   666,   672,   674,   683,   684,   685,   686,
     688,   692,   696,   700,   705,   710,   711,   712,   715,   718,
     719,   722,   723,   727,   728,   739,   740,   743,   743,   755,
     755,   757,   757,   761,   764,   768,   761,   775,   776,   777,
     781,   782,   782,   785,   786,   788,   788,   789,   791,   791,
     792,   794,   794,   796,   811,   811,   812,   812,   813,   815,
     815,   816,   816,   817,   817,   818,   820,   820
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_EndOfFile", "T_Cln", "T_SCln", "T_NL",
  "T_IN", "T_NEQ", "T_EQ", "T_GT", "T_LT", "T_EGT", "T_ELT", "T_Or",
  "T_And", "ID", "ND", "DD", "T_String", "Trip_Quote", "T_Import", "T_MN",
  "T_PL", "T_DV", "T_ML", "T_OP", "T_CP", "T_OB", "T_CB", "T_Def",
  "T_Comma", "T_Range", "T_List", "T_Number", "T_ID", "T_EQL", "T_LEN",
  "T_True", "T_False", "T_Not", "T_Pass", "T_Break", "T_Return", "T_Print",
  "T_If", "T_Elif", "T_Else", "T_For", "T_While", "$accept",
  "StartDebugger", "$@1", "constant", "list_index", "term", "StartParse",
  "$@2", "basic_stmt", "arith_exp", "bool_exp", "bool_term", "bool_factor",
  "import_stmt", "pass_stmt", "break_stmt", "return_stmt", "assign_stmt",
  "print_stmt", "finalStatements", "simple_stmt", "next_simple_stmt",
  "end_simple_stmt", "cmpd_stmt", "optional_else", "for_stmt", "$@3",
  "$@4", "$@5", "while_stmt", "$@6", "$@7", "$@8", "range_stmt", "suite",
  "$@9", "repeat_stmt", "args", "$@10", "args_list", "$@11", "func_def",
  "$@12", "list_stmt", "call_list", "$@13", "$@14", "call_args", "$@15",
  "$@16", "$@17", "func_call", "$@18", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

#define YYPACT_NINF (-142)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-107)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -142,    53,    67,  -142,  -142,    67,  -142,   -29,   178,   176,
      44,  -142,     5,  -142,  -142,    45,  -142,  -142,    64,    61,
      65,  -142,  -142,  -142,  -142,  -142,   108,   210,  -142,   120,
    -142,  -142,  -142,  -142,  -142,  -142,  -142,  -142,  -142,  -142,
    -142,  -142,  -142,  -142,  -142,   178,    75,   183,    12,    91,
    -142,    87,    56,   176,  -142,  -142,    64,   119,   176,    98,
    -142,  -142,  -142,    94,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   176,   176,    67,   129,  -142,
    -142,   105,   115,    93,    24,   210,  -142,  -142,  -142,   210,
     122,    46,  -142,  -142,   108,  -142,   183,   183,   183,   183,
     183,   183,   183,   123,   123,  -142,  -142,     6,  -142,  -142,
    -142,   135,  -142,  -142,  -142,  -142,   126,   131,  -142,   147,
    -142,  -142,  -142,   170,  -142,  -142,   148,   146,   146,   146,
    -142,    93,   144,   175,   177,   179,   124,   151,   182,    58,
    -142,  -142,  -142,   160,   -20,   124,   124,   124,   180,  -142,
    -142,   165,  -142,   124,  -142,  -142,  -142,  -142,   167,   156,
     156,   156,  -142,   156,  -142,  -142,   146,   146,    49,   205,
    -142,  -142,  -142,   150,  -142,   151,  -142,  -142,  -142,   184,
     124,   150,  -142,   199,  -142,   150,   209,  -142,  -142,  -142
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     1,    13,     0,     5,     0,     0,     0,
       0,     4,     7,    41,    42,     0,    46,    47,    48,     0,
       0,    73,     8,     9,    23,     3,     0,    18,    20,    38,
      39,    16,    14,    15,    22,    17,    21,    11,    55,    56,
      64,    63,    57,    10,    45,     0,     7,    19,     0,     0,
      91,     0,     0,     0,    43,    49,     0,     0,     0,     0,
      61,    58,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      44,     0,     0,   105,     7,    52,    51,    53,    50,     0,
       0,     0,    74,    62,     0,    37,    40,    34,    31,    36,
      35,    30,    33,    25,    24,    27,    26,     0,    29,    32,
      12,    87,     6,   103,   101,    99,     0,     0,    54,     0,
      69,    67,    71,     0,    60,    85,     0,    98,    98,    98,
      93,   105,     0,     0,     0,     0,     0,    90,     0,     0,
     104,   102,   100,     0,     0,     0,     0,     0,     0,    80,
      75,     0,    86,     0,    96,    94,   107,    77,     0,    66,
      66,    66,    81,    66,    88,    92,    98,    98,     0,     0,
      70,    68,    72,     0,    76,    90,    97,    95,    78,     0,
       0,    84,    89,     0,    65,    84,     0,    79,    83,    82
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -142,  -142,  -142,  -142,  -142,   -14,    -5,  -142,   169,    -7,
      -4,    85,   214,  -142,  -142,  -142,  -142,  -142,  -142,  -141,
    -133,   136,  -142,  -142,   -38,  -142,  -142,  -142,  -142,  -142,
    -142,  -142,  -142,  -142,  -137,  -142,    51,  -142,  -142,    62,
    -142,  -142,  -142,   140,  -111,  -142,  -142,   107,  -142,  -142,
    -142,  -142,  -142
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    22,    23,    24,    25,    77,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    61,    62,    39,   170,    40,   134,   133,   135,    41,
      58,   123,   163,   121,   150,   173,   186,   126,   137,   152,
     175,    42,    81,    87,   140,   167,   166,   116,   129,   128,
     127,    88,   117
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,    47,    48,   149,    55,    49,    44,   157,   159,   160,
     161,   158,   149,   149,   149,    64,   165,   141,   142,    63,
     149,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,   181,    51,    71,    72,    73,    74,    78,    79,
     185,    52,    90,   184,   185,    85,    89,   149,    86,    49,
    -106,    89,    51,     3,    92,   176,   177,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   107,
       4,    53,   110,     5,    83,     6,   178,   154,   119,    50,
     179,   120,     9,     6,    83,    15,     6,    56,     7,     8,
      11,    84,   155,     9,    13,    14,    15,    10,    11,    46,
      57,    11,    12,    51,    93,    13,    14,    15,    16,    17,
      18,    19,   113,    59,    60,    20,    21,     6,    80,     7,
       8,    82,   171,   172,     9,   174,    91,   114,   115,    95,
     148,   111,    11,    12,    75,    76,    13,    14,    15,    16,
      17,    18,    19,     6,   112,     7,     8,    73,    74,   118,
       9,    71,    72,    73,    74,   130,    79,   131,    11,    12,
     108,   109,    13,    14,    15,    16,    17,    18,    19,     6,
     125,     7,     8,   132,   136,   138,     9,   139,   144,   145,
      10,   146,   151,   147,    11,    12,   153,   156,    13,    14,
      15,    16,    17,    18,    19,     6,   162,     6,    20,    21,
     164,   168,     9,   169,    45,    71,    72,    73,    74,   180,
      11,    46,    11,    46,    13,    14,    15,    63,   183,    64,
      65,    66,    67,    68,    69,    70,   187,   189,    94,    54,
     124,   122,    71,    72,    73,    74,   188,   182,   143
};

static const yytype_uint8 yycheck[] =
{
       5,     8,     9,   136,    18,     9,    35,    27,   145,   146,
     147,    31,   145,   146,   147,     9,   153,   128,   129,     7,
     153,     9,    10,    11,    12,    13,    14,    15,    22,    23,
      24,    25,   173,    28,    22,    23,    24,    25,    45,    27,
     181,    36,    56,   180,   185,    52,    53,   180,    52,    53,
      26,    58,    28,     0,    58,   166,   167,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
       3,    26,    77,     6,    28,    19,    27,    19,    32,    35,
      31,    35,    26,    19,    28,    40,    19,    26,    21,    22,
      34,    35,    34,    26,    38,    39,    40,    30,    34,    35,
      35,    34,    35,    28,     6,    38,    39,    40,    41,    42,
      43,    44,    19,     5,     6,    48,    49,    19,    27,    21,
      22,    34,   160,   161,    26,   163,     7,    34,    35,    35,
       6,    26,    34,    35,    14,    15,    38,    39,    40,    41,
      42,    43,    44,    19,    29,    21,    22,    24,    25,    27,
      26,    22,    23,    24,    25,    29,    27,    26,    34,    35,
      75,    76,    38,    39,    40,    41,    42,    43,    44,    19,
      35,    21,    22,    26,     4,    27,    26,    31,    34,     4,
      30,     4,    31,     4,    34,    35,     4,    27,    38,    39,
      40,    41,    42,    43,    44,    19,    16,    19,    48,    49,
      35,    34,    26,    47,    26,    22,    23,    24,    25,     4,
      34,    35,    34,    35,    38,    39,    40,     7,    34,     9,
      10,    11,    12,    13,    14,    15,    27,    18,    59,    15,
      94,    91,    22,    23,    24,    25,   185,   175,   131
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    51,    52,     0,     3,     6,    19,    21,    22,    26,
      30,    34,    35,    38,    39,    40,    41,    42,    43,    44,
      48,    49,    53,    54,    55,    56,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    73,
      75,    79,    91,    56,    35,    26,    35,    59,    59,    60,
      35,    28,    36,    26,    62,    55,    26,    35,    80,     5,
       6,    71,    72,     7,     9,    10,    11,    12,    13,    14,
      15,    22,    23,    24,    25,    14,    15,    57,    59,    27,
      27,    92,    34,    28,    35,    59,    60,    93,   101,    59,
      55,     7,    60,     6,    58,    35,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    61,    61,
      56,    26,    29,    19,    34,    35,    97,   102,    27,    32,
      35,    83,    93,    81,    71,    35,    87,   100,    99,    98,
      29,    26,    26,    77,    76,    78,     4,    88,    27,    31,
      94,    94,    94,    97,    34,     4,     4,     4,     6,    70,
      84,    31,    89,     4,    19,    34,    27,    27,    31,    84,
      84,    84,    16,    82,    35,    84,    96,    95,    34,    47,
      74,    74,    74,    85,    74,    90,    94,    94,    27,    31,
       4,    69,    89,    34,    84,    69,    86,    27,    86,    18
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    52,    51,    53,    53,    54,    55,    55,    55,
      56,    57,    56,    56,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    59,    59,    59,    59,    59,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    61,
      61,    61,    61,    62,    62,    63,    64,    65,    66,    66,
      67,    67,    67,    67,    68,    69,    69,    69,    70,    71,
      71,    72,    72,    73,    73,    74,    74,    76,    75,    77,
      75,    78,    75,    80,    81,    82,    79,    83,    83,    83,
      84,    85,    84,    86,    86,    88,    87,    87,    90,    89,
      89,    92,    91,    93,    95,    94,    96,    94,    94,    98,
      97,    99,    97,   100,    97,    97,   102,   101
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     4,     1,     1,     1,
       2,     0,     3,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       3,     1,     1,     2,     3,     2,     1,     1,     1,     2,
       3,     3,     3,     3,     4,     1,     1,     1,     2,     1,
       3,     1,     2,     1,     1,     3,     0,     0,     8,     0,
       8,     0,     8,     0,     0,     0,     8,     4,     6,     8,
       1,     0,     6,     2,     0,     0,     3,     0,     0,     4,
       0,     0,     8,     3,     0,     4,     0,     4,     0,     0,
       3,     0,     3,     0,     3,     0,     0,     5
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 530 "parser.y"
                {init();}
#line 2170 "y.tab.c"
    break;

  case 3:
#line 531 "parser.y"
                                            {
								if(check_error == 0)
									printf("\nValid Python Syntax\n"); 
								else
									printf("\nInvalid Python Syntax\n"); 
								printf("**************************************************************************\n");
								printSTable();// freeAll();  
								printQuads();
								exit(0);
							}
#line 2185 "y.tab.c"
    break;

  case 4:
#line 542 "parser.y"
                    {
						insertRecord("Constant", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);
						fprintf(fptr,"t%d = %s\n", tempNo, (yyvsp[0].text));
						make_quad("=", (yyvsp[0].text),  "-", makeStr(tempNo, 1));
						(yyval.node) = get_node();
						(yyval.node->addr) = strdup(makeStr(tempNo++, 1));
					}
#line 2197 "y.tab.c"
    break;

  case 5:
#line 549 "parser.y"
                            {
			 			insertRecord("Constant", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);
			 			fprintf(fptr,"t%d = %s\n", tempNo,(yyvsp[0].text));
						make_quad("=", (yyvsp[0].text),  "-", makeStr(tempNo, 1));
						(yyval.node) = get_node();
						(yyval.node->addr) = strdup(makeStr(tempNo++, 1));
					}
#line 2209 "y.tab.c"
    break;

  case 6:
#line 557 "parser.y"
                                     {checkList((yyvsp[-3].text), (yylsp[-3]).first_line, currentScope);}
#line 2215 "y.tab.c"
    break;

  case 7:
#line 560 "parser.y"
            {
				modifyRecordID("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);
				fprintf(fptr,"t%d = %s\n", tempNo,(yyvsp[0].text));
				make_quad("=", (yyvsp[0].text),  "-", makeStr(tempNo, 1));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));
			}
#line 2227 "y.tab.c"
    break;

  case 8:
#line 567 "parser.y"
                {(yyval.node) = (yyvsp[0].node);}
#line 2233 "y.tab.c"
    break;

  case 10:
#line 570 "parser.y"
                             {(yyval.node) = (yyvsp[0].node);}
#line 2239 "y.tab.c"
    break;

  case 11:
#line 571 "parser.y"
                                          {/*resetDepth();*/ updateCScope(1);}
#line 2245 "y.tab.c"
    break;

  case 12:
#line 571 "parser.y"
                                                                                          {/*char *temp[200]; 				//sprintf(temp, "%s%s", $<node->code>1, $<node->code>2); $<node->code>$ = strdup(temp); 
			*/}
#line 2252 "y.tab.c"
    break;

  case 13:
#line 573 "parser.y"
                                      {(yyval.node) = NULL;}
#line 2258 "y.tab.c"
    break;

  case 14:
#line 575 "parser.y"
                       {(yyval.node)=(yyvsp[0].node);}
#line 2264 "y.tab.c"
    break;

  case 15:
#line 576 "parser.y"
                                     {(yyval.node)=(yyvsp[0].node);}
#line 2270 "y.tab.c"
    break;

  case 16:
#line 577 "parser.y"
                                      {(yyval.node)=(yyvsp[0].node);}
#line 2276 "y.tab.c"
    break;

  case 17:
#line 578 "parser.y"
                                      {(yyval.node)=(yyvsp[0].node);}
#line 2282 "y.tab.c"
    break;

  case 18:
#line 579 "parser.y"
                                    {(yyval.node)=(yyvsp[0].node);}
#line 2288 "y.tab.c"
    break;

  case 19:
#line 580 "parser.y"
                                         {(yyval.node)=(yyvsp[0].node);}
#line 2294 "y.tab.c"
    break;

  case 20:
#line 581 "parser.y"
                                   {(yyval.node)=(yyvsp[0].node);}
#line 2300 "y.tab.c"
    break;

  case 21:
#line 582 "parser.y"
                                     {(yyval.node)=(yyvsp[0].node);}
#line 2306 "y.tab.c"
    break;

  case 22:
#line 583 "parser.y"
                                      {(yyval.node)=(yyvsp[0].node);}
#line 2312 "y.tab.c"
    break;

  case 23:
#line 586 "parser.y"
                  {(yyval.node)=(yyvsp[0].node);}
#line 2318 "y.tab.c"
    break;

  case 24:
#line 587 "parser.y"
                                                     {
				fprintf(fptr, "t%d = %s + %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++,1));}
#line 2327 "y.tab.c"
    break;

  case 25:
#line 591 "parser.y"
                                                    {
				fprintf(fptr, "t%d = %s - %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++,1));}
#line 2336 "y.tab.c"
    break;

  case 26:
#line 595 "parser.y"
                                                    {
				fprintf(fptr, "t%d = %s * %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++,1));}
#line 2345 "y.tab.c"
    break;

  case 27:
#line 599 "parser.y"
                                                     {
				fprintf(fptr, "t%d = %s mod %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++,1));}
#line 2354 "y.tab.c"
    break;

  case 28:
#line 604 "parser.y"
                                              {(yyval.node) = (yyvsp[-1].node);}
#line 2360 "y.tab.c"
    break;

  case 29:
#line 607 "parser.y"
                                    {
				fprintf(fptr, "t%d = %s or %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));}
#line 2369 "y.tab.c"
    break;

  case 30:
#line 611 "parser.y"
                                            {
				fprintf(fptr, "t%d = %s or %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));}
#line 2378 "y.tab.c"
    break;

  case 31:
#line 615 "parser.y"
                                    {
				fprintf(fptr, "t%d = %s < %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));}
#line 2387 "y.tab.c"
    break;

  case 32:
#line 619 "parser.y"
                                     {
				fprintf(fptr, "t%d = %s and %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));}
#line 2396 "y.tab.c"
    break;

  case 33:
#line 623 "parser.y"
                                     {
				fprintf(fptr, "t%d = %s and %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));}
#line 2405 "y.tab.c"
    break;

  case 34:
#line 627 "parser.y"
                                    {
				fprintf(fptr, "t%d = %s > %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));
				(yyval.node->code) = strdup("code");
				}
#line 2416 "y.tab.c"
    break;

  case 35:
#line 633 "parser.y"
                                     {
				fprintf(fptr, "t%d = %s <= %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));}
#line 2425 "y.tab.c"
    break;

  case 36:
#line 637 "parser.y"
                                     {
				fprintf(fptr, "t%d = %s >= %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));}
#line 2434 "y.tab.c"
    break;

  case 37:
#line 641 "parser.y"
                               {checkList((yyvsp[0].text), (yylsp[0]).first_line, currentScope);
				fprintf(fptr, "t%d = %s in %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));}
#line 2443 "y.tab.c"
    break;

  case 38:
#line 645 "parser.y"
                     {(yyval.node) = (yyvsp[0].node);}
#line 2449 "y.tab.c"
    break;

  case 39:
#line 647 "parser.y"
                        {(yyval.node) = (yyvsp[0].node);}
#line 2455 "y.tab.c"
    break;

  case 40:
#line 648 "parser.y"
                                     {
  				fprintf(fptr, "t%d = %s == %s\n", tempNo, (yyvsp[-2].node->addr), (yyvsp[0].node->addr));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));
			}
#line 2465 "y.tab.c"
    break;

  case 41:
#line 653 "parser.y"
                   {//insertRecord("Constant", "True", @1.first_line, currentScope);
				fprintf(fptr,"t%d = True\n", tempNo);
				make_quad("=", "True",  "-", makeStr(tempNo, 1));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));
          }
#line 2476 "y.tab.c"
    break;

  case 42:
#line 659 "parser.y"
                    {//insertRecord("Constant", "False", @1.first_line, currentScope);
				fprintf(fptr,"t%d = False\n", tempNo);
				make_quad("=", "False",  "-", makeStr(tempNo, 1));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));
          }
#line 2487 "y.tab.c"
    break;

  case 43:
#line 666 "parser.y"
                                {
				fprintf(fptr,"t%d = ! %s\n", tempNo, (yyvsp[-1].node->addr));
				make_quad("!", (yyvsp[-1].node->addr),  "-", makeStr(tempNo, 1));
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));
			}
#line 2498 "y.tab.c"
    break;

  case 44:
#line 672 "parser.y"
                                 {(yyval.node) = (yyvsp[-1].node);}
#line 2504 "y.tab.c"
    break;

  case 45:
#line 674 "parser.y"
                            {insertRecord("PackageName", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);
				fprintf(fptr,"t%d = %s\n", tempNo, (yyvsp[0].text));
				make_quad("=", (yyvsp[0].text),  "-", makeStr(tempNo, 1));
				fprintf(fptr,"import t%d\n", tempNo);
				make_quad("import", makeStr(tempNo, 1),  "-", "-");
				(yyval.node) = get_node();
				(yyval.node->addr) = strdup(makeStr(tempNo++, 1));
			}
#line 2517 "y.tab.c"
    break;

  case 46:
#line 683 "parser.y"
                     {(yyval.node) = NULL;}
#line 2523 "y.tab.c"
    break;

  case 47:
#line 684 "parser.y"
                      {fprintf(fptr, "goto L%d\n",lIndex);(yyval.node) = NULL;}
#line 2529 "y.tab.c"
    break;

  case 48:
#line 685 "parser.y"
                       {(yyval.node) = NULL;}
#line 2535 "y.tab.c"
    break;

  case 49:
#line 686 "parser.y"
                                        {(yyval.node) = NULL;}
#line 2541 "y.tab.c"
    break;

  case 50:
#line 688 "parser.y"
                                   {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);
				fprintf(fptr,"%s = %s\n", (yyvsp[-2].text), (yyvsp[0].node->addr));
				make_quad("=", (yyvsp[0].node->addr), "-", (yyvsp[-2].text));
			}
#line 2550 "y.tab.c"
    break;

  case 51:
#line 692 "parser.y"
                                              {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);
				fprintf(fptr,"%s = %s\n", (yyvsp[-2].text), (yyvsp[0].node->addr));
				make_quad("=", (yyvsp[0].node->addr), "-", (yyvsp[-2].text));
			 }
#line 2559 "y.tab.c"
    break;

  case 52:
#line 696 "parser.y"
                                                {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);
				fprintf(fptr,"%s = %s\n", (yyvsp[-2].text), (yyvsp[0].node->addr));
				make_quad("=", (yyvsp[0].node->addr), "-", (yyvsp[-2].text));
			 }
#line 2568 "y.tab.c"
    break;

  case 53:
#line 700 "parser.y"
                                                {insertRecord("ListTypeID", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);
				fprintf(fptr,"%s = %s\n", (yyvsp[-2].text), (yyvsp[0].node->addr));
				make_quad("=", (yyvsp[0].node->addr), "-", (yyvsp[-2].text));
			 }
#line 2577 "y.tab.c"
    break;

  case 54:
#line 705 "parser.y"
                                    {
				fprintf(fptr,"Print %s\n", (yyvsp[-1].node->addr));
				make_quad("Print", (yyvsp[-1].node->addr), "-", "-");
			}
#line 2586 "y.tab.c"
    break;

  case 63:
#line 727 "parser.y"
                                    {(yyval.node) = (yyvsp[0].node);}
#line 2592 "y.tab.c"
    break;

  case 64:
#line 728 "parser.y"
                                   {(yyval.node) = (yyvsp[0].node);}
#line 2598 "y.tab.c"
    break;

  case 67:
#line 743 "parser.y"
                                     {
		insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);
			insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope); 
			char rangeNodeText[20] ="";
			strcat(rangeNodeText, (yyvsp[-2].text));
			strcat(rangeNodeText, " in range");
			//printf("%\n%s", rangeNodeText);
			//$$ =make_node("FOR", "FOR", make_leaf(rangeNodeText, argsList), $6);  
			for_code_before_suite(rangeNodeText, argsList);
			clearArgsList(); 
		}
#line 2614 "y.tab.c"
    break;

  case 68:
#line 754 "parser.y"
                                      {for_code_after_suite();}
#line 2620 "y.tab.c"
    break;

  case 69:
#line 755 "parser.y"
                               {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);}
#line 2626 "y.tab.c"
    break;

  case 70:
#line 756 "parser.y"
                {checkList((yyvsp[-4].text), (yylsp[-4]).first_line, currentScope);}
#line 2632 "y.tab.c"
    break;

  case 71:
#line 757 "parser.y"
                                    {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);}
#line 2638 "y.tab.c"
    break;

  case 72:
#line 757 "parser.y"
                                                                                                                                   {(yyval.node) = NULL;}
#line 2644 "y.tab.c"
    break;

  case 73:
#line 761 "parser.y"
                  {fprintf(fptr, "L%d:\n",lIndex); 
			make_quad("Label", "-", "-", makeStr(lIndex, 0)); 
		}
#line 2652 "y.tab.c"
    break;

  case 74:
#line 764 "parser.y"
                         { 	
			fprintf(fptr,"If False %s goto L%d\n", (yyvsp[0].node->addr), lIndex+1); 
			make_quad("If False", (yyvsp[0].node->addr), "-", makeStr(lIndex+1, 0)); 
		}
#line 2661 "y.tab.c"
    break;

  case 75:
#line 768 "parser.y"
                            {
			fprintf(fptr,"goto L%d\n",lIndex);
			make_quad("goto", "-", "-", makeStr(lIndex, 0));
			fprintf(fptr,"L%d:",lIndex+1);
			make_quad("Label", "-", "-", makeStr(lIndex+1, 0)); 
		}
#line 2672 "y.tab.c"
    break;

  case 76:
#line 773 "parser.y"
                                {lIndex += 2;}
#line 2678 "y.tab.c"
    break;

  case 77:
#line 775 "parser.y"
                                       {addToList("0", 1); addToList((yyvsp[-1].text), 0); }
#line 2684 "y.tab.c"
    break;

  case 78:
#line 776 "parser.y"
                                                                      {addToList((yyvsp[-3].text), 1); addToList((yyvsp[-1].text), 0);}
#line 2690 "y.tab.c"
    break;

  case 79:
#line 777 "parser.y"
                                                                                        {addToList((yyvsp[-5].text), 1); addToList((yyvsp[-3].text), 1); addToList((yyvsp[-1].text), 1); }
#line 2696 "y.tab.c"
    break;

  case 81:
#line 782 "parser.y"
                  {initNewTable((yyvsp[0].depth)); updateCScope((yyvsp[0].depth));}
#line 2702 "y.tab.c"
    break;

  case 82:
#line 782 "parser.y"
                                                                                                     {updateCScope(currentScope-1); }
#line 2708 "y.tab.c"
    break;

  case 85:
#line 788 "parser.y"
            {insertRecord("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); addToList((yyvsp[0].text), 1);}
#line 2714 "y.tab.c"
    break;

  case 86:
#line 788 "parser.y"
                                                                                                                   {(yyval.node) = NULL;}
#line 2720 "y.tab.c"
    break;

  case 87:
#line 789 "parser.y"
                  {clearArgsList();}
#line 2726 "y.tab.c"
    break;

  case 88:
#line 791 "parser.y"
                         {insertRecord("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); addToList((yyvsp[0].text), 0);}
#line 2732 "y.tab.c"
    break;

  case 90:
#line 792 "parser.y"
                          {addToList("",0); clearArgsList();}
#line 2738 "y.tab.c"
    break;

  case 91:
#line 794 "parser.y"
                      {insertRecord("Func_Name", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2744 "y.tab.c"
    break;

  case 92:
#line 794 "parser.y"
                                                                                                                     {clearArgsList();}
#line 2750 "y.tab.c"
    break;

  case 93:
#line 796 "parser.y"
                               {
		 		char* str = (char *)malloc(102*sizeof(char));
			 	strcpy(str,"[");
			 	strcat(str, argsList);
			 	char close[2];
			 	strcpy(close,"]");
			 	strcat(str, close);
			 	fprintf(fptr, "t%d = %s\n", tempNo, str);
			 	(yyval.node) = get_node();
			 	(yyval.node->addr) = strdup(makeStr(tempNo, 1));
			 	make_quad("=", str, "-", makeStr(tempNo++, 1));
			 	clearArgsList(); 
			 	free(str);
			 }
#line 2769 "y.tab.c"
    break;

  case 94:
#line 811 "parser.y"
                             {addToList((yyvsp[-1].text), 0);}
#line 2775 "y.tab.c"
    break;

  case 96:
#line 812 "parser.y"
                                           {addToList((yyvsp[-1].text), 0);}
#line 2781 "y.tab.c"
    break;

  case 99:
#line 815 "parser.y"
                 {modifyRecordID("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); addToList((yyvsp[0].text), 1);}
#line 2787 "y.tab.c"
    break;

  case 101:
#line 816 "parser.y"
                                                   {addToList((yyvsp[0].text), 1);}
#line 2793 "y.tab.c"
    break;

  case 102:
#line 816 "parser.y"
                                                                                       {/*clearArgsList();*/}
#line 2799 "y.tab.c"
    break;

  case 103:
#line 817 "parser.y"
                                                   {addToList((yyvsp[0].text), 1);}
#line 2805 "y.tab.c"
    break;

  case 104:
#line 817 "parser.y"
                                                                                       {/*clearArgsList();*/}
#line 2811 "y.tab.c"
    break;

  case 105:
#line 818 "parser.y"
                                          {}
#line 2817 "y.tab.c"
    break;

  case 106:
#line 820 "parser.y"
                 {modifyRecordID("Func_Name", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2823 "y.tab.c"
    break;


#line 2827 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 822 "parser.y"

void yyerror(const char *msg)
{
	printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.last_column);
	printf("**************************************************************************\n");
	printf("**************************************************************************\n");
}


void for_code_before_suite(char *rtext, char *list)
{
	temp2 = tempNo;
	int temp = lIndex;

	//printf("%s\n",node->child[0]->nodeType); argsList
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
    //printf("\nrangeStart =%s rangeEnd =%s",rangeStart,rangeEnd);

    //char *loop_var = strtok(rtext, " ");
    loop_var = strtok(rtext, " ");
    
	make_quad("=", rangeStart, "-",  loop_var); //initializing i=0
	fprintf(fptr, "%s = %s\n", loop_var, rangeStart);

	make_quad( "Label", "-", "-",makeStr(lIndex, 0));	//everything in the current loop is under a label
	fprintf(fptr, "L%d: ", lIndex);

	make_quad("<", loop_var, rangeEnd, makeStr(tempNo,1)); //t=i<n
	fprintf(fptr, "t%d = %s <%s\n", tempNo, loop_var, rangeEnd);
	
	make_quad("If False", makeStr(tempNo, 1), "-",makeStr(lIndex+1, 0));
	//if condition is false, goto exit label				
	fprintf(fptr, "If False t%d goto L%d\n", tempNo++, lIndex+1);

	lIndex+=2;		
}
//ICG_main(node->child[1]);

void for_code_after_suite()
{	
	int temp = lIndex - 2;
	//increment loop variable
	fprintf(fptr, "t%d = %s + 1\n", temp2, loop_var);
	make_quad("+", loop_var, "1", makeStr(temp2,1));
	fprintf(fptr, "%s = t%d\n", loop_var, temp2);
	make_quad("=", makeStr(temp2,1), "-",  loop_var);
	//make_quad("=", makeStr(tempNo++,1), "-",  loop_var);
	
	fprintf(fptr, "goto L%d\n", temp); //end of loop
	make_quad("goto", "-", "-", makeStr(temp, 0));

	fprintf(fptr, "L%d: ", temp+1);
	make_quad("Label", "-", "-", makeStr(temp+1, 0)); 

	lIndex = lIndex+2;
	return;
}



