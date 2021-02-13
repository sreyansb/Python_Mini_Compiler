/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser_file.y" /* yacc.c:339  */

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

	//Functions for AST
	struct AST* make_for_node(char* root, AST_node* child1, AST_node* child2, AST_node* child3, AST_node* child4);
	struct AST * make_node(char*, AST_node*, AST_node*);
	struct AST* make_leaf(char* root);
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



	struct AST* make_node(char* root, AST_node* child1, AST_node* child2)
	{
		//printf("Creating new node\n");
		struct AST * node = (struct AST*)malloc(sizeof(struct AST));
		node->child = (struct AST**)malloc(2*sizeof(struct AST *));
		node->NumChild = 2;//
		strcpy(node->lexeme,root);
		//printf("Copied lexeme\n");
		//printf("%s\n",node->lexeme);
		//printf("Children = %s  %s\n", child1->lexeme, child2->lexeme);
		node->child[0] = child1;
		node->child[1] = child2;
		return node;
	}

	struct AST* make_for_node(char* root, AST_node* child1, AST_node* child2, AST_node* child3, AST_node* child4)
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
		return node;
	}


	struct AST* make_leaf(char* root)
	{
		//printf("Creating new leaf ");
		struct AST * node = (struct AST*)malloc(sizeof(struct AST));
		strcpy(node->lexeme,root);
		//printf("%s\n",node->lexeme);
		node->NumChild = 0;
		node->child = NULL;
		return node;
	}


#line 415 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


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
    T_Number = 259,
    T_Cln = 260,
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
    T_ID = 289,
    T_EQL = 290,
    T_LEN = 291,
    T_True = 292,
    T_False = 293,
    T_Not = 294,
    T_Pass = 295,
    T_Break = 296,
    T_Return = 297,
    T_Print = 298,
    T_If = 299,
    T_Elif = 300,
    T_Else = 301,
    T_For = 302,
    T_While = 303
  };
#endif
/* Tokens.  */
#define T_EndOfFile 258
#define T_Number 259
#define T_Cln 260
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
#define T_ID 289
#define T_EQL 290
#define T_LEN 291
#define T_True 292
#define T_False 293
#define T_Not 294
#define T_Pass 295
#define T_Break 296
#define T_Return 297
#define T_Print 298
#define T_If 299
#define T_Elif 300
#define T_Else 301
#define T_For 302
#define T_While 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 352 "parser_file.y" /* yacc.c:355  */
 char *text; int depth; struct AST *node; 

#line 551 "y.tab.c" /* yacc.c:355  */
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



/* Copy the second part of user declarations.  */

#line 582 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
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
#define YYLAST   231

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  97
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  177

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   303

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
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
      45,    46,    47,    48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   370,   370,   370,   374,   376,   381,   386,   388,   389,
     391,   392,   392,   393,   395,   396,   397,   398,   399,   400,
     401,   402,   404,   405,   406,   407,   408,   409,   410,   416,
     417,   418,   419,   420,   421,   422,   424,   426,   427,   428,
     429,   431,   432,   434,   437,   438,   439,   441,   447,   452,
     457,   465,   467,   468,   469,   471,   472,   473,   476,   477,
     480,   481,   483,   485,   486,   488,   490,   491,   492,   493,
     494,   494,   496,   497,   499,   499,   500,   502,   502,   503,
     505,   505,   506,   508,   508,   511,   512,   514,   514,   514,
     516,   516,   517,   517,   518,   518,   519,   521
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_EndOfFile", "T_Number", "T_Cln",
  "T_NL", "T_IN", "T_NEQ", "T_EQ", "T_GT", "T_LT", "T_EGT", "T_ELT",
  "T_Or", "T_And", "ID", "ND", "DD", "T_String", "Trip_Quote", "T_Import",
  "T_MN", "T_PL", "T_DV", "T_ML", "T_OP", "T_CP", "T_OB", "T_CB", "T_Def",
  "T_Comma", "T_Range", "T_List", "T_ID", "T_EQL", "T_LEN", "T_True",
  "T_False", "T_Not", "T_Pass", "T_Break", "T_Return", "T_Print", "T_If",
  "T_Elif", "T_Else", "T_For", "T_While", "$accept", "StartDebugger",
  "$@1", "constant", "list_index", "term", "StartParse", "$@2",
  "basic_stmt", "arith_exp", "bool_exp", "bool_term", "bool_factor",
  "import_stmt", "pass_stmt", "break_stmt", "return_stmt", "assign_stmt",
  "print_stmt", "finalStatements", "cmpd_stmt", "if_stmt", "elif_stmts",
  "else_stmt", "for_stmt", "while_stmt", "range_stmt", "start_suite",
  "$@3", "suite", "end_suite", "$@4", "args", "$@5", "args_list", "$@6",
  "func_def", "$@7", "list_stmt", "call_list", "$@8", "call_args", "$@9",
  "$@10", "$@11", "func_call", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303
};
# endif

