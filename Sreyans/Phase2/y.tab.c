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
#line 1 "parser_file2.y" /* yacc.c:339  */

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

	void makequads(char* s)
	{
		printf("\n****************************\n");
		printf("%s",s);
		printf("\n****************************\n");
	}


#line 164 "y.tab.c" /* yacc.c:339  */

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

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
    T_NL = 258,
    T_IND = 259,
    T_DED = 260,
    T_SAI = 261,
    T_EOF = 262,
    T_EQ = 263,
    T_Comma = 264,
    T_Del = 265,
    T_Pass = 266,
    T_Break = 267,
    T_Continue = 268,
    T_In = 269,
    T_Print = 270,
    T_Import = 271,
    T_From = 272,
    T_Star = 273,
    T_LP = 274,
    T_RP = 275,
    T_Cln = 276,
    T_For = 277,
    T_While = 278,
    T_Or = 279,
    T_Range = 280,
    T_And = 281,
    T_Not = 282,
    T_Lt = 283,
    T_Gt = 284,
    T_Lte = 285,
    T_Gte = 286,
    T_Deq = 287,
    T_Plus = 288,
    T_Minus = 289,
    T_Divide = 290,
    T_Mod = 291,
    T_DDiv = 292,
    T_Power = 293,
    T_Ls = 294,
    T_Rs = 295,
    T_True = 296,
    T_False = 297,
    T_ID = 298,
    T_Integer = 299,
    T_Real = 300,
    T_String = 301
  };
#endif
/* Tokens.  */
#define T_NL 258
#define T_IND 259
#define T_DED 260
#define T_SAI 261
#define T_EOF 262
#define T_EQ 263
#define T_Comma 264
#define T_Del 265
#define T_Pass 266
#define T_Break 267
#define T_Continue 268
#define T_In 269
#define T_Print 270
#define T_Import 271
#define T_From 272
#define T_Star 273
#define T_LP 274
#define T_RP 275
#define T_Cln 276
#define T_For 277
#define T_While 278
#define T_Or 279
#define T_Range 280
#define T_And 281
#define T_Not 282
#define T_Lt 283
#define T_Gt 284
#define T_Lte 285
#define T_Gte 286
#define T_Deq 287
#define T_Plus 288
#define T_Minus 289
#define T_Divide 290
#define T_Mod 291
#define T_DDiv 292
#define T_Power 293
#define T_Ls 294
#define T_Rs 295
#define T_True 296
#define T_False 297
#define T_ID 298
#define T_Integer 299
#define T_Real 300
#define T_String 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 101 "parser_file2.y" /* yacc.c:355  */
struct symtabnode* data;struct nodeyacc* node;
		

#line 300 "y.tab.c" /* yacc.c:355  */
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

/* Copy the second part of user declarations.  */

#line 331 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  61
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   236

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  155

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

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
      45,    46
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   115,   115,   121,   123,   127,   132,   142,   150,   157,
     166,   168,   172,   178,   182,   186,   190,   194,   198,   202,
     208,   214,   222,   227,   230,   233,   234,   237,   240,   245,
     256,   264,   270,   275,   282,   288,   294,   299,   304,   309,
     314,   319,   328,   334,   340,   345,   352,   357,   373,   381,
     382,   385,   386,   387,   388,   389,   392,   399,   408,   414,
     420,   426,   436,   442,   448,   454,   462,   468,   473,   476,
     507,   532,   557,   561,   569,   576,   585,   590,   603,   613,
     618,   629,   635,   642
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_NL", "T_IND", "T_DED", "T_SAI",
  "T_EOF", "T_EQ", "T_Comma", "T_Del", "T_Pass", "T_Break", "T_Continue",
  "T_In", "T_Print", "T_Import", "T_From", "T_Star", "T_LP", "T_RP",
  "T_Cln", "T_For", "T_While", "T_Or", "T_Range", "T_And", "T_Not", "T_Lt",
  "T_Gt", "T_Lte", "T_Gte", "T_Deq", "T_Plus", "T_Minus", "T_Divide",
  "T_Mod", "T_DDiv", "T_Power", "T_Ls", "T_Rs", "T_True", "T_False",
  "T_ID", "T_Integer", "T_Real", "T_String", "$accept", "start_maro",
  "start_karo", "term", "math_term", "stmt", "simple_stmt", "base_stmt",
  "pass_stmt", "delete_stmt", "import_stmt", "import_from",
  "end_import_from", "cobr_stmt", "assign_stmt", "print_stmt",
  "printable_stmt", "arith_stmt", "bool_stmt", "bool_term", "comp_op",
  "list_stmt", "args", "items", "compound_stmt", "for_stmt", "range_stmt",
  "while_stmt", "block_code", "repeater", YY_NULLPTR
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
     295,   296,   297,   298,   299,   300,   301
};
# endif

