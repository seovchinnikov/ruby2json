/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     tCLASS = 258,
     tMODULE = 259,
     tDEF = 260,
     tUNDEF = 261,
     tBEGIN = 262,
     tRESCUE = 263,
     tENSURE = 264,
     tEND = 265,
     tIF = 266,
     tUNLESS = 267,
     tTHEN = 268,
     tELSIF = 269,
     tELSE = 270,
     tCASE = 271,
     tWHEN = 272,
     tWHILE = 273,
     tUNTIL = 274,
     tFOR = 275,
     tBREAK = 276,
     tNEXT = 277,
     tREDO = 278,
     tRETRY = 279,
     tIN = 280,
     tDO = 281,
     tDO_COND = 282,
     tDO_BLOCK = 283,
     tRETURN = 284,
     tYIELD = 285,
     tKWAND = 286,
     tKWOR = 287,
     tKWNOT = 288,
     tALIAS = 289,
     tDEFINED = 290,
     upBEGIN = 291,
     upEND = 292,
     tTRUE = 293,
     tFALSE = 294,
     tNIL = 295,
     tENCODING = 296,
     tDSTAR = 297,
     tFILE = 298,
     tLINE = 299,
     tSELF = 300,
     tSUPER = 301,
     GLOBAL = 302,
     BASE = 303,
     CONST = 304,
     tDO_LAMBDA = 305,
     tCHAR = 306,
     tIMAGINARY = 307,
     IVAR = 308,
     CVAR = 309,
     tINTEGER = 310,
     tFLOAT = 311,
     tNTH_REF = 312,
     tBACKTICK = 313,
     tpEND = 314,
     tSYMBEG = 315,
     tRATIONAL = 316,
     tAMPER = 317,
     tAREF = 318,
     tASET = 319,
     tASSOC = 320,
     tCOLON2 = 321,
     tCOLON3 = 322,
     tLAMBDA = 323,
     tLAMBEG = 324,
     tLBRACE = 325,
     tLBRACKET = 326,
     tLPAREN = 327,
     tLPAREN_ARG = 328,
     tSTAR = 329,
     tCOMMENT = 330,
     ARRAY = 331,
     tKEY = 332,
     SYMBOL = 333,
     tUMINUS_NUM = 334,
     tSTRING_BEG = 335,
     tSTRING_CONTENT = 336,
     tSTRING_DBEG = 337,
     tSTRING_DEND = 338,
     tSTRING_END = 339,
     tSTRING_DVAR = 340,
     tLOWEST = 341,
     tLBRACE_ARG = 342,
     modifier_until = 343,
     modifier_while = 344,
     modifier_unless = 345,
     modifier_if = 346,
     tOP_ASGN = 347,
     modifier_rescue = 348,
     tDOT3 = 349,
     tDOT2 = 350,
     tOR = 351,
     tAND = 352,
     tNMATCH = 353,
     tMATCH = 354,
     tNEQ = 355,
     tEQQ = 356,
     tEQ = 357,
     tCMP = 358,
     tLEQ = 359,
     tGEQ = 360,
     tRSHIFT = 361,
     tLSHIFT = 362,
     tUMINUS = 363,
     tPOW = 364,
     tUPLUS = 365
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2053 of yacc.c  */
#line 218 "parser.y"

    struct node *n;
    int num;
    struct term_t *term;


/* Line 2053 of yacc.c  */
#line 174 "parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (struct parser_t *parser);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_H_INCLUDED  */
