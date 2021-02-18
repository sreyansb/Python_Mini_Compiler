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



	//--------------------------------BASIC VARIABLE DECLARATIONS----------------------------------------
	extern int yylineno;
	extern int depth;
	extern int top_1();
	extern int pop_1();
	int currentScope = 1;//int previousScope = 1;
	
	int check_error = 0;
	
	int *arrayScope = NULL;

	/*
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
	*/


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
	
	void resetDepth()
	{
		while(top_1()!=-1) 
			pop_1();
		depth = 1;
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
		symbolTables[sIndex].Elements = (record*)calloc(20, sizeof(record));
		
		symbolTables[sIndex].Parent = scopeBasedTableSearch(currentScope); 
	}
	
	void init()
	{
		argsList = (char *)malloc(100);
		strcpy(argsList, "");
		symbolTables = (STable*)calloc(100, sizeof(STable));
		//all_quads = (quad*)calloc(MAXQUADS, sizeof(quad));
		//tString = (char*)calloc(10, sizeof(char));
		//lString = (char*)calloc(10, sizeof(char));
		arrayScope = (int*)calloc(10, sizeof(int));
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
		//printf("\n In modifyRecordID");
		
		//Imp change made - made check_error global
		//int check_error = 0;
		int i = 0;
		
		//Important change made
		//int index = scopeBasedTableSearch(scope);
		int FScope = power(scope, arrayScope[scope]);
		int index = scopeBasedTableSearch(FScope);
		
		
		
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


#line 404 "y.tab.c"

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
#line 338 "parser.y"
 char *text; int depth; struct AST *node; 

#line 557 "y.tab.c"

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
#define YYLAST   247

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  97
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  179

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
       0,   358,   358,   358,   369,   370,   374,   377,   378,   379,
     381,   382,   382,   383,   385,   386,   387,   388,   389,   390,
     391,   392,   395,   396,   397,   398,   399,   400,   401,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   418,
     419,   420,   421,   423,   424,   426,   428,   429,   430,   430,
     432,   433,   434,   435,   437,   439,   440,   444,   447,   448,
     451,   452,   453,   455,   456,   457,   460,   463,   464,   468,
     469,   482,   482,   485,   491,   496,   516,   518,   519,   520,
     533,   534,   534,   537,   538,   572,   573,   586,   587,   589,
     589,   590,   590,   591,   591,   592,   596,   596
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
  "end_simple_stmt", "cmpd_stmt", "if_stmt", "elif_stmt", "optional_else",
  "for_stmt", "$@3", "endfor", "while_stmt", "range_stmt", "suite", "$@4",
  "repeat_stmt", "list_stmt", "call_list", "call_args", "$@5", "$@6",
  "$@7", "func_call", "$@8", YY_NULLPTR
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

#define YYPACT_NINF (-128)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -128,    12,    79,  -128,  -128,    79,  -128,   -11,    18,   164,
    -128,   -21,  -128,  -128,   -15,  -128,  -128,    -8,     4,   164,
      13,   164,  -128,  -128,  -128,  -128,   110,   217,  -128,   121,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,    26,  -128,   198,    29,    52,
      68,   164,  -128,  -128,    18,   217,    89,  -128,    91,     0,
    -128,  -128,  -128,    62,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   164,   164,    79,  -128,  -128,
      75,    -6,    26,   217,  -128,  -128,  -128,    98,   125,   122,
     125,  -128,   110,  -128,   115,    87,    87,    87,    87,    87,
      87,    87,   114,   114,  -128,  -128,   192,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,   103,   116,  -128,   124,  -128,
      99,    23,   101,  -128,   169,   123,   123,   123,  -128,    38,
    -128,   164,   101,   126,   152,  -128,   178,   183,   184,  -128,
      18,  -128,  -128,  -128,   162,   136,   191,  -128,   163,   125,
     125,   125,   125,   123,  -128,   136,   125,    53,   101,   101,
     101,  -128,  -128,   136,   182,    99,  -128,   172,  -128,  -128,
    -128,  -128,  -128,  -128,    61,  -128,   185,   206,  -128
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     1,    13,     0,     5,     0,     0,     0,
       4,     7,    41,    42,     0,    46,    47,    48,     0,     0,
       0,     0,     8,     9,    22,     3,    62,    18,    19,    38,
      39,    16,    14,    15,    21,    17,    20,    11,    55,    56,
      63,    65,    64,    10,    45,     7,    27,     0,     0,     0,
       0,     0,    43,    49,     0,     0,     0,    71,     0,     0,
      60,    57,    58,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    28,    44,
       0,     0,     7,    52,    51,    53,    50,     0,     0,     0,
       0,    61,    62,    37,     0,    40,    34,    31,    36,    35,
      30,    33,    24,    23,    26,    25,     0,    29,    32,    12,
       6,    93,    85,    91,    89,     0,     0,    54,     0,    80,
      67,     0,    69,    59,     0,    88,    88,    88,    86,    95,
      81,     0,    69,     0,     0,    72,     0,     0,     0,    76,
       0,    94,    92,    90,     0,     0,     0,    66,     0,     0,
       0,     0,     0,    88,    97,    84,     0,     0,    69,    69,
      69,    70,    87,    84,     0,    67,    77,     0,    74,    73,
      75,    83,    82,    68,     0,    78,     0,     0,    79
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -128,  -128,  -128,  -128,  -128,    -7,    -3,  -128,   159,    -5,
      -1,    97,   220,  -128,  -128,  -128,  -128,  -128,  -128,   -64,
      -2,   143,  -128,  -128,  -128,    71,  -127,  -128,  -128,  -128,
    -128,  -128,   -73,  -128,    74,   117,  -117,   118,  -128,  -128,
    -128,  -128,  -128
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    22,    23,    24,    25,    77,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
     119,    61,    62,    39,    40,   132,   139,    41,    89,   135,
      42,   136,   120,   145,   164,    85,   141,   115,   127,   126,
     125,    86,   116
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      38,    46,    43,    38,    47,   147,    91,    49,    48,   142,
     143,    51,     3,   111,    55,    50,    55,   122,    56,     6,
      58,     7,     8,   112,    44,    14,     9,    53,   113,   114,
      54,   168,   169,   170,    10,    11,   162,     6,    12,    13,
      14,    15,    16,    17,    18,    83,    55,    87,    57,    84,
      48,    81,    10,    45,    49,   133,    79,   111,   134,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   106,   113,   114,   109,    38,   158,   159,   160,   161,
     166,   155,     4,   165,   167,     5,    80,     6,   175,   124,
       8,   163,   176,    88,     9,    90,    81,    93,     6,   163,
       7,     8,    10,    82,   110,     9,    12,    13,    14,    71,
      72,    73,    74,    10,    11,    59,    60,    12,    13,    14,
      15,    16,    17,    18,    19,   117,    55,    20,    21,   121,
     146,   118,   128,   153,     6,    75,    76,     8,    73,    74,
     130,    94,   129,    38,     6,   131,     7,     8,   138,    10,
      45,     9,   148,    38,   140,     6,   149,     7,     8,    10,
      11,    38,     9,    12,    13,    14,    15,    16,    17,    18,
      10,    11,   107,   108,    12,    13,    14,    15,    16,    17,
      18,    19,   150,     6,    20,    21,     8,   151,   152,   154,
       9,    71,    72,    73,    74,   156,    78,   157,    10,    45,
     172,    64,    12,    13,    14,    63,   174,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    92,   177,
      71,    72,    73,    74,    63,    78,    64,    65,    66,    67,
      68,    69,    70,   178,    52,   123,   173,   171,   137,    71,
      72,    73,    74,     0,     0,     0,     0,   144
};