#define YYPACT_NINF -133

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-133)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       6,     6,   -38,  -133,  -133,  -133,   -11,   -31,   -23,   156,
      -3,   156,   156,   142,  -133,  -133,    26,  -133,  -133,  -133,
      44,    70,  -133,  -133,     6,  -133,  -133,  -133,  -133,  -133,
    -133,  -133,  -133,  -133,  -133,   195,  -133,    32,  -133,  -133,
    -133,  -133,  -133,  -133,   135,  -133,    78,  -133,   175,    76,
      84,   195,    96,  -133,  -133,   118,   118,   118,   118,   104,
     135,  -133,  -133,  -133,   -13,  -133,  -133,  -133,  -133,  -133,
     -13,   -13,   -13,   -13,   -13,   -13,    47,    47,   109,   112,
    -133,  -133,    14,    89,   146,  -133,  -133,  -133,  -133,  -133,
    -133,   -13,  -133,   122,   122,  -133,  -133,  -133,    85,  -133,
    -133,  -133,   147,   144,   139,   143,   145,   169,  -133,  -133,
     118,   118,   118,   118,    39,   151,  -133,   103,    89,    89,
      89,   126,  -133,  -133,  -133,  -133,   147,    34,    77,  -133,
    -133,  -133,    68,  -133,   153,  -133,   140,  -133,   126,    68,
     210,   106,   141,    68,  -133,  -133,   176,  -133,   174,  -133,
    -133,   200,   201,  -133,  -133
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     5,     0,    20,    27,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,     7,     9,     8,     6,
       0,     0,    48,    41,     5,    10,    12,    13,    14,    15,
      23,    16,    17,    18,    19,    31,    32,    44,    33,    11,
      67,    68,     3,    21,     0,    22,     0,     7,     0,     0,
       0,     0,     0,    45,    56,    66,    66,    66,    66,     0,
       0,     1,     2,     4,     0,    51,    52,    54,    55,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,    46,     0,     0,     0,    61,    60,    59,    58,    57,
      29,     0,    36,    34,    35,    37,    39,    38,    47,    42,
      43,    30,    26,     0,     0,     0,     0,     0,    79,    78,
      66,    66,    66,    66,     0,     0,    24,     0,     0,     0,
       0,     0,    65,    64,    63,    62,    26,     0,     0,    71,
      70,    69,    83,    25,     0,    73,     0,    72,     0,    83,
       0,     0,     0,    83,    82,    80,     0,    75,     0,    74,
      81,     0,     0,    77,    76
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -133,  -133,     3,   152,  -133,   -97,  -133,   -82,  -133,  -133,
    -133,  -133,   107,  -133,  -133,  -133,   -34,    -9,   205,    75,
    -133,   154,  -133,   -43,  -133,  -133,  -133,  -133,     5,  -132
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   116,    31,    32,    33,    34,    35,    36,    37,
      75,    38,    59,    85,    39,    40,   106,    41,   109,   140
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      48,   108,    51,    51,    42,    43,    91,   144,    44,     1,
      78,   150,    45,    86,    87,    88,     2,     3,     4,     5,
      46,     6,     7,     8,   132,     9,    90,    63,    10,    11,
      47,    17,    18,    12,    60,   139,   108,   108,   108,   103,
      50,   143,   139,   134,    61,    13,   139,    14,    15,    16,
      17,    18,    19,    13,   135,    92,    76,    64,    77,    80,
      19,    93,    94,    95,    96,    97,    98,   122,   123,   124,
     125,   138,    70,    71,    72,    73,    74,    62,     2,     3,
       4,     5,   114,     6,     7,     8,   136,     9,    14,    15,
      10,    11,   107,    19,    79,    12,    81,   137,    82,     2,
       3,     4,     5,    64,     6,     7,     8,    13,     9,    14,
      15,    16,    17,    18,    19,   146,    12,    83,    70,    71,
      72,    73,    74,   129,   130,   131,   147,    84,    13,   101,
      14,    15,    16,    17,    18,    19,     2,     3,     4,     5,
      64,     6,     7,     8,    89,     9,   127,   128,    10,    11,
     148,    99,   100,    12,     9,   102,   115,    72,    73,    74,
     118,   149,    12,   117,   119,    13,   120,    14,    15,    16,
      17,    18,    19,   121,    13,     9,    14,    15,    47,    17,
      18,    19,    54,    12,   142,    55,    56,    57,    58,   110,
     111,   112,   113,    64,   126,    80,   141,    14,    15,    47,
      17,    18,    19,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    64,    49,   145,    52,    53,   152,   151,
     153,   154,     0,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,   133,   104,     0,   105
};

