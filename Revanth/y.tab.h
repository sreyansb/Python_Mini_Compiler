/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
#line 278 "parser.y"
 char *text; int depth; struct AST *node; 

#line 158 "y.tab.h"

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