#define YYPACT_NINF -121

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-121)))

#define YYTABLE_NINF -14

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -121,    30,    71,  -121,  -121,    71,  -121,    37,    15,    19,
      51,    24,  -121,  -121,     7,  -121,  -121,  -121,    11,    19,
      60,    19,  -121,  -121,  -121,    45,  -121,     4,  -121,    -5,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,    90,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,    88,  -121,   204,    73,  -121,
     103,   184,    19,  -121,    15,     4,   121,   128,   132,  -121,
     106,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      19,    19,   138,  -121,  -121,   116,   115,     2,    14,     4,
    -121,  -121,  -121,   118,   159,    54,   159,  -121,    98,   148,
     148,   148,   148,   148,    79,    79,  -121,  -121,   144,  -121,
    -121,    71,   112,  -121,  -121,  -121,  -121,  -121,   119,    72,
    -121,  -121,  -121,    77,   123,   147,   169,  -121,    57,  -121,
    -121,   149,   146,   146,   146,  -121,   152,   117,    19,   177,
    -121,  -121,   179,   159,   159,   153,   181,    15,  -121,  -121,
    -121,  -121,   111,   182,   159,     8,  -121,  -121,   155,  -121,
     159,  -121,   117,  -121,  -121,  -121,   159,  -121,  -121,   186,
    -121,  -121,   146,   111,   117,    77,    47,   153,  -121,  -121,
    -121,  -121,  -121,   187,  -121,   165,  -121
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     4,     0,     5,     0,     0,     0,
       0,     7,    39,    40,     0,    44,    45,    46,     0,     0,
       0,     0,     8,     9,    22,     0,    52,    18,    19,    36,
      37,    16,    14,    15,    21,    17,    20,     0,    53,    55,
      57,    56,    54,    10,    43,     7,    27,     0,     0,    83,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     3,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    11,    28,    42,     0,     0,     0,     7,    47,
      48,    50,    49,     0,     0,     0,     0,    35,     0,    38,
      32,    30,    34,    33,    24,    23,    26,    25,     0,    29,
      31,     0,    79,     6,    92,    94,    85,    90,     0,    96,
      51,    70,    69,    58,     0,     0,     0,    65,     0,    12,
      77,     0,    89,    89,    89,    86,     0,     0,     0,     0,
      59,    60,     0,     0,     0,    82,     0,     0,    93,    95,
      91,    97,    76,     0,     0,     0,    64,    63,     0,    78,
       0,    87,     0,    74,    71,    73,     0,    62,    66,     0,
      80,    84,    89,    76,     0,     0,     0,    82,    88,    72,
      75,    61,    67,     0,    81,     0,    68
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -121,  -121,  -121,  -121,  -121,    -7,    -3,  -121,    -2,    -1,
       3,    63,   180,  -121,  -121,  -121,  -121,  -121,  -121,  -120,
    -121,  -121,    39,  -121,  -121,  -121,  -121,   -61,  -121,    32,
    -121,  -121,  -121,  -121,    38,  -121,  -121,  -121,  -121,  -119,
    -121,    99,  -121,  -121,  -121,  -121
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    22,    23,    24,    25,   101,   112,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   130,   131,    40,    41,   116,   113,   127,   154,
     155,   164,   121,   135,   149,   167,    42,    75,    81,   138,
     162,   108,   124,   122,   123,    82
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      26,    46,    43,    26,   139,   140,   104,   142,    47,    70,
      71,    60,    48,    61,    62,    63,    64,    65,    55,     4,
      55,   105,    56,     4,    58,   117,    66,    67,    68,    69,
       3,   106,   163,    52,     6,   158,   107,    54,     6,   159,
     109,     8,    50,   168,   170,     9,    14,    83,    59,    45,
      79,    55,    50,    45,    80,    48,    12,    13,    14,    51,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      98,    44,   146,   147,   172,     4,   104,     5,   173,    66,
      67,    68,    69,   157,    73,    49,   114,   118,   115,   161,
       6,   105,     7,     8,    57,   165,    72,     9,   119,    26,
      74,    10,     4,    68,    69,    11,   107,    76,    12,    13,
      14,    15,    16,    17,    18,    19,    50,     6,    20,    21,
       8,     4,   128,   129,    88,    26,    84,    55,   152,   153,
     151,   143,    45,    99,   100,    85,     6,    86,     7,     8,
      87,   -13,   102,     9,   103,   110,   120,    10,   125,   132,
      26,    11,   133,    61,    12,    13,    14,    15,    16,    17,
      18,    19,    26,     4,    20,    21,    66,    67,    68,    69,
      66,    67,    68,    69,   134,   111,   136,   137,     6,   141,
       7,     8,   144,   145,   148,     9,   150,   156,     4,   160,
     166,   175,   176,    11,    53,   169,    12,    13,    14,    15,
      16,    17,    18,     6,   171,   174,     8,     0,   126,     0,
       9,    60,    77,    61,    62,    63,    64,    65,    78,     0,
       0,    12,    13,    14,     0,     0,    66,    67,    68,    69,
       0,    73
};