static const yytype_int16 yycheck[] =
{
       2,     8,     5,     5,     9,   132,     6,    28,     9,   126,
     127,    26,     0,    19,    19,    36,    21,    90,    19,    19,
      21,    21,    22,    29,    35,    40,    26,    35,    34,    35,
      26,   158,   159,   160,    34,    35,   153,    19,    38,    39,
      40,    41,    42,    43,    44,    50,    51,    54,    35,    50,
      51,    28,    34,    35,    28,    32,    27,    19,    35,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    34,    35,    77,    77,   149,   150,   151,   152,
      27,   145,     3,   156,    31,     6,    34,    19,    27,    94,
      22,   155,    31,     4,    26,     4,    28,    35,    19,   163,
      21,    22,    34,    35,    29,    26,    38,    39,    40,    22,
      23,    24,    25,    34,    35,     5,     6,    38,    39,    40,
      41,    42,    43,    44,    45,    27,   131,    48,    49,     7,
     131,     6,    29,   140,    19,    14,    15,    22,    24,    25,
      16,    26,    26,   145,    19,    46,    21,    22,    47,    34,
      35,    26,    26,   155,    31,    19,     4,    21,    22,    34,
      35,   163,    26,    38,    39,    40,    41,    42,    43,    44,
      34,    35,    75,    76,    38,    39,    40,    41,    42,    43,
      44,    45,     4,    19,    48,    49,    22,     4,     4,    27,
      26,    22,    23,    24,    25,     4,    27,    34,    34,    35,
      18,     9,    38,    39,    40,     7,    34,     9,    10,    11,
      12,    13,    14,    15,    22,    23,    24,    25,    59,    34,
      22,    23,    24,    25,     7,    27,     9,    10,    11,    12,
      13,    14,    15,    27,    14,    92,   165,   163,   121,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,   129
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    51,    52,     0,     3,     6,    19,    21,    22,    26,
      34,    35,    38,    39,    40,    41,    42,    43,    44,    45,
      48,    49,    53,    54,    55,    56,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    73,
      74,    77,    80,    56,    35,    35,    55,    59,    60,    28,
      36,    26,    62,    35,    26,    59,    60,    35,    60,     5,
       6,    71,    72,     7,     9,    10,    11,    12,    13,    14,
      15,    22,    23,    24,    25,    14,    15,    57,    27,    27,
      34,    28,    35,    59,    60,    85,    91,    55,     4,    78,
       4,     6,    58,    35,    26,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    61,    61,    56,
      29,    19,    29,    34,    35,    87,    92,    27,     6,    70,
      82,     7,    82,    71,    59,    90,    89,    88,    29,    26,
      16,    46,    75,    32,    35,    79,    81,    85,    47,    76,
      31,    86,    86,    86,    87,    83,    60,    76,    26,     4,
       4,     4,     4,    55,    27,    69,     4,    34,    82,    82,
      82,    82,    86,    69,    84,    82,    27,    31,    76,    76,
      76,    84,    18,    75,    34,    27,    31,    34,    27
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    52,    51,    53,    53,    54,    55,    55,    55,
      56,    57,    56,    56,    58,    58,    58,    58,    58,    58,
      58,    58,    59,    59,    59,    59,    59,    59,    59,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    61,
      61,    61,    61,    62,    62,    63,    64,    65,    66,    66,
      67,    67,    67,    67,    68,    69,    69,    70,    71,    71,
      72,    72,    72,    73,    73,    73,    74,    75,    75,    76,
      76,    78,    77,    79,    79,    79,    80,    81,    81,    81,
      82,    83,    82,    84,    84,    85,    85,    86,    86,    88,
      87,    89,    87,    90,    87,    87,    92,    91
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     4,     1,     1,     1,
       2,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       3,     1,     1,     2,     3,     2,     1,     1,     1,     2,
       3,     3,     3,     3,     4,     1,     1,     2,     1,     3,
       1,     2,     0,     1,     1,     1,     6,     0,     5,     0,
       3,     0,     5,     4,     4,     4,     5,     4,     6,     8,
       1,     0,     6,     2,     0,     2,     3,     3,     0,     0,
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
#line 358 "parser.y"
                {init();}
#line 1987 "y.tab.c"
    break;

  case 3:
#line 359 "parser.y"
                                            {
								if(check_error == 0)
									printf("\nValid Python Syntax\n" ); 
								else
									printf("\nInvalid Python Syntax\n" ); 
								printf("**************************************************************************\n");
								printSTable();// freeAll();  
								exit(0);
							}
#line 2001 "y.tab.c"
    break;

  case 4:
#line 369 "parser.y"
                    {insertRecord("Constant", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2007 "y.tab.c"
    break;

  case 5:
#line 370 "parser.y"
                            {insertRecord("Constant", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2013 "y.tab.c"
    break;

  case 6:
#line 374 "parser.y"
                                     {checkList((yyvsp[-3].text), (yylsp[-3]).first_line, currentScope);}
#line 2019 "y.tab.c"
    break;

  case 7:
#line 377 "parser.y"
            {modifyRecordID("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2025 "y.tab.c"
    break;

  case 11:
#line 382 "parser.y"
                                          {resetDepth(); updateCScope(1);}
#line 2031 "y.tab.c"
    break;

  case 37:
#line 415 "parser.y"
                               {checkList((yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2037 "y.tab.c"
    break;

  case 41:
#line 420 "parser.y"
                   {insertRecord("Constant", "True", (yylsp[0]).first_line, currentScope);}
#line 2043 "y.tab.c"
    break;

  case 42:
#line 421 "parser.y"
                    {insertRecord("Constant", "False", (yylsp[0]).first_line, currentScope);}
#line 2049 "y.tab.c"
    break;

  case 45:
#line 426 "parser.y"
                            {insertRecord("PackageName", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2055 "y.tab.c"
    break;

  case 49:
#line 430 "parser.y"
                                       {char return_val[100]; strcpy(return_val, "return "); strcat(return_val, (yyvsp[0].text));}
#line 2061 "y.tab.c"
    break;

  case 50:
#line 432 "parser.y"
                                    {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);}
#line 2067 "y.tab.c"
    break;

  case 51:
#line 433 "parser.y"
                                               {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);}
#line 2073 "y.tab.c"
    break;

  case 52:
#line 434 "parser.y"
                                                {insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);}
#line 2079 "y.tab.c"
    break;

  case 53:
#line 435 "parser.y"
                                                {insertRecord("ListTypeID", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);}
#line 2085 "y.tab.c"
    break;

  case 67:
#line 463 "parser.y"
          {}
#line 2091 "y.tab.c"
    break;

  case 69:
#line 468 "parser.y"
          {}
#line 2097 "y.tab.c"
    break;

  case 71:
#line 482 "parser.y"
                     {insertRecord("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2103 "y.tab.c"
    break;

  case 73:
#line 485 "parser.y"
                                               {
				//char rangeNodeText[20] ="";
				//strcat(rangeNodeText, $<text>2);
				//strcat(rangeNodeText, " in range");
				clearArgsList(); 
				}
#line 2114 "y.tab.c"
    break;

  case 74:
#line 491 "parser.y"
                                         {//printf("\n\nBEFOREEE\n");printf("AFTERRR"); 
		 //printSTable();
		 checkList((yyvsp[-3].text), (yylsp[-3]).first_line, currentScope);
		 }
#line 2123 "y.tab.c"
    break;

  case 77:
#line 518 "parser.y"
                                       {addToList("0", 1); addToList((yyvsp[-1].text), 0);}
#line 2129 "y.tab.c"
    break;

  case 78:
#line 519 "parser.y"
                                                                      {addToList((yyvsp[-3].text), 1); addToList((yyvsp[-1].text), 0);}
#line 2135 "y.tab.c"
    break;

  case 79:
#line 520 "parser.y"
                                                                                       {addToList((yyvsp[-5].text), 1); addToList((yyvsp[-3].text), 0); addToList((yyvsp[-1].text),0);}
#line 2141 "y.tab.c"
    break;

  case 81:
#line 534 "parser.y"
                  {initNewTable((yyvsp[0].depth)); updateCScope((yyvsp[0].depth));}
#line 2147 "y.tab.c"
    break;

  case 82:
#line 534 "parser.y"
                                                                                                     {updateCScope(currentScope-1); }
#line 2153 "y.tab.c"
    break;

  case 86:
#line 573 "parser.y"
                                       {
		 		char* str = (char *)malloc(102*sizeof(char));
			 	strcpy(str,"[");
			 	strcat(str, argsList);
			 	char close[2];
			 	strcpy(close,"]");
			 	strcat(str, close);
			 	clearArgsList(); 
			 	free(str);
			 }
#line 2168 "y.tab.c"
    break;

  case 89:
#line 589 "parser.y"
                 {modifyRecordID("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); addToList((yyvsp[0].text), 1);}
#line 2174 "y.tab.c"
    break;

  case 91:
#line 590 "parser.y"
                                                   {addToList((yyvsp[0].text), 1);}
#line 2180 "y.tab.c"
    break;

  case 92:
#line 590 "parser.y"
                                                                                       {clearArgsList();}
#line 2186 "y.tab.c"
    break;

  case 93:
#line 591 "parser.y"
                                                   {addToList((yyvsp[0].text), 1);}
#line 2192 "y.tab.c"
    break;

  case 94:
#line 591 "parser.y"
                                                                                       {clearArgsList();}
#line 2198 "y.tab.c"
    break;

  case 96:
#line 596 "parser.y"
                 {modifyRecordID("Func_Name", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2204 "y.tab.c"
    break;


#line 2208 "y.tab.c"

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
#line 601 "parser.y"


void yyerror(const char *msg)
{
	printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.last_column);
	printf("**************************************************************************\n");
	printf("**************************************************************************\n");
}

