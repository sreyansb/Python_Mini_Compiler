/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 270 "copy_parser_file2.y" /* yacc.c:1909  */
struct symtabnode* data;struct nodeyacc* node;
		

#line 150 "y.tab.h" /* yacc.c:1909  */
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