static const yytype_int16 yycheck[] =
{
       2,     8,     5,     5,   123,   124,     4,   127,     9,    14,
      15,     7,     9,     9,    10,    11,    12,    13,    19,     4,
      21,    19,    19,     4,    21,    86,    22,    23,    24,    25,
       0,    29,   152,    26,    19,    27,    34,    26,    19,    31,
      26,    22,    28,   162,   164,    26,    39,    54,     3,    34,
      51,    52,    28,    34,    51,    52,    37,    38,    39,    35,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    34,   133,   134,    27,     4,     4,     6,    31,    22,
      23,    24,    25,   144,    27,    34,    32,    88,    34,   150,
      19,    19,    21,    22,    34,   156,     6,    26,   101,   101,
      27,    30,     4,    24,    25,    34,    34,     4,    37,    38,
      39,    40,    41,    42,    43,    44,    28,    19,    47,    48,
      22,     4,    45,    46,    26,   127,     5,   128,    17,    18,
     137,   128,    34,    70,    71,     7,    19,     5,    21,    22,
      34,     3,    26,    26,    29,    27,    34,    30,    29,    26,
     152,    34,     5,     9,    37,    38,    39,    40,    41,    42,
      43,    44,   164,     4,    47,    48,    22,    23,    24,    25,
      22,    23,    24,    25,     5,    16,    27,    31,    19,    27,
      21,    22,     5,     4,    31,    26,     5,     5,     4,    34,
       4,     4,    27,    34,    14,   163,    37,    38,    39,    40,
      41,    42,    43,    19,   165,   167,    22,    -1,   109,    -1,
      26,     7,    28,     9,    10,    11,    12,    13,    34,    -1,
      -1,    37,    38,    39,    -1,    -1,    22,    23,    24,    25,
      -1,    27
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    50,    51,     0,     4,     6,    19,    21,    22,    26,
      30,    34,    37,    38,    39,    40,    41,    42,    43,    44,
      47,    48,    52,    53,    54,    55,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      73,    74,    85,    55,    34,    34,    54,    58,    59,    34,
      28,    35,    26,    61,    26,    58,    59,    34,    59,     3,
       7,     9,    10,    11,    12,    13,    22,    23,    24,    25,
      14,    15,     6,    27,    27,    86,     4,    28,    34,    58,
      59,    87,    94,    54,     5,     7,     5,    34,    26,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    60,
      60,    56,    26,    29,     4,    19,    29,    34,    90,    26,
      27,    16,    57,    76,    32,    34,    75,    76,    58,    55,
      34,    81,    92,    93,    91,    29,    90,    77,    45,    46,
      71,    72,    26,     5,     5,    82,    27,    31,    88,    88,
      88,    27,    68,    59,     5,     4,    76,    76,    31,    83,
       5,    54,    17,    18,    78,    79,     5,    76,    27,    31,
      34,    76,    89,    68,    80,    76,     4,    84,    88,    78,
      68,    71,    27,    31,    83,     4,    27
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    51,    50,    52,    52,    53,    54,    54,    54,
      55,    56,    55,    55,    57,    57,    57,    57,    57,    57,
      57,    57,    58,    58,    58,    58,    58,    58,    58,    59,
      59,    59,    59,    59,    59,    59,    59,    60,    60,    60,
      60,    61,    61,    62,    63,    64,    65,    66,    66,    66,
      66,    67,    68,    68,    68,    69,    69,    69,    70,    70,
      71,    71,    72,    73,    73,    74,    75,    75,    75,    76,
      77,    76,    78,    78,    80,    79,    79,    82,    81,    81,
      84,    83,    83,    86,    85,    87,    87,    89,    88,    88,
      91,    90,    92,    90,    93,    90,    90,    94
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     4,     1,     1,     1,
       2,     0,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     3,     1,
       1,     2,     3,     2,     1,     1,     1,     3,     3,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     4,     5,
       1,     5,     3,     6,     6,     4,     4,     6,     8,     1,
       0,     4,     3,     1,     0,     3,     0,     0,     3,     0,
       0,     4,     0,     0,     8,     2,     3,     0,     4,     0,
       0,     3,     0,     3,     0,     3,     0,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

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

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 370 "parser_file.y" /* yacc.c:1646  */
    {init();}
#line 1896 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 370 "parser_file.y" /* yacc.c:1646  */
    {printf("\nValid Python Syntax\n"); 
printf("\n\n---------------------------------AST-----------------------------------"); AST_print((yyvsp[-1].node));   
printSTable(); freeAll(); exit(0);}
#line 1904 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 374 "parser_file.y" /* yacc.c:1646  */
    {insertRecord("Constant", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);
					  (yyval.node) = make_leaf((yyvsp[0].text));}
#line 1911 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 376 "parser_file.y" /* yacc.c:1646  */
    {insertRecord("Constant", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);
		 			  (yyval.node) = make_leaf((yyvsp[0].text));}
#line 1918 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 381 "parser_file.y" /* yacc.c:1646  */
    {
									  checkList((yyvsp[-3].text), (yylsp[-3]).first_line, currentScope);
									  (yyval.node) = make_node("ListIndex", make_leaf((yyvsp[-3].text)), make_leaf((yyvsp[-1].text)));
									  }
#line 1927 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 386 "parser_file.y" /* yacc.c:1646  */
    {modifyRecordID("Identifier", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); 
		(yyval.node) = make_leaf("Identifier");}
#line 1934 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 388 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 389 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1946 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 391 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1952 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 392 "parser_file.y" /* yacc.c:1646  */
    {resetDepth();}
#line 1958 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 392 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("Start", (yyvsp[-3].node), (yyvsp[0].node));}
#line 1964 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 393 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node);}
#line 1970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 395 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 396 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 397 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 398 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 1994 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 399 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 400 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2006 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 401 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2012 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 402 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 404 "parser_file.y" /* yacc.c:1646  */
    {printf("\nIn arith_exp->term"); (yyval.node)=(yyvsp[0].node);}
#line 2024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 405 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("+",(yyvsp[-2].node), (yyvsp[0].node));}
#line 2030 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 406 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("-",(yyvsp[-2].node), (yyvsp[0].node));}
#line 2036 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 407 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("*",(yyvsp[-2].node), (yyvsp[0].node));}
#line 2042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 408 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("/",(yyvsp[-2].node), (yyvsp[0].node));}
#line 2048 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 409 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("temp", make_leaf("-"), (yyvsp[0].node));}
#line 2054 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 410 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[-1].node);}
#line 2060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 416 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("or", (yyvsp[-2].node), (yyvsp[0].node));}
#line 2066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 417 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("<", (yyvsp[-2].node), (yyvsp[0].node));}
#line 2072 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 418 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("and", (yyvsp[-2].node), (yyvsp[0].node));}
#line 2078 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 419 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node(">",(yyvsp[-2].node), (yyvsp[0].node));}
#line 2084 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 420 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("<=", (yyvsp[-2].node), (yyvsp[0].node));}
#line 2090 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 421 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node(">=",(yyvsp[-2].node), (yyvsp[0].node));}
#line 2096 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 422 "parser_file.y" /* yacc.c:1646  */
    {checkList((yyvsp[0].text), (yylsp[0]).first_line, currentScope); 
         (yyval.node) = make_node("in", (yyvsp[-2].node), make_leaf("$<text>3"));}
#line 2103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 424 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2109 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 426 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 2115 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 427 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("==", (yyvsp[-2].node), (yyvsp[0].node));}
#line 2121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 428 "parser_file.y" /* yacc.c:1646  */
    {insertRecord("Constant", "True", (yylsp[0]).first_line, currentScope); (yyval.node) = make_leaf("True");}
#line 2127 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 429 "parser_file.y" /* yacc.c:1646  */
    {insertRecord("Constant", "False", (yylsp[0]).first_line, currentScope); (yyval.node) = make_leaf("False");}
#line 2133 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 431 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("!", make_leaf("end"), (yyvsp[0].node));}
#line 2139 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 432 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);}
#line 2145 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 434 "parser_file.y" /* yacc.c:1646  */
    {insertRecord("PackageName", (yyvsp[0].text), (yylsp[0]).first_line, currentScope); 
							  (yyval.node) = make_node("import_stmt", make_leaf("import"), make_leaf((yyvsp[0].text)) );}
#line 2152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 437 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=make_leaf("pass");}
#line 2158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 438 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=make_leaf("break");}
#line 2164 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 439 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=make_leaf("return");}
#line 2170 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 442 "parser_file.y" /* yacc.c:1646  */
    {
			  insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope); 
			  //$1 = make_leaf($<text>1);
			  (yyval.node)=make_node("=",make_leaf((yyvsp[-2].text)),(yyvsp[0].node));
			  }
