
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LOAD = 258,
     STORE = 259,
     REG = 260,
     ADD = 261,
     SUB = 262,
     COMMA = 263,
     LABEL = 264,
     CMP = 265,
     GOTO = 266,
     ADDRESS = 267,
     FACTOR = 268,
     MUL = 269,
     RETLINE = 270,
     CALL = 271,
     FUNCTION = 272,
     ENTERCYCLE = 273,
     EXITCYCLE = 274,
     NUMERIC = 275,
     NAME = 276
   };
#endif
/* Tokens.  */
#define LOAD 258
#define STORE 259
#define REG 260
#define ADD 261
#define SUB 262
#define COMMA 263
#define LABEL 264
#define CMP 265
#define GOTO 266
#define ADDRESS 267
#define FACTOR 268
#define MUL 269
#define RETLINE 270
#define CALL 271
#define FUNCTION 272
#define ENTERCYCLE 273
#define EXITCYCLE 274
#define NUMERIC 275
#define NAME 276




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 6 "yacc_myprof.y"

 int value;
 char *str;



/* Line 1676 of yacc.c  */
#line 101 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