static const yytype_int16 yycheck[] =
{
       9,    83,    11,    12,     1,    43,    19,   139,    19,     3,
      44,   143,    43,    56,    57,    58,    10,    11,    12,    13,
      43,    15,    16,    17,   121,    19,    60,    24,    22,    23,
      43,    44,    45,    27,     8,   132,   118,   119,   120,    25,
      43,   138,   139,     9,     0,    39,   143,    41,    42,    43,
      44,    45,    46,    39,    20,    64,    24,    18,    26,    20,
      46,    70,    71,    72,    73,    74,    75,   110,   111,   112,
     113,     3,    33,    34,    35,    36,    37,     7,    10,    11,
      12,    13,    91,    15,    16,    17,     9,    19,    41,    42,
      22,    23,     3,    46,    16,    27,    20,    20,    14,    10,
      11,    12,    13,    18,    15,    16,    17,    39,    19,    41,
      42,    43,    44,    45,    46,     9,    27,    21,    33,    34,
      35,    36,    37,   118,   119,   120,    20,     9,    39,    20,
      41,    42,    43,    44,    45,    46,    10,    11,    12,    13,
      18,    15,    16,    17,    40,    19,    43,    44,    22,    23,
       9,    76,    77,    27,    19,    43,     9,    35,    36,    37,
      21,    20,    27,    19,    21,    39,    21,    41,    42,    43,
      44,    45,    46,     4,    39,    19,    41,    42,    43,    44,
      45,    46,    40,    27,    44,    43,    44,    45,    46,    43,
      44,    45,    46,    18,    43,    20,    43,    41,    42,    43,
      44,    45,    46,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    18,     9,     5,    11,    12,    44,    43,
      20,    20,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,   126,    82,    -1,    82
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    10,    11,    12,    13,    15,    16,    17,    19,
      22,    23,    27,    39,    41,    42,    43,    44,    45,    46,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    60,    61,    62,    63,    64,    65,    66,    68,    71,
      72,    74,    49,    43,    19,    43,    43,    43,    64,    65,
      43,    64,    65,    65,    40,    43,    44,    45,    46,    69,
       8,     0,     7,    49,    18,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    67,    24,    26,    63,    16,
      20,    20,    14,    21,     9,    70,    70,    70,    70,    40,
      63,    19,    64,    64,    64,    64,    64,    64,    64,    66,
      66,    20,    43,    25,    50,    68,    73,     3,    54,    75,
      43,    44,    45,    46,    64,     9,    59,    19,    21,    21,
      21,     4,    70,    70,    70,    70,    43,    43,    44,    75,
      75,    75,    52,    59,     9,    20,     9,    20,     3,    52,
      76,    43,    44,    52,    76,     5,     9,    20,     9,    20,
      76,    43,    44,    20,    20
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    49,    49,    49,    50,    51,    51,    51,
      52,    52,    53,    54,    54,    54,    54,    54,    54,    54,
      55,    56,    57,    57,    58,    59,    59,    60,    60,    61,
      62,    63,    63,    63,    64,    64,    64,    64,    64,    64,
      64,    64,    65,    65,    65,    65,    65,    65,    66,    66,
      66,    67,    67,    67,    67,    67,    68,    68,    69,    69,
      69,    69,    70,    70,    70,    70,    70,    71,    71,    72,
      72,    72,    73,    73,    73,    73,    73,    73,    74,    75,
      75,    76,    76,    76
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     5,     3,     0,     1,     1,     3,
       4,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     3,     1,     2,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     3,     2,     2,
       2,     2,     3,     3,     3,     3,     0,     1,     1,     6,
       6,     6,     4,     4,     6,     6,     8,     8,     4,     1,
       5,     3,     2,     0
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
#line 115 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
                        strcpy((yyval.node->code),(yyvsp[-1].node->code));
                        printf("\nAccepted Code : Valid\n\n");printTable();
                        makequads((yyval.node->code));}
#line 1633 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 121 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
                        strcpy((yyval.node->code),(yyvsp[0].node->code));}
#line 1640 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 123 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
                       strcat((yyvsp[-1].node->code),(yyvsp[0].node->code));
					   strcpy((yyval.node->code),(yyvsp[-1].node->code));
                    	}
#line 1649 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 127 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
			  strcpy((yyval.node->code),"");
              }
#line 1657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 132 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
                temp_gen((yyval.node->addr));
				snprintf(code_temp,4200,"(%s=%s)\n",(yyval.node->addr),(yyvsp[0].data->name));
				strcpy((yyval.node->code),code_temp);
				//strcpy($<node->code>$,code_temp);
				(yyval.node->leng)=strlen((yyvsp[0].data->name));
				(yyval.node->value)=(strcmp((yyvsp[0].data->name),"\"\"") && strcmp((yyvsp[0].data->name),"\'\'"));
				}
#line 1670 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 142 "parser_file2.y" /* yacc.c:1646  */
    { 
			searchele((yyvsp[0].data->name),(yyvsp[0].data->scope));
            (yyval.node)=malloc(sizeof(struct nodeyacc));
			strcpy((yyval.node->addr),(yyvsp[0].data->name));
			(yyval.node->value)=(yyvsp[0].node->value);
			strcpy((yyval.node->code),(yyvsp[0].data->name));
			
			}
#line 1683 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 150 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
			  (yyval.node->value)=atoi((yyvsp[0].data->name));
			  temp_gen((yyval.node->addr));
			  snprintf(code_temp,2000,"(%s = %s)\n",(yyval.node->addr),(yyvsp[0].data->name));
			  strcpy((yyval.node->code),code_temp);
			  
			  }
#line 1695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 157 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
			  	(yyval.node->value)=atoi((yyvsp[0].data->name));
			  	temp_gen((yyval.node->addr));
			  	snprintf(code_temp,2000,"(%s = %s)\n",(yyval.node->addr),(yyvsp[0].data->name));
			  	strcpy((yyval.node->code),code_temp);
			  }
