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
    T_EOF = 261,
    T_EQ = 262,
    T_Comma = 263,
    T_Del = 264,
    T_Pass = 265,
    T_Break = 266,
    T_Continue = 267,
    T_In = 268,
    T_Print = 269,
    T_Import = 270,
    T_From = 271,
    T_Star = 272,
    T_LP = 273,
    T_RP = 274,
    T_Cln = 275,
    T_For = 276,
    T_While = 277,
    T_Or = 278,
    T_Range = 279,
    T_And = 280,
    T_Not = 281,
    T_Lt = 282,
    T_Gt = 283,
    T_Lte = 284,
    T_Gte = 285,
    T_Deq = 286,
    T_Plus = 287,
    T_Minus = 288,
    T_Divide = 289,
    T_Mod = 290,
    T_DDiv = 291,
    T_Power = 292,
    T_Ls = 293,
    T_Rs = 294,
    T_True = 295,
    T_False = 296,
    T_ID = 297,
    T_Integer = 298,
    T_Real = 299,
    T_String = 300
  };
#endif
/* Tokens.  */
#define T_NL 258
#define T_IND 259
#define T_DED 260
#define T_EOF 261
#define T_EQ 262
#define T_Comma 263
#define T_Del 264
#define T_Pass 265
#define T_Break 266
#define T_Continue 267
#define T_In 268
#define T_Print 269
#define T_Import 270
#define T_From 271
#define T_Star 272
#define T_LP 273
#define T_RP 274
#define T_Cln 275
#define T_For 276
#define T_While 277
#define T_Or 278
#define T_Range 279
#define T_And 280
#define T_Not 281
#define T_Lt 282
#define T_Gt 283
#define T_Lte 284
#define T_Gte 285
#define T_Deq 286
#define T_Plus 287
#define T_Minus 288
#define T_Divide 289
#define T_Mod 290
#define T_DDiv 291
#define T_Power 292
#define T_Ls 293
#define T_Rs 294
#define T_True 295
#define T_False 296
#define T_ID 297
#define T_Integer 298
#define T_Real 299
#define T_String 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 48 "parser_file.y" /* yacc.c:1909  */
struct symtabnode* data;

#line 147 "y.tab.h" /* yacc.c:1909  */
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