#line 2180 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 448 "parser_file.y" /* yacc.c:1646  */
    {
			 	insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);
			 	(yyval.node) = make_node("=", make_leaf((yyvsp[-2].text)), (yyvsp[0].node));
			 }
#line 2189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 453 "parser_file.y" /* yacc.c:1646  */
    {
			 	insertRecord("Identifier", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);
			 	(yyval.node) = make_node("=", make_leaf((yyvsp[-2].text)), (yyvsp[0].node));
			 }
#line 2198 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 458 "parser_file.y" /* yacc.c:1646  */
    {
			 	insertRecord("ListTypeID", (yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);
			 	(yyval.node) = make_node("=", make_leaf((yyvsp[-2].text)), (yyvsp[0].node));
			 }
#line 2207 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 465 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=make_node("print_stmt", (yyvsp[-1].node), make_leaf("NULL"));}
#line 2213 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 471 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 2219 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 472 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 2225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 473 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 2231 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 476 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)= make_node("IF", (yyvsp[-2].node), (yyvsp[0].node));}
#line 2237 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 477 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)= make_node("IF-ELIF-ELSE", make_node("IF", (yyvsp[-3].node), (yyvsp[-1].node)), (yyvsp[0].node));}
#line 2243 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 480 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)= (yyvsp[0].node);}
#line 2249 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 481 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_for_node("ELIF", (yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node), make_leaf("NULL"));}
#line 2255 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 483 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("ELSE", (yyvsp[0].node), make_leaf("NULL"));}
#line 2261 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 485 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) =make_node("FOR", make_leaf("range"), (yyvsp[0].node));}
#line 2267 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 486 "parser_file.y" /* yacc.c:1646  */
    {checkList((yyvsp[-2].text), (yylsp[-2]).first_line, currentScope);(yyval.node) =make_node("FOR", make_leaf((yyvsp[-2].text)), (yyvsp[0].node));}
#line 2273 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 488 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) =make_node("WHILE", (yyvsp[-2].node), (yyvsp[0].node));}
#line 2279 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 493 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node)=(yyvsp[0].node);}
#line 2285 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 494 "parser_file.y" /* yacc.c:1646  */
    { initNewTable((yyvsp[0].depth)); updateCScope((yyvsp[0].depth));}
#line 2291 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 494 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) =make_node("start_suite",  (yyvsp[-1].node), (yyvsp[0].node));}
#line 2297 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 496 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("Next", (yyvsp[-1].node), (yyvsp[0].node));}
#line 2303 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 497 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 2309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 499 "parser_file.y" /* yacc.c:1646  */
    {updateCScope((yyvsp[0].depth));}
#line 2315 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 499 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("EndBlock", (yyvsp[0].node), make_leaf("NULL"));}
#line 2321 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 500 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_leaf("EndBlock"); resetDepth();}
#line 2327 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 502 "parser_file.y" /* yacc.c:1646  */
    {addToList((yyvsp[0].text), 1);}
#line 2333 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 502 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_leaf(argsList); clearArgsList();}
#line 2339 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 503 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_leaf("null");}
#line 2345 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 505 "parser_file.y" /* yacc.c:1646  */
    {addToList((yyvsp[0].text), 0);}
#line 2351 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 506 "parser_file.y" /* yacc.c:1646  */
    {addToList("",0);}
#line 2357 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 508 "parser_file.y" /* yacc.c:1646  */
    {insertRecord("Func_Name", (yyvsp[0].text), (yylsp[0]).first_line, currentScope);}
#line 2363 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 509 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_for_node("Func_def", make_leaf((yyvsp[-6].text)), (yyvsp[-3].node), (yyvsp[0].node), make_leaf("NULL"));}
#line 2369 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 511 "parser_file.y" /* yacc.c:1646  */
    { (yyval.node) = make_leaf("[]"); }
#line 2375 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 512 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_leaf("LIST");}
#line 2381 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 514 "parser_file.y" /* yacc.c:1646  */
    {addToList((yyvsp[-1].text), 0);}
#line 2387 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 516 "parser_file.y" /* yacc.c:1646  */
    {addToList((yyvsp[0].text), 1);}
#line 2393 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 516 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_leaf(argsList); clearArgsList();}
#line 2399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 517 "parser_file.y" /* yacc.c:1646  */
    {addToList((yyvsp[0].text), 1);}
#line 2405 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 517 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_leaf(argsList); clearArgsList();}
#line 2411 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 518 "parser_file.y" /* yacc.c:1646  */
    {addToList((yyvsp[0].text), 1);}
#line 2417 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 518 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_leaf(argsList); clearArgsList();}
#line 2423 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 519 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_leaf("null");}
#line 2429 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 521 "parser_file.y" /* yacc.c:1646  */
    {(yyval.node) = make_node("Func_Call", make_leaf((yyvsp[-3].text)), (yyvsp[-1].node));}
#line 2435 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2439 "y.tab.c" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
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
                  yystos[*yyssp], yyvsp, yylsp);
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
#line 523 "parser_file.y" /* yacc.c:1906  */


void yyerror(const char *msg)
{
	//printSTable();
	printf("\n\n%s", msg);
	printf("\nSyntax Error at Line %d, Column : %d\n",  yylineno, yylloc.first_line);
	printSTable();
	exit(0);
}