#line 1707 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 166 "parser_file2.y" /* yacc.c:1646  */
    { (yyval.node)=malloc(sizeof(struct nodeyacc));
					strcpy((yyval.node->code),(yyvsp[0].node->code));}
#line 1714 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 168 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
					strcpy((yyval.node->code),(yyvsp[0].node->code));}
#line 1721 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 172 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),(yyvsp[0].node->code));
				}
#line 1730 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 178 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),(yyvsp[0].node->code));
				}
#line 1739 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 182 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),(yyvsp[0].node->code));
				}
#line 1748 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 186 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),(yyvsp[0].node->code));
				}
#line 1757 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 190 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),(yyvsp[0].node->code));
				}
#line 1766 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 194 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),(yyvsp[0].node->code));
				}
#line 1775 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 198 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),(yyvsp[0].node->code));
				}
#line 1784 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 202 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),(yyvsp[0].node->code));
				}
#line 1793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 208 "parser_file2.y" /* yacc.c:1646  */
    {
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),"PASS\n");
			}
#line 1802 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 214 "parser_file2.y" /* yacc.c:1646  */
    {
				if (searchele((yyvsp[0].data->name),(yyvsp[0].data->scope))==0)
					{printf("Invalid Code\n");exit(0);}
				(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),"DELETE\n");
			}
#line 1813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 222 "parser_file2.y" /* yacc.c:1646  */
    {
					searchele((yyvsp[0].data->name),(yyvsp[0].data->scope));
					(yyval.node)=malloc(sizeof(struct nodeyacc));
					strcpy((yyval.node->code),"IMPORT\n");
					}
#line 1823 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 237 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
				strcpy((yyval.node->code),"BREAK\n");
			  }
#line 1831 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 240 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
				  strcpy((yyval.node->code),"CONTINUE\n");
			    }
#line 1839 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 245 "parser_file2.y" /* yacc.c:1646  */
    {
								searchele((yyvsp[-2].data->name),(yyvsp[-2].data->scope));
								(yyval.node)=malloc(sizeof(struct nodeyacc));
								snprintf(temp,3000,"%s=%s\n",(yyvsp[-2].data->name),(yyvsp[0].node->addr));//generate function
								strcat((yyvsp[0].node->code),temp);
								strcpy((yyval.node->code),(yyvsp[0].node->code));
								(yyvsp[-2].node->value)=(yyvsp[0].node->value);
								(yyval.node->value)=(yyvsp[0].node->value);
								}
#line 1853 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 256 "parser_file2.y" /* yacc.c:1646  */
    {
										snprintf(code_temp,4200,"PRINT %s\n",(yyvsp[-1].node->addr));
										(yyval.node)=malloc(sizeof(struct nodeyacc));
										strcat((yyvsp[-1].node->code),code_temp);
										strcpy((yyval.node->code),(yyvsp[-1].node->code));
									   }
#line 1864 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 264 "parser_file2.y" /* yacc.c:1646  */
    {
				  (yyval.node)=malloc(sizeof(struct nodeyacc));
				  (yyval.node->value)=(yyvsp[0].node->value);
				  strcpy((yyval.node->code),(yyvsp[0].node->code));
				  strcpy((yyval.node->addr),(yyvsp[0].node->addr));
				  }
#line 1875 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 270 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
				  (yyval.node->value)=(yyvsp[0].node->value);
				  strcpy((yyval.node->code),(yyvsp[0].node->code));
				  strcpy((yyval.node->addr),(yyvsp[0].node->addr));
				  }
#line 1885 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 275 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
				  (yyval.node->value)=(yyvsp[0].node->value);
				  strcpy((yyval.node->code),(yyvsp[0].node->code));
				  strcpy((yyval.node->addr),(yyvsp[0].node->addr));
				  }
#line 1895 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 282 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
									temp_gen((yyval.node->addr));//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s=%s+%s\n",(yyval.node->addr),(yyvsp[-2].node->addr),(yyvsp[0].node->addr));
									strcpy((yyval.node->code),code_temp);
									(yyval.node->value)=(yyvsp[-2].node->value)+(yyvsp[0].node->value);
									}
#line 1906 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 288 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
									temp_gen((yyval.node->addr));//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope
									snprintf(code_temp,4200,"%s=%s-%s\n",(yyval.node->addr),(yyvsp[-2].node->addr),(yyvsp[0].node->addr));
									strcpy((yyval.node->code),code_temp);
									(yyval.node->value)=(yyvsp[-2].node->value)-(yyvsp[0].node->value);
									}
#line 1917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 294 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));temp_gen((yyval.node->addr));//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope
									snprintf(code_temp,4200,"%s=%s*%s\n",(yyval.node->addr),(yyvsp[-2].node->addr),(yyvsp[0].node->addr));
									strcpy((yyval.node->code),code_temp);
									(yyval.node->value)=(yyvsp[-2].node->value)*(yyvsp[0].node->value);
									}
#line 1927 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 299 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));temp_gen((yyval.node->addr));//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s=%s/%s\n",(yyval.node->addr),(yyvsp[-2].node->addr),(yyvsp[0].node->addr));
									strcpy((yyval.node->code),code_temp);
									(yyval.node->value)=(yyvsp[-2].node->value)/(yyvsp[0].node->value);
									}
#line 1937 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 304 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));temp_gen((yyval.node->addr));//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s=%s//%s\n",(yyval.node->addr),(yyvsp[-2].node->addr),(yyvsp[0].node->addr));
									strcpy((yyval.node->code),code_temp);
									(yyval.node->value)=(yyvsp[-2].node->value)/(yyvsp[0].node->value);
									}
#line 1947 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 309 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));temp_gen((yyval.node->addr));//generates a temporary and also adds to symbol table with scope = -1, cant keep track of scope 
									snprintf(code_temp,4200,"%s=%s%%%s\n",(yyval.node->addr),(yyvsp[-2].node->addr),(yyvsp[0].node->addr));
									strcpy((yyval.node->code),code_temp);
									(yyval.node->value)=(yyvsp[-2].node->value)%(yyvsp[0].node->value);
									}
#line 1957 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 314 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
							(yyval.node->value)=(yyvsp[-1].node->value);
							strcpy((yyval.node->code),(yyvsp[-1].node->code));
							strcpy((yyval.node->addr),(yyvsp[-1].node->addr));
							}
#line 1967 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 319 "parser_file2.y" /* yacc.c:1646  */
    {
				 
				 (yyval.node)=malloc(sizeof(struct nodeyacc));
				 (yyval.node->value)=(yyvsp[0].node->value);
				 strcpy((yyval.node->code),(yyvsp[0].node->code));
				 strcpy((yyval.node->addr),(yyvsp[0].node->addr));
				 }
#line 1979 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 328 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
								(yyval.node->value)=(yyvsp[-2].node->value)||(yyvsp[0].node->value);
								temp_gen((yyval.node->addr));
								snprintf(code_temp,4200,"%s=%s OR %s\n",(yyval.node->addr),(yyvsp[-2].node->addr),(yyvsp[0].node->addr));
								strcpy((yyval.node->code),code_temp);
								}
#line 1990 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 334 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
								(yyval.node->value)=(yyvsp[-2].node->value)&&(yyvsp[0].node->value);
								temp_gen((yyval.node->addr));
								snprintf(code_temp,4200,"%s=%s AND %s\n",(yyval.node->addr),(yyvsp[-2].node->addr),(yyvsp[0].node->addr));
								strcpy((yyval.node->code),code_temp);
								}
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 340 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
				(yyval.node->value)=(yyvsp[0].node->value);
				strcpy((yyval.node->code),(yyvsp[0].node->code));
				strcpy((yyval.node->addr),(yyvsp[0].node->addr));
				}
#line 2011 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 345 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
						(yyval.node->value)=!((yyvsp[0].node->value));
					   temp_gen((yyval.node->addr));
					   snprintf(code_temp,4200,"%s=NOT(%s)\n",(yyval.node->addr),(yyvsp[0].node->addr));
					   strcpy((yyval.node->code),code_temp);
					   //strncpy($<code>$,code_temp,1999);
						}
#line 2023 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 352 "parser_file2.y" /* yacc.c:1646  */
    { (yyval.node)=malloc(sizeof(struct nodeyacc));
							(yyval.node->value)=((yyvsp[-1].node->value));
							strcpy((yyval.node->code),(yyvsp[-1].node->code));
							strcpy((yyval.node->addr),(yyvsp[-2].node->addr));
							}
#line 2033 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 357 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
									(yyval.node->value)=0;
									temp_gen((yyval.node->addr));
									switch((yyvsp[-1].node->value))
									{
										case 1:{(yyval.node->value)=((yyvsp[-2].node->value) < (yyvsp[0].node->value));break;}
										case 2:{(yyval.node->value)=((yyvsp[-2].node->value) > (yyvsp[0].node->value));break;}
										case 3:{(yyval.node->value)=((yyvsp[-2].node->value) == (yyvsp[0].node->value));break;}
										case 4:{(yyval.node->value)=((yyvsp[-2].node->value) <= (yyvsp[0].node->value));break;}
										case 5:{(yyval.node->value)=((yyvsp[-2].node->value) >= (yyvsp[0].node->value));break;}
									}
									snprintf(temp,3000,"%s=%s %s %s\n",(yyval.node->addr),(yyvsp[-2].node->addr),(yyvsp[-1].node->code),(yyvsp[0].node->addr));
									strcpy((yyval.node->code),temp);
									}
#line 2052 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 373 "parser_file2.y" /* yacc.c:1646  */
    {
			(yyval.node)=malloc(sizeof(struct nodeyacc));
			(yyval.node->value)=(yyvsp[0].node->value);
			strcpy((yyval.node->code),(yyvsp[0].node->code));
			//strcpy($<code>$,$<code>1);
			strcpy((yyval.node->addr),(yyvsp[0].node->addr));
			
			}
#line 2065 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 381 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));(yyval.node->value)=1;strcpy((yyval.node->code),"TRUE\n");}
#line 2071 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 382 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));(yyval.node->value)=0;strcpy((yyval.node->code),"FALSE\n");}
#line 2077 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 385 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));strcpy((yyval.node->code),"<");(yyval.node->value)=1;}
#line 2083 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 386 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));strcpy((yyval.node->code),">");(yyval.node->value)=2;}
#line 2089 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 387 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));strcpy((yyval.node->code),"==");(yyval.node->value)=3;}
#line 2095 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 388 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));strcpy((yyval.node->code),"<=");(yyval.node->value)=4;}
#line 2101 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 389 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));strcpy((yyval.node->code),">=");(yyval.node->value)=5;}
#line 2107 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 392 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
				 (yyval.node->leng)=0;
				 (yyval.node->value)=0;
				 temp_gen((yyval.node->addr));
				 snprintf(code_temp,4200,"%s = []\n",(yyval.node->addr));
				 strcpy((yyval.node->code),code_temp);
				 }
#line 2119 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 399 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
					  (yyval.node->leng)=(yyvsp[-1].node->leng);
					  temp_gen((yyval.node->addr));
					  (yyval.node->value)=(yyvsp[-1].node->leng);
					  snprintf(code_temp,4200,"%s = [%s]\n",(yyval.node->addr),(yyvsp[-1].node->code));
					  strcpy((yyval.node->code),code_temp);
					  }
#line 2131 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 408 "parser_file2.y" /* yacc.c:1646  */
    {
						(yyval.node)=malloc(sizeof(struct nodeyacc));
						(yyval.node->leng)=(yyvsp[0].node->leng)+1;
						snprintf(code_temp,4200,"%s%s",(yyvsp[-1].node->code),(yyvsp[0].node->code));
						strcpy((yyval.node->code),code_temp);
					 }
#line 2142 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 414 "parser_file2.y" /* yacc.c:1646  */
    {
						(yyval.node)=malloc(sizeof(struct nodeyacc));
						(yyval.node->leng)=(yyvsp[0].node->leng)+1;
						snprintf(code_temp,4200,"%s%s",(yyvsp[-1].data->name),(yyvsp[0].node->code));
						strcpy((yyval.node->code),code_temp);
					 }
#line 2153 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 420 "parser_file2.y" /* yacc.c:1646  */
    {
						(yyval.node)=malloc(sizeof(struct nodeyacc));
						(yyval.node->leng)=(yyvsp[0].node->leng)+1;
						snprintf(code_temp,4200,"%s%s",(yyvsp[-1].data->name),(yyvsp[0].node->code));
						strcpy((yyval.node->code),code_temp);
					 }
#line 2164 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 426 "parser_file2.y" /* yacc.c:1646  */
    {
				 if (searchele((yyvsp[-1].data->name),(yyvsp[-1].data->scope))==0)
				 	{printf("\nERROR : %s Not Defined\n",yytext);return 0;}
				 (yyval.node)=malloc(sizeof(struct nodeyacc));
				 (yyval.node->leng)=(yyvsp[0].node->leng)+1;
				 snprintf(code_temp,4200,"%s%s",(yyvsp[-1].data->name),(yyvsp[0].node->code));
				 strcpy((yyval.node->code),code_temp);
				 }
#line 2177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 436 "parser_file2.y" /* yacc.c:1646  */
    {
								(yyval.node)=malloc(sizeof(struct nodeyacc));
								(yyval.node->leng)=(yyvsp[-1].node->leng)+1;
								snprintf(code_temp,4200,",%s%s",(yyvsp[-1].data->name),(yyvsp[0].node->code));
								strcpy((yyval.node->code),code_temp);
					 		 }
#line 2188 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 442 "parser_file2.y" /* yacc.c:1646  */
    {
								(yyval.node)=malloc(sizeof(struct nodeyacc));
								(yyval.node->leng)=(yyvsp[-1].node->leng)+1;
								snprintf(code_temp,4200,",%s%s",(yyvsp[-1].data->name),(yyvsp[0].node->code));
								strcpy((yyval.node->code),code_temp);
					 		 }
#line 2199 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 448 "parser_file2.y" /* yacc.c:1646  */
    {
								(yyval.node)=malloc(sizeof(struct nodeyacc));
								(yyval.node->leng)=(yyvsp[-1].node->leng)+1;
								snprintf(code_temp,4200,",%s%s",(yyvsp[-1].data->name),(yyvsp[0].node->code));
								strcpy((yyval.node->code),code_temp);
					 		  }
#line 2210 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 454 "parser_file2.y" /* yacc.c:1646  */
    {
							if (searchele((yyvsp[-1].data->name),(yyvsp[-1].data->scope))==0)
						   		{printf("\nERROR : %s Not Defined\n",yytext);return 0;}
							(yyval.node)=malloc(sizeof(struct nodeyacc));
							(yyval.node->leng)=(yyvsp[-1].node->leng)+1;
							snprintf(code_temp,4200,",%s%s",(yyvsp[-1].data->name),(yyvsp[0].node->code));
							strcpy((yyval.node->code),code_temp);
						 }
#line 2223 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 462 "parser_file2.y" /* yacc.c:1646  */
    {
			  (yyval.node)=malloc(sizeof(struct nodeyacc));
			  (yyval.node->leng)=0;
			  strcpy((yyval.node->code),"");
			  }
#line 2233 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 468 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));strcpy((yyval.node->code),(yyvsp[0].node->code));
				printf("\n***************\n");
				printf("%s",(yyvsp[0].node->code));
				printf("\n***************\n");
				}
#line 2243 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 473 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));strcpy((yyval.node->code),(yyvsp[0].node->code));}
#line 2249 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 476 "parser_file2.y" /* yacc.c:1646  */
    {
													searchele((yyvsp[-4].data->name),(yyvsp[-4].data->scope));
													snprintf(code_temp,4200,"%s = %d\n",(yyvsp[-4].data->name),(yyvsp[-2].node->start));//make the initial value
													(yyval.node)=malloc(sizeof(struct nodeyacc));
													strcpy((yyval.node->code),code_temp);

													char labelif[10];char labelb[10];//labels for if and true block
													label_gen(labelif);
													label_gen(labelb);
													char tempcre[10];temp_gen(tempcre);

													//print the loop condition
													//printf("\nRANGE STMT : %s\n",labelb);
													snprintf(temp,3000,"%s : %s=(%s < %d)\nIF %s GOTO %s\n",labelif,tempcre,(yyvsp[-4].data->name),(yyvsp[-2].node->end),tempcre,labelb);
													//printf("\nLABEL B : %s",labelb);
													strcat((yyval.node->code),temp);

													//print the loop block
													snprintf(temp,3000,"%s : %s",labelb,(yyvsp[0].node->code));
													strcat((yyval.node->code),temp);

													//make the increment function													
													char tempinc[10];
													temp_gen(tempinc);
													snprintf(temp,3000,"%s=%s+%d\n%s=%s\nGOTO %s\n",tempinc,(yyvsp[-4].data->name),(yyvsp[-2].node->step),(yyvsp[-4].data->name),tempinc,labelif);
													//printf("\nRANGE\n%s\n",$<node->code>$);
													strcat((yyval.node->code),temp);
													//printf("\nRANGE : \n %s \nFINISHED\n",$<node->code>$);
													//printf("\nDRANGE : \n %s \nDFINISHED\n",$<node->code>$);
													}
#line 2284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 507 "parser_file2.y" /* yacc.c:1646  */
    {	searchele((yyvsp[-4].data->name),(yyvsp[-4].data->scope));
													snprintf(code_temp,4200,"%s\n%s = %s\n",(yyvsp[-2].node->code),(yyvsp[-4].data->name),(yyvsp[-2].node->addr));
													(yyval.node)=malloc(sizeof(struct nodeyacc));
													strcpy((yyval.node->code),code_temp);

													char labelif[10];char labelb[10];//labels for if and true block
													label_gen(labelif);
													label_gen(labelb);
													char tempcre[10];temp_gen(tempcre);

													snprintf(temp,3000,"%s : %s=(%s < %s+%d)\nIF %s GOTO %s\n",labelif,tempcre,(yyvsp[-4].data->name),(yyvsp[-2].node->addr),(yyvsp[-2].node->leng),tempcre,labelb);
													strcat((yyval.node->code),temp);

													//print the loop block
													
													snprintf(temp,3000,"%s : %s\n",labelb,(yyvsp[0].node->code));
													strcat((yyval.node->code),temp);

													char tempinc[10];
													temp_gen(tempinc);
													snprintf(temp,3000,"%s=%s+1\n%s=%s\nGOTO %s\n",tempinc,(yyvsp[-4].data->name),(yyvsp[-4].data->name),tempinc,labelif);
													strcat((yyval.node->code),temp);
													//printf("\n%s\n",$<node->code>$);															
													}
#line 2313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 532 "parser_file2.y" /* yacc.c:1646  */
    {		searchele((yyvsp[-4].data->name),(yyvsp[-4].data->scope));
													snprintf(code_temp,4200,"%s\n%s = %s\n",(yyvsp[-2].node->code),(yyvsp[-4].data->name),(yyvsp[-2].node->addr));
													(yyval.node)=malloc(sizeof(struct nodeyacc));
													strcpy((yyval.node->code),code_temp);

													char labelif[10];char labelb[10];//labels for if and true block
													label_gen(labelif);
													label_gen(labelb);
													char tempcre[10];temp_gen(tempcre);
													snprintf(temp,3000,"%s : %s=(%s < %d)\nIF %s GOTO %s\n",labelif,tempcre,(yyvsp[-4].data->name),(yyvsp[-2].node->leng),tempcre,labelb);
													strcat((yyval.node->code),temp);

													//print the loop block
													snprintf(temp,3000,"%s : %s\n",labelb,(yyvsp[0].node->code));
													strcat((yyval.node->code),temp);

													char tempinc[10];
													temp_gen(tempinc);
													snprintf(temp,3000,"%s=%s+1\n%s=%s\nGOTO %s\n",tempinc,(yyvsp[-4].data->name),(yyvsp[-4].data->name),tempinc,labelif);
													strcat((yyval.node->code),temp);
																								
													}
#line 2340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 557 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
									(yyval.node->start)=0;
									(yyval.node->end)=atoi((yyvsp[-1].data->name));
									(yyval.node->step)=1;}
#line 2349 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 561 "parser_file2.y" /* yacc.c:1646  */
    {		
									if (searchdo((yyvsp[-1].data->name))==0)
										{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
									(yyval.node)=malloc(sizeof(struct nodeyacc));
									(yyval.node->start)=0;
									(yyval.node->end)=(yyvsp[-1].node->value);
									(yyval.node->step)=1;
							 }
#line 2362 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 569 "parser_file2.y" /* yacc.c:1646  */
    {	
														(yyval.node)=malloc(sizeof(struct nodeyacc));
														(yyval.node->start)=atoi((yyvsp[-3].data->name));
														(yyval.node->end)=atoi((yyvsp[-1].data->name));
														(yyval.node->step)=1;
														//printf("\nRANGE : %d %d %d \n",$<node->start>$,$<node->end>$,$<node->step>$);
														}
#line 2374 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 576 "parser_file2.y" /* yacc.c:1646  */
    {
											if (searchdo((yyvsp[-3].data->name))==0)
												{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
											if (searchdo((yyvsp[-1].data->name))==0)
												{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
											(yyval.node)=malloc(sizeof(struct nodeyacc));
											(yyval.node->start)=(yyvsp[-3].node->value);
											(yyval.node->end)=(yyvsp[-1].node->value);
											(yyval.node->step)=1;}
#line 2388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 585 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));
																		(yyval.node->start)=atoi((yyvsp[-5].data->name));
																		(yyval.node->end)=atoi((yyvsp[-3].data->name));
																		(yyval.node->step)=atoi((yyvsp[-1].data->name));
																	  }
#line 2398 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 590 "parser_file2.y" /* yacc.c:1646  */
    {
														if (searchdo((yyvsp[-5].data->name))==0)
															{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
														if (searchdo((yyvsp[-3].data->name))==0)
															{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
														if (searchdo((yyvsp[-1].data->name))==0)
															{printf("\nIDENTIFIER NOT DEFINED\n");return 0;}
														(yyval.node)=malloc(sizeof(struct nodeyacc));
														(yyval.node->start)=(yyvsp[-5].node->value);
														(yyval.node->end)=(yyvsp[-3].node->value);
														(yyval.node->step)=(yyvsp[-1].node->value);}
#line 2414 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 603 "parser_file2.y" /* yacc.c:1646  */
    {
										  char labelif[10];char labelb[10];//labels for if and true block
										  label_gen(labelif);
										  label_gen(labelb);
										  (yyval.node)=malloc(sizeof(struct nodeyacc));
										  snprintf(code_temp,4200,"%s : IF (%s) GOTO %s\n%s : %s\nGOTO %s\n",labelif,(yyvsp[-2].node->code),labelb,labelb,(yyvsp[0].node->code),labelif);
										  strcpy((yyval.node->code),code_temp);
										  }
#line 2427 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 613 "parser_file2.y" /* yacc.c:1646  */
    {
				 (yyval.node)=malloc(sizeof(struct nodeyacc));
				 strcpy((yyval.node->code),(yyvsp[0].node->code));
				 //printf("\nBLOCK CODE :%s\n",$<node->code>$);
				}
#line 2437 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 618 "parser_file2.y" /* yacc.c:1646  */
    {
										// printf("\nIN BLOCK CODDE\n");
										// printf("%s\n",$<node->code>3);
										(yyval.node)=malloc(sizeof(struct nodeyacc));
										snprintf(code_temp,4200,"%s%s\n",(yyvsp[-2].node->code),(yyvsp[-1].node->code));
										strcpy((yyval.node->code),code_temp);
										// printf("%s",$<node->code>$);
										// printf("FINISH\n");
									 }
#line 2451 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 629 "parser_file2.y" /* yacc.c:1646  */
    {
							(yyval.node)=malloc(sizeof(struct nodeyacc));
							snprintf(code_temp,4200,"%s%s\n",(yyvsp[-1].node->code),(yyvsp[0].node->code));
							strcpy((yyval.node->code),code_temp);
							// printf("\nREPEATER BIG\n%s \nDONE\n %s\nFIN\n",$<node->code>2,$<node->code>3);
							}
#line 2462 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 635 "parser_file2.y" /* yacc.c:1646  */
    {
						//printf("\nHERE\n");
						(yyval.node)=malloc(sizeof(struct nodeyacc));
						snprintf(code_temp,4200,"%s%s\n",(yyvsp[-1].node->code),(yyvsp[0].node->code));
						strcpy((yyval.node->code),code_temp);
						//printf("\nREPEATER\n%s \nDONE\n %s\nFIN\n",$<node->code>1,$<node->code>2);
						}
#line 2474 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 642 "parser_file2.y" /* yacc.c:1646  */
    {(yyval.node)=malloc(sizeof(struct nodeyacc));strcpy((yyval.node->code),"");}
#line 2480 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2484 "y.tab.c" /* yacc.c:1646  */
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
#line 643 "parser_file2.y" /* yacc.c:1906  */
