/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison implementation for Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.7.12-4996"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 27 "parser.y"

/* копирование строки */
extern char *strdup(const char *s);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"


#define SSIZE 256
#define LSIZE (SSIZE << 2)


/* Маски состояний лексера */
enum lex_state_bits {
    EXPR_BEG_bit,               /* ignore newline, +/- is a sign. */
    EXPR_END_bit,               /* newline significant, +/- is an operator. */
    EXPR_ENDARG_bit,            /* ditto, and unbound braces. */
    EXPR_ENDFN_bit,             /* ditto, and unbound braces. */
    EXPR_ARG_bit,               /* newline significant, +/- is an operator. */
    EXPR_CMDARG_bit,            /* newline significant, +/- is an operator. */
    EXPR_MID_bit,               /* newline significant, +/- is an operator. */
    EXPR_FNAME_bit,             /* ignore newline, no reserved words. */
    EXPR_DOT_bit,               /* right after `.' or `::', no reserved words. */
    EXPR_CLASS_bit,             /* immediate after `class', no here document. */
    EXPR_VALUE_bit,             /* alike EXPR_BEG but label is disallowed. */
};

/* Состояния лексера */
enum lex_state_e {
#define DEF_EXPR(n) EXPR_##n = (1 << EXPR_##n##_bit)
    DEF_EXPR(BEG),
    DEF_EXPR(END),
    DEF_EXPR(ENDARG),
    DEF_EXPR(ENDFN),
    DEF_EXPR(ARG),
    DEF_EXPR(CMDARG),
    DEF_EXPR(MID),
    DEF_EXPR(FNAME),
    DEF_EXPR(DOT),
    DEF_EXPR(CLASS),
    DEF_EXPR(VALUE),
    EXPR_BEG_ANY  =  (EXPR_BEG | EXPR_VALUE | EXPR_MID | EXPR_CLASS),
    EXPR_ARG_ANY  =  (EXPR_ARG | EXPR_CMDARG),
    EXPR_END_ANY  =  (EXPR_END | EXPR_ENDARG | EXPR_ENDFN)
};

/* Макросы для работы с состояниями лексера */
#define IS_lex_state_for(x, ls) ((x) & (ls))
#define IS_lex_state(ls)        IS_lex_state_for(lex_state, (ls))

/* And now some macros that will help us on some stacks of the parser. */
#define BITSTACK_PUSH(stack, n) ((stack) = ((stack)<<1)|((n)&1))
#define BITSTACK_POP(stack)     ((stack) = (stack) >> 1)
#define BITSTACK_LEXPOP(stack)  ((stack) = ((stack) >> 1) | ((stack) & 1))
#define BITSTACK_SET_P(stack)   ((stack)&1)

#define COND_PUSH(n)    BITSTACK_PUSH(parser->cond_stack, (n))
#define COND_POP()      BITSTACK_POP(parser->cond_stack)
#define COND_LEXPOP()   BITSTACK_LEXPOP(parser->cond_stack)
#define COND_P()        BITSTACK_SET_P(parser->cond_stack)

#define CMDARG_PUSH(n)  BITSTACK_PUSH(parser->cmdarg_stack, (n))
#define CMDARG_POP()    BITSTACK_POP(parser->cmdarg_stack)
#define CMDARG_LEXPOP() BITSTACK_LEXPOP(parser->cmdarg_stack)
#define CMDARG_P()      BITSTACK_SET_P(parser->cmdarg_stack)


/*
 * Для обработки string/heredoc/regexp/shortcut .
 */
struct term_t {
    int token;
    char *word;
    int nest;
    unsigned char term;
    unsigned char paren;
    unsigned char can_embed : 1;
    unsigned char nestable : 1;
};

/*
 * Комментарии к коду.
 */
struct comment_t {
    char *comment;
    int line;
};

/*
 * Эта структура содержит все инфо о парсере (AST, флаги и тд).
 */
struct parser_t {
    /* дерево */
    struct node *ast;

    /* Stack of positions */
    struct pos_t *pos_stack;
    int stack_scale;
    int pos_size;

    /* Флаги, используемые парсером */
    unsigned char eof_reached : 1;
    unsigned int cond_stack;
    unsigned int cmdarg_stack;
    int in_def;
    int paren_nest;
    int lpar_beg;
    int parser_command_start;
    enum ruby_version version;

    /* Переменные, используемые лексером */
    enum lex_state_e lex_state;
    struct term_t *lex_strterm;
    char *lex_p; // текущий символ
    char *lex_prev; // пред. символ
    char *lex_pend;
    unsigned long lex_prevc;
	unsigned long str_pos;

    /* Чтобы обрабатывать heredoc */
    unsigned long line_pend;
    unsigned long column_pend;
    unsigned char here_found : 1;

    /* Ошибки */
    struct error_t *errors;
    struct error_t *last_error;
    unsigned char warning : 1;
    unsigned char unrecoverable : 1;

    /* Stack of names */
    char *stack[2];
    char *aux;
	char string_val[1000];
	char string_buf[1000];
	int is_here_end;
    int sp;

    /* The last allocated comment + the comment stack    */
    struct comment_t last_comment;
    char *comment_stack[SSIZE];
    int comment_index;

    /* Инфо о входном потоке */
    unsigned long length;
    unsigned long line;
    unsigned long column;
    unsigned char content_given : 1;
    char *blob;
};

#include "parser.h"
#define yyparse ruby_yyparse
#define YYERROR_VERBOSE 1

/* Синонимы. */
#define lex_strterm parser->lex_strterm
#define lex_state parser->lex_state
#define command_start parser->parser_command_start

/* yy's functions */
#if YYPURE
static int yylex(void *, void *);
#else
static int yylex(void *);
#endif
static void yyerror(struct parser_t *, const char *);
#define yywarning(msg) { parser->warning = 1; yyerror(parser, (msg)); parser->warning = 0;}

/* Работаем со стеком */
static void pop_stack(struct parser_t *parser, struct node *n);
static void push_last_comment(struct parser_t *parser);
static void pop_comment(struct parser_t *parser, struct node *n);
static void pop_pos(struct parser_t *parser, struct node *n);
static void pop_start(struct parser_t *parser, struct node *n);
static void pop_end(struct parser_t *parser, struct node *n);

/* Helper macros for nodes, positions and stacks */
#define ALLOC_N(kind, l, r) alloc_node(kind, l, r); pop_pos(parser, yyval.n);
#define DISPOSE2(node1, node2) { free_ast(node1); free_ast(node2); }
#define DISPOSE3(node1, node2, node3) { DISPOSE2(node1, node2); free_ast(node3); }
#define POP_STACK pop_stack(parser, yyval.n)
#define discard_pos() pop_pos(parser, NULL)
#define copy_op(op) { parser->aux = strdup(op); }

/* Line 371 of yacc.c  */
#line 256 "parser.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
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
   by #include "parser.h".  */
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
/* Line 387 of yacc.c  */
#line 218 "parser.y"

    struct node *n;
    int num;
    struct term_t *term;


/* Line 387 of yacc.c  */
#line 416 "parser.c"
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 443 "parser.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

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
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  293
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   8570

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  137
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  152
/* YYNRULES -- Number of rules.  */
#define YYNRULES  540
/* YYNRULES -- Number of states.  */
#define YYNSTATES  949

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   365

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     136,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   123,     2,     2,     2,   120,   113,     2,
     132,   133,   118,   116,   130,   117,   129,   119,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    96,   135,
     108,    92,   107,    95,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   128,     2,   134,   112,     2,   131,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   126,   111,   127,   124,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    93,    94,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   109,
     110,   114,   115,   121,   122,   125
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    14,    15,    16,
      23,    26,    28,    30,    34,    37,    38,    43,    47,    51,
      54,    58,    62,    66,    70,    74,    75,    81,    86,    90,
      94,    98,   105,   111,   117,   123,   129,   133,   137,   141,
     145,   147,   149,   151,   155,   159,   163,   166,   168,   170,
     172,   174,   179,   184,   189,   192,   196,   201,   207,   212,
     218,   221,   224,   227,   230,   233,   235,   239,   241,   245,
     247,   250,   254,   260,   263,   268,   271,   276,   278,   282,
     284,   288,   291,   295,   297,   301,   303,   308,   312,   316,
     320,   324,   327,   329,   331,   336,   340,   344,   348,   352,
     355,   357,   359,   362,   364,   368,   370,   372,   374,   376,
     378,   380,   382,   383,   388,   390,   392,   394,   396,   398,
     400,   402,   404,   406,   408,   410,   412,   414,   416,   418,
     420,   422,   424,   426,   428,   430,   432,   434,   436,   438,
     440,   442,   444,   446,   448,   450,   452,   454,   456,   458,
     460,   462,   464,   466,   468,   470,   472,   474,   476,   478,
     480,   482,   484,   486,   488,   490,   492,   494,   496,   498,
     500,   502,   504,   506,   508,   510,   512,   514,   516,   518,
     520,   522,   524,   526,   528,   530,   534,   540,   544,   550,
     557,   563,   569,   575,   581,   586,   590,   594,   598,   602,
     606,   610,   614,   618,   622,   627,   630,   633,   637,   641,
     645,   649,   653,   657,   661,   665,   669,   673,   677,   681,
     685,   688,   691,   695,   699,   703,   707,   711,   718,   720,
     722,   725,   730,   733,   737,   739,   741,   743,   745,   747,
     750,   753,   758,   760,   761,   764,   767,   770,   772,   774,
     776,   779,   783,   788,   792,   797,   800,   802,   804,   806,
     808,   812,   813,   818,   822,   826,   829,   831,   835,   839,
     841,   846,   850,   852,   858,   863,   867,   870,   873,   876,
     883,   890,   891,   892,   900,   901,   902,   910,   916,   921,
     922,   923,   933,   934,   941,   949,   950,   956,   957,   964,
     965,   966,   976,   978,   980,   982,   984,   986,   988,   991,
     993,   995,   997,  1003,  1005,  1008,  1010,  1012,  1014,  1018,
    1020,  1024,  1026,  1031,  1038,  1042,  1048,  1051,  1056,  1058,
    1062,  1067,  1070,  1073,  1075,  1078,  1079,  1086,  1095,  1100,
    1107,  1112,  1115,  1122,  1125,  1130,  1137,  1140,  1145,  1148,
    1153,  1155,  1157,  1159,  1163,  1165,  1170,  1172,  1175,  1177,
    1181,  1183,  1185,  1186,  1190,  1195,  1197,  1201,  1205,  1210,
    1213,  1218,  1223,  1226,  1231,  1236,  1240,  1244,  1248,  1251,
    1253,  1258,  1260,  1262,  1267,  1272,  1278,  1280,  1282,  1288,
    1290,  1293,  1295,  1297,  1299,  1301,  1304,  1306,  1309,  1311,
    1313,  1315,  1318,  1320,  1324,  1325,  1328,  1330,  1331,  1332,
    1338,  1339,  1343,  1345,  1347,  1349,  1351,  1354,  1356,  1358,
    1360,  1362,  1364,  1366,  1369,  1371,  1373,  1375,  1377,  1379,
    1381,  1383,  1385,  1387,  1389,  1391,  1393,  1395,  1397,  1399,
    1401,  1403,  1405,  1407,  1408,  1413,  1416,  1420,  1423,  1428,
    1431,  1434,  1436,  1439,  1440,  1447,  1456,  1461,  1468,  1473,
    1480,  1483,  1488,  1495,  1498,  1503,  1506,  1511,  1513,  1515,
    1517,  1519,  1521,  1523,  1525,  1527,  1529,  1533,  1535,  1539,
    1542,  1545,  1547,  1551,  1553,  1557,  1559,  1561,  1564,  1566,
    1567,  1572,  1576,  1578,  1582,  1584,  1588,  1590,  1592,  1595,
    1597,  1599,  1601,  1604,  1607,  1609,  1611,  1612,  1617,  1619,
    1621,  1623,  1626,  1628,  1632,  1636,  1639,  1642,  1644,  1646,
    1648,  1650,  1652,  1654,  1656,  1658,  1660,  1662,  1664,  1665,
    1667,  1668,  1670,  1673,  1676,  1678,  1680,  1682,  1684,  1686,
    1689
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     138,     0,    -1,   139,    -1,   286,    -1,   288,    -1,   145,
      -1,     1,   145,    -1,    -1,    -1,   141,   142,   143,   221,
     197,   225,    -1,   144,   281,    -1,   288,    -1,   145,    -1,
     144,   287,   145,    -1,     1,   145,    -1,    -1,    34,   164,
     146,   164,    -1,    34,    47,    47,    -1,    34,    47,    57,
      -1,     6,   165,    -1,   145,    91,   148,    -1,   145,    90,
     148,    -1,   145,    89,   148,    -1,   145,    88,   148,    -1,
     145,    94,   145,    -1,    -1,    36,   147,   126,   143,   127,
      -1,    37,   126,   143,   127,    -1,   160,    92,   149,    -1,
     153,    92,   149,    -1,   239,    93,   149,    -1,   181,   128,
     173,   284,    93,   149,    -1,   181,   129,   271,    93,   149,
      -1,   181,   129,   270,    93,   149,    -1,   181,    66,   270,
      93,   149,    -1,   181,    66,   271,    93,   149,    -1,   241,
      93,   149,    -1,   160,    92,   180,    -1,   153,    92,   169,
      -1,   153,    92,   180,    -1,   148,    -1,    59,    -1,   149,
      -1,   148,    31,   148,    -1,   148,    32,   148,    -1,    33,
     282,   148,    -1,   123,   149,    -1,   169,    -1,   152,    -1,
     150,    -1,   215,    -1,   215,   129,   276,   175,    -1,   215,
      66,   276,   175,    -1,    87,   205,   143,   127,    -1,   275,
     175,    -1,   275,   175,   151,    -1,   181,   129,   276,   175,
      -1,   181,   129,   276,   175,   151,    -1,   181,    66,   276,
     175,    -1,   181,    66,   276,   175,   151,    -1,    46,   174,
      -1,    30,   174,    -1,    29,   174,    -1,    21,   174,    -1,
      22,   174,    -1,   155,    -1,    72,   154,   283,    -1,   155,
      -1,    72,   154,   283,    -1,   157,    -1,   157,   156,    -1,
     157,    74,   159,    -1,   157,    74,   159,   130,   158,    -1,
     157,    74,    -1,   157,    74,   130,   158,    -1,    74,   159,
      -1,    74,   159,   130,   158,    -1,    74,    -1,    74,   130,
     158,    -1,   159,    -1,    72,   154,   283,    -1,   156,   130,
      -1,   157,   156,   130,    -1,   156,    -1,   158,   130,   156,
      -1,   239,    -1,   181,   128,   173,   284,    -1,   181,   129,
     271,    -1,   181,    66,   271,    -1,   181,   129,   270,    -1,
     181,    66,   270,    -1,    67,   270,    -1,   241,    -1,   239,
      -1,   181,   128,   173,   284,    -1,   181,   129,   271,    -1,
     181,    66,   271,    -1,   181,   129,   270,    -1,   181,    66,
     270,    -1,    67,   270,    -1,    48,    -1,   270,    -1,    67,
     161,    -1,   161,    -1,   181,    66,   161,    -1,   271,    -1,
     270,    -1,   167,    -1,   168,    -1,   163,    -1,   235,    -1,
     164,    -1,    -1,   165,   130,   166,   164,    -1,   111,    -1,
     112,    -1,   113,    -1,   106,    -1,   105,    -1,   104,    -1,
     102,    -1,   101,    -1,   107,    -1,   110,    -1,   108,    -1,
     109,    -1,   103,    -1,   115,    -1,   114,    -1,   116,    -1,
     117,    -1,   118,    -1,    74,    -1,   119,    -1,   120,    -1,
     122,    -1,    63,    -1,   131,    -1,   125,    -1,    64,    -1,
     121,    -1,    42,    -1,   123,    -1,   124,    -1,    44,    -1,
      43,    -1,    41,    -1,    36,    -1,    37,    -1,    34,    -1,
      31,    -1,     7,    -1,    21,    -1,    16,    -1,     3,    -1,
       5,    -1,    35,    -1,    26,    -1,    15,    -1,    14,    -1,
      10,    -1,     9,    -1,    39,    -1,    20,    -1,    25,    -1,
       4,    -1,    22,    -1,    40,    -1,    33,    -1,    32,    -1,
      23,    -1,     8,    -1,    24,    -1,    29,    -1,    45,    -1,
      46,    -1,    13,    -1,    38,    -1,     6,    -1,    17,    -1,
      30,    -1,    11,    -1,    12,    -1,    18,    -1,    19,    -1,
     160,    92,   169,    -1,   160,    92,   169,    94,   169,    -1,
     239,    93,   169,    -1,   239,    93,   169,    94,   169,    -1,
     181,   128,   173,   284,    93,   169,    -1,   181,   129,   271,
      93,   169,    -1,   181,   129,   270,    93,   169,    -1,   181,
      66,   271,    93,   169,    -1,   181,    66,   270,    93,   169,
      -1,    67,   270,    93,   169,    -1,   241,    93,   169,    -1,
     169,    98,   169,    -1,   169,    97,   169,    -1,   169,   116,
     169,    -1,   169,   117,   169,    -1,   169,   118,   169,    -1,
     169,   119,   169,    -1,   169,   120,   169,    -1,   169,   122,
     169,    -1,    79,   238,   122,   169,    -1,   125,   169,    -1,
     121,   169,    -1,   169,   111,   169,    -1,   169,   112,   169,
      -1,   169,   113,   169,    -1,   169,   106,   169,    -1,   169,
     107,   169,    -1,   169,   110,   169,    -1,   169,   108,   169,
      -1,   169,   109,   169,    -1,   169,   105,   169,    -1,   169,
     104,   169,    -1,   169,   103,   169,    -1,   169,   102,   169,
      -1,   169,   101,   169,    -1,   123,   169,    -1,   124,   169,
      -1,   169,   115,   169,    -1,   169,   114,   169,    -1,   169,
     100,   169,    -1,   169,    99,   169,    -1,    35,   282,   169,
      -1,   169,    95,   169,   282,    96,   169,    -1,   181,    -1,
     288,    -1,   179,   285,    -1,   179,   130,   273,   285,    -1,
     273,   285,    -1,   132,   173,   283,    -1,   288,    -1,   171,
      -1,   288,    -1,   174,    -1,   152,    -1,   179,   178,    -1,
     273,   178,    -1,   179,   130,   273,   178,    -1,   177,    -1,
      -1,   176,   174,    -1,    62,   169,    -1,   130,   177,    -1,
     130,    -1,   288,    -1,   169,    -1,    74,   169,    -1,   179,
     130,   169,    -1,   179,   130,    74,   169,    -1,   179,   130,
     169,    -1,   179,   130,    74,   169,    -1,    74,   169,    -1,
     226,    -1,   227,    -1,   239,    -1,   241,    -1,     7,   140,
      10,    -1,    -1,    73,   148,   182,   283,    -1,    72,   143,
     133,    -1,   181,    66,   270,    -1,    67,   270,    -1,    76,
      -1,    71,   170,   134,    -1,    70,   272,   127,    -1,    29,
      -1,    30,   132,   174,   283,    -1,    30,   132,   283,    -1,
      30,    -1,    35,   282,   132,   148,   283,    -1,    33,   132,
     148,   283,    -1,    33,   132,   283,    -1,   275,   218,    -1,
     216,   217,    -1,    68,   210,    -1,    11,   148,   194,   143,
     196,    10,    -1,    12,   148,   194,   143,   197,    10,    -1,
      -1,    -1,    18,   183,   148,   195,   184,   143,    10,    -1,
      -1,    -1,    19,   185,   148,   195,   186,   143,    10,    -1,
      16,   148,   281,   219,    10,    -1,    16,   281,   219,    10,
      -1,    -1,    -1,    20,   198,    25,   187,   148,   195,   188,
     143,    10,    -1,    -1,     3,   162,   242,   189,   140,    10,
      -1,     3,   281,   115,   148,   286,   140,    10,    -1,    -1,
       4,   162,   190,   140,    10,    -1,    -1,     5,   163,   191,
     244,   140,    10,    -1,    -1,    -1,     5,   268,   280,   192,
     163,   193,   244,   140,    10,    -1,    21,    -1,    22,    -1,
      23,    -1,    24,    -1,   286,    -1,    13,    -1,   286,    13,
      -1,   286,    -1,    27,    -1,   197,    -1,    14,   148,   194,
     143,   196,    -1,   288,    -1,    15,   143,    -1,   160,    -1,
     153,    -1,   249,    -1,    72,   201,   283,    -1,   199,    -1,
     200,   130,   199,    -1,   200,    -1,   200,   130,    74,   249,
      -1,   200,   130,    74,   249,   130,   200,    -1,   200,   130,
      74,    -1,   200,   130,    74,   130,   200,    -1,    74,   249,
      -1,    74,   249,   130,   200,    -1,    74,    -1,    74,   130,
     200,    -1,   254,   130,   257,   267,    -1,   254,   267,    -1,
     257,   267,    -1,   266,    -1,   130,   202,    -1,    -1,   251,
     130,   261,   130,   264,   203,    -1,   251,   130,   261,   130,
     264,   130,   251,   203,    -1,   251,   130,   261,   203,    -1,
     251,   130,   261,   130,   251,   203,    -1,   251,   130,   264,
     203,    -1,   251,   130,    -1,   251,   130,   264,   130,   251,
     203,    -1,   251,   203,    -1,   261,   130,   264,   203,    -1,
     261,   130,   264,   130,   251,   203,    -1,   261,   203,    -1,
     261,   130,   251,   203,    -1,   264,   203,    -1,   264,   130,
     251,   203,    -1,   202,    -1,   288,    -1,   206,    -1,   111,
     207,   111,    -1,    99,    -1,   111,   204,   207,   111,    -1,
     288,    -1,   135,   208,    -1,   209,    -1,   208,   130,   209,
      -1,   271,    -1,   248,    -1,    -1,   211,   212,   213,    -1,
     132,   247,   207,   283,    -1,   247,    -1,    69,   143,   127,
      -1,    50,   143,    10,    -1,    28,   205,   143,    10,    -1,
     152,   214,    -1,   215,   129,   276,   172,    -1,   215,    66,
     276,   172,    -1,   275,   171,    -1,   181,   129,   276,   172,
      -1,   181,    66,   276,   171,    -1,   181,    66,   277,    -1,
     181,   129,   171,    -1,   181,    66,   171,    -1,   279,   171,
      -1,   279,    -1,   181,   128,   173,   284,    -1,   288,    -1,
     218,    -1,   126,   205,   143,   127,    -1,    26,   205,   143,
      10,    -1,    17,   179,   194,   143,   220,    -1,   197,    -1,
     219,    -1,     8,   222,   194,   143,   221,    -1,   288,    -1,
     223,   224,    -1,   169,    -1,   180,    -1,   288,    -1,   288,
      -1,    65,   160,    -1,   288,    -1,     9,   143,    -1,   237,
      -1,   235,    -1,   228,    -1,   227,   228,    -1,    51,    -1,
      80,   229,    84,    -1,    -1,   229,   230,    -1,    81,    -1,
      -1,    -1,    82,   231,   232,   143,   127,    -1,    -1,    85,
     233,   234,    -1,   241,    -1,    47,    -1,    53,    -1,    54,
      -1,    60,   236,    -1,   163,    -1,   227,    -1,    47,    -1,
      53,    -1,    54,    -1,   238,    -1,    79,   238,    -1,    55,
      -1,    56,    -1,    61,    -1,    52,    -1,   271,    -1,    47,
      -1,    53,    -1,    54,    -1,   270,    -1,   240,    -1,    40,
      -1,    45,    -1,    38,    -1,    39,    -1,    43,    -1,    44,
      -1,    41,    -1,    57,    -1,   286,    -1,    -1,   108,   243,
     148,   286,    -1,     1,   286,    -1,   132,   247,   283,    -1,
     247,   286,    -1,   255,   130,   257,   267,    -1,   255,   267,
      -1,   257,   267,    -1,   266,    -1,   130,   245,    -1,    -1,
     251,   130,   262,   130,   264,   246,    -1,   251,   130,   262,
     130,   264,   130,   251,   246,    -1,   251,   130,   262,   246,
      -1,   251,   130,   262,   130,   251,   246,    -1,   251,   130,
     264,   246,    -1,   251,   130,   264,   130,   251,   246,    -1,
     251,   246,    -1,   262,   130,   264,   246,    -1,   262,   130,
     264,   130,   251,   246,    -1,   262,   246,    -1,   262,   130,
     251,   246,    -1,   264,   246,    -1,   264,   130,   251,   246,
      -1,   245,    -1,   288,    -1,    49,    -1,    53,    -1,    47,
      -1,    54,    -1,   248,    -1,   271,    -1,   249,    -1,    72,
     201,   283,    -1,   250,    -1,   251,   130,   250,    -1,   278,
     169,    -1,   278,   181,    -1,   253,    -1,   254,   130,   253,
      -1,   252,    -1,   255,   130,   252,    -1,   122,    -1,    42,
      -1,   256,   271,    -1,   256,    -1,    -1,   271,    92,   259,
     169,    -1,   271,    92,   181,    -1,   260,    -1,   261,   130,
     260,    -1,   258,    -1,   262,   130,   258,    -1,   118,    -1,
      74,    -1,   263,   271,    -1,   263,    -1,   113,    -1,    62,
      -1,   265,   271,    -1,   130,   266,    -1,   288,    -1,   239,
      -1,    -1,   132,   269,   148,   283,    -1,    49,    -1,    48,
      -1,   288,    -1,   273,   285,    -1,   274,    -1,   273,   130,
     274,    -1,   169,    65,   169,    -1,   278,   169,    -1,    42,
     169,    -1,   271,    -1,   270,    -1,   271,    -1,   270,    -1,
     167,    -1,   271,    -1,   167,    -1,    77,    -1,    46,    -1,
     129,    -1,    66,    -1,    -1,   287,    -1,    -1,   136,    -1,
     282,   133,    -1,   282,   134,    -1,   282,    -1,   130,    -1,
     135,    -1,   136,    -1,   286,    -1,   287,   135,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   284,   284,   285,   288,   289,   290,   294,   297,   294,
     309,   312,   313,   314,   315,   318,   318,   322,   335,   340,
     344,   348,   352,   356,   360,   365,   364,   375,   381,   382,
     383,   384,   389,   394,   399,   405,   410,   411,   412,   413,
     414,   415,   418,   419,   420,   421,   422,   423,   426,   426,
     429,   430,   435,   442,   449,   453,   457,   462,   467,   472,
     477,   478,   479,   480,   481,   484,   485,   488,   489,   492,
     493,   494,   499,   504,   510,   517,   518,   519,   524,   532,
     533,   536,   537,   540,   541,   544,   545,   549,   550,   551,
     552,   558,   564,   567,   568,   572,   573,   574,   575,   581,
     589,   594,   597,   598,   599,   602,   603,   604,   614,   621,
     621,   624,   625,   625,   628,   628,   628,   629,   629,   629,
     630,   630,   630,   631,   631,   631,   632,   632,   632,   633,
     633,   633,   634,   634,   634,   635,   635,   635,   636,   636,
     637,   637,   638,   638,   641,   641,   641,   641,   641,   641,
     641,   642,   642,   642,   642,   642,   642,   642,   642,   642,
     643,   643,   643,   643,   643,   643,   643,   643,   643,   644,
     644,   644,   644,   644,   644,   644,   644,   644,   645,   645,
     645,   645,   645,   645,   645,   648,   649,   654,   655,   660,
     665,   670,   675,   680,   686,   692,   693,   694,   695,   696,
     697,   698,   699,   700,   701,   706,   707,   708,   709,   710,
     711,   712,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,   723,   724,   725,   726,   727,   728,   732,   735,
     736,   737,   738,   741,   744,   744,   747,   747,   750,   751,
     752,   757,   763,   767,   767,   777,   780,   781,   782,   785,
     786,   787,   788,   791,   792,   793,   796,   797,   798,   799,
     800,   801,   801,   802,   803,   808,   809,   810,   811,   816,
     817,   818,   819,   820,   824,   825,   826,   827,   832,   833,
     838,   843,   843,   843,   848,   848,   848,   853,   858,   863,
     863,   863,   869,   868,   878,   884,   883,   894,   893,   903,
     904,   903,   916,   917,   918,   919,   922,   923,   924,   927,
     927,   930,   931,   937,   938,   941,   941,   944,   945,   948,
     949,   952,   953,   954,   958,   963,   968,   969,   970,   971,
     978,   982,   986,   990,   993,   994,   997,  1001,  1005,  1009,
    1013,  1017,  1018,  1022,  1023,  1027,  1031,  1032,  1036,  1037,
    1041,  1044,  1045,  1052,  1053,  1054,  1057,  1058,  1067,  1068,
    1071,  1072,  1080,  1080,  1091,  1092,  1095,  1101,  1108,  1115,
    1116,  1121,  1128,  1132,  1137,  1142,  1147,  1151,  1155,  1156,
    1157,  1163,  1164,  1167,  1172,  1179,  1185,  1185,  1188,  1193,
    1196,  1202,  1202,  1202,  1205,  1205,  1208,  1209,  1212,  1212,
    1215,  1216,  1219,  1223,  1235,  1236,  1245,  1247,  1251,  1246,
    1263,  1262,  1275,  1276,  1277,  1278,  1281,  1289,  1290,  1291,
    1292,  1293,  1296,  1297,  1303,  1304,  1305,  1313,  1323,  1324,
    1325,  1326,  1327,  1328,  1331,  1332,  1333,  1334,  1335,  1336,
    1337,  1340,  1343,  1345,  1344,  1353,  1356,  1362,  1370,  1377,
    1384,  1391,  1397,  1398,  1401,  1405,  1409,  1413,  1417,  1421,
    1425,  1429,  1433,  1437,  1441,  1445,  1449,  1453,  1454,  1457,
    1458,  1459,  1460,  1463,  1463,  1466,  1467,  1470,  1471,  1474,
    1481,  1488,  1489,  1492,  1493,  1496,  1496,  1499,  1504,  1512,
    1511,  1525,  1528,  1529,  1532,  1533,  1536,  1536,  1539,  1540,
    1543,  1543,  1546,  1549,  1550,  1553,  1554,  1554,  1572,  1575,
    1578,  1579,  1582,  1583,  1586,  1590,  1597,  1606,  1606,  1609,
    1610,  1611,  1618,  1619,  1627,  1630,  1633,  1633,  1636,  1636,
    1639,  1639,  1642,  1645,  1648,  1648,  1651,  1651,  1654,  1654,
    1657
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tCLASS", "tMODULE", "tDEF", "tUNDEF",
  "tBEGIN", "tRESCUE", "tENSURE", "tEND", "tIF", "tUNLESS", "tTHEN",
  "tELSIF", "tELSE", "tCASE", "tWHEN", "tWHILE", "tUNTIL", "tFOR",
  "tBREAK", "tNEXT", "tREDO", "tRETRY", "tIN", "tDO", "tDO_COND",
  "tDO_BLOCK", "tRETURN", "tYIELD", "tKWAND", "tKWOR", "tKWNOT", "tALIAS",
  "tDEFINED", "upBEGIN", "upEND", "tTRUE", "tFALSE", "tNIL", "tENCODING",
  "tDSTAR", "tFILE", "tLINE", "tSELF", "tSUPER", "GLOBAL", "BASE", "CONST",
  "tDO_LAMBDA", "tCHAR", "tIMAGINARY", "IVAR", "CVAR", "tINTEGER",
  "tFLOAT", "tNTH_REF", "tBACKTICK", "tpEND", "tSYMBEG", "tRATIONAL",
  "tAMPER", "tAREF", "tASET", "tASSOC", "tCOLON2", "tCOLON3", "tLAMBDA",
  "tLAMBEG", "tLBRACE", "tLBRACKET", "tLPAREN", "tLPAREN_ARG", "tSTAR",
  "tCOMMENT", "ARRAY", "tKEY", "SYMBOL", "tUMINUS_NUM", "tSTRING_BEG",
  "tSTRING_CONTENT", "tSTRING_DBEG", "tSTRING_DEND", "tSTRING_END",
  "tSTRING_DVAR", "tLOWEST", "tLBRACE_ARG", "modifier_until",
  "modifier_while", "modifier_unless", "modifier_if", "'='", "tOP_ASGN",
  "modifier_rescue", "'?'", "':'", "tDOT3", "tDOT2", "tOR", "tAND",
  "tNMATCH", "tMATCH", "tNEQ", "tEQQ", "tEQ", "tCMP", "'>'", "'<'", "tLEQ",
  "tGEQ", "'|'", "'^'", "'&'", "tRSHIFT", "tLSHIFT", "'+'", "'-'", "'*'",
  "'/'", "'%'", "tUMINUS", "tPOW", "'!'", "'~'", "tUPLUS", "'{'", "'}'",
  "'['", "'.'", "','", "'`'", "'('", "')'", "']'", "';'", "'\\n'",
  "$accept", "top_compstmt", "top_stmt", "bodystmt", "@1", "@2",
  "compstmt", "stmts", "stmt", "$@3", "$@4", "expr", "command_call",
  "block_command", "cmd_brace_block", "command", "mlhs", "mlhs_inner",
  "mlhs_basic", "mlhs_item", "mlhs_head", "mlhs_post", "mlhs_node", "lhs",
  "cname", "cpath", "fname", "fsym", "undef_list", "$@5", "op", "reswords",
  "arg", "aref_args", "paren_args", "opt_paren_args", "opt_call_args",
  "call_args", "command_args", "@6", "block_arg", "opt_block_arg", "args",
  "mrhs", "primary", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13",
  "$@14", "$@15", "$@16", "$@17", "$@18", "then", "do", "if_tail",
  "opt_else", "for_var", "f_marg", "f_marg_list", "f_margs",
  "block_args_tail", "opt_block_args_tail", "block_param",
  "opt_block_param", "block_param_def", "opt_bv_decl", "bv_decls", "bvar",
  "lambda", "@19", "f_larglist", "lambda_body", "do_block", "block_call",
  "method_call", "opt_brace_block", "brace_block", "case_body", "cases",
  "opt_rescue", "rescue_arg", "exc_list", "exc_var", "opt_ensure",
  "literal", "strings", "string", "string_contents", "string_content",
  "@20", "@21", "@22", "string_dvar", "symbol", "sym", "numeric",
  "simple_numeric", "variable", "other_vars", "backref", "superclass",
  "$@23", "f_arglist", "args_tail", "opt_args_tail", "f_args", "f_bad_arg",
  "f_norm_arg", "f_arg_item", "f_arg", "f_kw", "f_block_kw",
  "f_block_kwarg", "f_kwarg", "kwrest_mark", "f_kwrest", "f_opt", "@24",
  "f_block_opt", "f_block_optarg", "f_optarg", "restarg_mark",
  "f_rest_arg", "blkarg_mark", "f_block_arg", "opt_f_block_arg",
  "singleton", "$@25", "const", "base", "assoc_list", "assocs", "assoc",
  "operation", "operation2", "operation3", "label", "super",
  "dot_or_colon", "opt_terms", "opt_eol", "rparen", "rbracket", "trailer",
  "term", "terms", "none", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,    61,   347,   348,    63,    58,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,    62,    60,   359,
     360,   124,    94,    38,   361,   362,    43,    45,    42,    47,
      37,   363,   364,    33,   126,   365,   123,   125,    91,    46,
      44,    96,    40,    41,    93,    59,    10
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   137,   138,   138,   139,   139,   139,   141,   142,   140,
     143,   144,   144,   144,   144,   146,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   147,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   148,   148,   148,   148,   148,   148,   149,   149,
     150,   150,   150,   151,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   153,   153,   154,   154,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   156,
     156,   157,   157,   158,   158,   159,   159,   159,   159,   159,
     159,   159,   159,   160,   160,   160,   160,   160,   160,   160,
     161,   161,   162,   162,   162,   163,   163,   163,   163,   164,
     164,   165,   166,   165,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   170,
     170,   170,   170,   171,   172,   172,   173,   173,   174,   174,
     174,   174,   174,   176,   175,   177,   178,   178,   178,   179,
     179,   179,   179,   180,   180,   180,   181,   181,   181,   181,
     181,   182,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   183,   184,   181,   185,   186,   181,   181,   181,   187,
     188,   181,   189,   181,   181,   190,   181,   191,   181,   192,
     193,   181,   181,   181,   181,   181,   194,   194,   194,   195,
     195,   196,   196,   197,   197,   198,   198,   199,   199,   200,
     200,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     202,   202,   202,   202,   203,   203,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   205,   205,   206,   206,   206,   207,   207,   208,   208,
     209,   209,   211,   210,   212,   212,   213,   213,   214,   215,
     215,   215,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   217,   217,   218,   218,   219,   220,   220,   221,   221,
     222,   223,   223,   223,   224,   224,   225,   225,   226,   226,
     227,   227,   228,   228,   229,   229,   230,   231,   232,   230,
     233,   230,   234,   234,   234,   234,   235,   236,   236,   236,
     236,   236,   237,   237,   238,   238,   238,   238,   239,   239,
     239,   239,   239,   239,   240,   240,   240,   240,   240,   240,
     240,   241,   242,   243,   242,   242,   244,   244,   245,   245,
     245,   245,   246,   246,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   248,
     248,   248,   248,   249,   249,   250,   250,   251,   251,   252,
     253,   254,   254,   255,   255,   256,   256,   257,   257,   259,
     258,   260,   261,   261,   262,   262,   263,   263,   264,   264,
     265,   265,   266,   267,   267,   268,   269,   268,   270,   271,
     272,   272,   273,   273,   274,   274,   274,   275,   275,   276,
     276,   276,   277,   277,   278,   279,   280,   280,   281,   281,
     282,   282,   283,   284,   285,   285,   286,   286,   287,   287,
     288
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     2,     0,     0,     6,
       2,     1,     1,     3,     2,     0,     4,     3,     3,     2,
       3,     3,     3,     3,     3,     0,     5,     4,     3,     3,
       3,     6,     5,     5,     5,     5,     3,     3,     3,     3,
       1,     1,     1,     3,     3,     3,     2,     1,     1,     1,
       1,     4,     4,     4,     2,     3,     4,     5,     4,     5,
       2,     2,     2,     2,     2,     1,     3,     1,     3,     1,
       2,     3,     5,     2,     4,     2,     4,     1,     3,     1,
       3,     2,     3,     1,     3,     1,     4,     3,     3,     3,
       3,     2,     1,     1,     4,     3,     3,     3,     3,     2,
       1,     1,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     0,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     5,     3,     5,     6,
       5,     5,     5,     5,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     3,     3,     3,     3,     3,     6,     1,     1,
       2,     4,     2,     3,     1,     1,     1,     1,     1,     2,
       2,     4,     1,     0,     2,     2,     2,     1,     1,     1,
       2,     3,     4,     3,     4,     2,     1,     1,     1,     1,
       3,     0,     4,     3,     3,     2,     1,     3,     3,     1,
       4,     3,     1,     5,     4,     3,     2,     2,     2,     6,
       6,     0,     0,     7,     0,     0,     7,     5,     4,     0,
       0,     9,     0,     6,     7,     0,     5,     0,     6,     0,
       0,     9,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     5,     1,     2,     1,     1,     1,     3,     1,
       3,     1,     4,     6,     3,     5,     2,     4,     1,     3,
       4,     2,     2,     1,     2,     0,     6,     8,     4,     6,
       4,     2,     6,     2,     4,     6,     2,     4,     2,     4,
       1,     1,     1,     3,     1,     4,     1,     2,     1,     3,
       1,     1,     0,     3,     4,     1,     3,     3,     4,     2,
       4,     4,     2,     4,     4,     3,     3,     3,     2,     1,
       4,     1,     1,     4,     4,     5,     1,     1,     5,     1,
       2,     1,     1,     1,     1,     2,     1,     2,     1,     1,
       1,     2,     1,     3,     0,     2,     1,     0,     0,     5,
       0,     3,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     4,     2,     3,     2,     4,     2,
       2,     1,     2,     0,     6,     8,     4,     6,     4,     6,
       2,     4,     6,     2,     4,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     3,     2,
       2,     1,     3,     1,     3,     1,     1,     2,     1,     0,
       4,     3,     1,     3,     1,     3,     1,     1,     2,     1,
       1,     1,     2,     2,     1,     1,     0,     4,     1,     1,
       1,     2,     1,     3,     3,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       0,     1,     2,     2,     1,     1,     1,     1,     1,     2,
       0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     0,   528,     0,     0,     0,     7,     0,     0,   528,
     281,   284,     0,   302,   303,   304,   305,   269,   272,   530,
       0,   530,    25,     0,   436,   437,   434,   440,   438,   439,
     435,   525,   429,   509,   508,   402,   427,   430,   431,   424,
     425,   441,    41,     0,   426,     0,   362,   540,   540,     0,
       0,    77,   266,     0,   404,     0,     0,     0,     0,   536,
     537,     0,     2,     5,    40,    42,    49,    48,     0,    65,
       0,    69,    79,     0,    47,   228,    50,   540,   256,   257,
     400,   399,   398,   422,   258,   433,   259,   432,   428,   243,
     379,     3,     4,     6,   302,   303,   269,   272,     0,   530,
     525,   100,     0,     0,     0,   103,     0,     0,   258,   259,
     101,     0,     0,   538,   529,   295,   154,   165,   155,   178,
     151,   171,   161,   160,   181,   182,   176,   159,   158,   153,
     179,   183,   184,   163,   152,   166,   170,   172,   164,   157,
     173,   180,   150,   169,   168,   149,   156,   147,   148,   177,
     162,   167,   146,   141,   145,   144,   174,   175,   136,   139,
     132,   121,   120,   126,   119,   118,   117,   122,   124,   125,
     123,   114,   115,   116,   128,   127,   129,   130,   131,   133,
     134,   140,   135,   142,   143,   138,   137,   506,   297,   107,
     108,   505,     0,   106,   105,   177,   162,   167,   146,   145,
     144,   174,   109,   111,    19,   110,   106,   105,     0,     8,
       0,     0,     0,   228,   258,   259,     0,   528,     0,     0,
       0,     0,   316,   315,     0,     0,   258,    92,     0,     0,
       0,   524,     0,   238,   249,    63,   242,   540,   540,   512,
       0,    64,    62,   530,    61,   530,   531,     0,     0,    15,
       0,     0,     0,    60,   419,   420,   421,   417,   418,   416,
     265,   278,   540,     0,   228,     0,   530,   510,     0,   530,
     530,   229,     0,     0,     0,   528,    12,   530,    67,    11,
     261,     0,     0,    75,     0,    85,   423,     0,   206,    46,
     220,   221,   205,     1,     0,     0,     0,     0,     0,     0,
       0,   540,   369,     0,    81,     0,    73,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   540,     0,     0,     0,
     540,   540,   277,   382,   381,   401,     0,     0,   540,   372,
      54,     0,   276,   378,     0,   100,   102,   101,   423,     0,
     443,   292,   442,     0,   540,     0,     0,   539,     7,     0,
     540,   527,   526,   299,   112,   260,     0,   265,   307,     0,
     306,     0,     0,   540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   540,     0,   289,   516,   245,
     250,     0,   247,   239,   248,   247,   240,   515,   530,     0,
     271,   530,   275,    45,    17,    18,     0,     0,   226,     0,
       0,     0,   486,   471,   469,   470,   472,   501,     0,   497,
     500,   496,   485,   540,     0,   467,   365,   473,   475,   477,
     453,   483,   540,   488,   540,   494,   453,   499,   453,     0,
     451,   474,     0,   468,     0,     0,   268,   535,   534,   511,
     267,   535,   230,   232,    14,   530,   263,    10,   529,    66,
     530,    91,    83,    78,     0,     0,   540,     0,     0,   406,
     407,   403,   410,   405,    23,    22,    21,    20,    24,    43,
      44,   354,   540,     0,   352,   351,     0,    29,    38,     0,
      39,   530,     0,    71,    82,    28,   185,    37,   530,   197,
     196,   225,   224,   219,   218,   217,   216,   215,   210,   211,
     213,   214,   212,   207,   208,   209,   223,   222,   198,   199,
     200,   201,   202,   203,   523,   377,   520,   519,   243,   375,
     530,   237,   236,   521,   376,   520,   519,   540,   520,   519,
     243,   243,     0,     0,    30,   187,    36,   195,   530,   540,
      55,   244,   445,     0,     7,   104,   101,   522,     0,   530,
     540,     0,     0,   530,   540,     7,     0,     0,     0,   540,
     540,   308,   185,   264,   522,   530,   520,   519,   540,     0,
     249,     0,   288,   310,   282,   309,   285,   264,   522,   530,
     520,   519,     0,   514,     0,   251,   246,   540,   513,   270,
     532,   274,    16,   530,     0,    27,   194,     0,   328,   319,
     321,   530,   317,   474,   540,     0,     0,   363,     0,   460,
       0,   449,   504,   487,     0,   450,     0,   463,   498,     0,
     465,   502,   489,   479,   530,    68,    13,   262,     0,    76,
      90,    88,   530,   520,   519,   204,   408,     0,     0,   350,
     540,     0,   335,   481,   540,   540,   492,   335,   335,   333,
     474,     0,   356,     0,   255,     0,    80,    74,     0,     0,
       0,     0,     0,   374,    58,     0,   380,     0,     0,   235,
     373,    56,   234,   371,    52,   370,    51,     0,     0,     0,
     233,     0,     0,     0,   380,     7,   296,   507,   530,     0,
     447,   300,   113,   540,   540,   389,     0,     0,     0,   311,
     313,     0,     0,   380,     0,     0,     0,   287,     0,     0,
       0,     0,   380,     0,   252,   241,   273,    26,   530,     0,
     326,     0,   476,   530,     0,     0,   452,   478,   453,   453,
     484,   540,   503,   453,   495,   453,   453,     0,   231,    84,
      86,     0,   413,   414,   415,   411,   412,   357,   358,   361,
     360,     0,   353,   341,   343,     0,   331,   332,     0,   346,
       0,   348,     0,     0,   480,   368,     0,   253,    72,   186,
       0,    34,   193,    35,   192,    59,   533,     0,    33,   191,
      32,   190,    57,   384,   383,   188,     0,   444,   293,     0,
     446,   298,   540,   391,   392,     0,   540,   393,   540,     0,
     314,   279,     0,   280,   251,   540,     0,     0,   290,   318,
     329,     0,   324,   320,   364,   367,   366,     0,   456,     0,
     458,   448,     0,   464,     0,   461,   466,   490,     0,     0,
     355,   334,   335,   335,   482,   540,   335,   493,   335,   335,
     491,   265,     0,   254,   227,    31,   189,    53,   294,     7,
       0,     0,   390,   394,     0,     9,   396,     0,   386,   387,
     385,   283,   286,     0,     0,   327,     0,   322,   453,   453,
     453,   453,   409,   359,     0,   338,     0,   340,   330,     0,
     347,     0,   344,   349,   264,     0,   540,     0,   395,     0,
      93,   397,   540,     0,   325,     0,   457,     0,   454,   459,
     462,   335,   335,   335,   335,   301,   388,    99,     0,   540,
       0,   312,   291,   323,   453,   339,     0,   336,   342,   345,
      98,   522,   530,   520,   519,   455,   335,   380,   337
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    61,    62,   208,   209,   376,   274,   275,   276,   416,
     251,    64,    65,    66,   560,    67,    68,   277,    69,    70,
      71,   473,    72,   212,   105,   106,   202,   203,   204,   578,
     543,   190,    74,   268,   535,   690,   540,   541,   350,   351,
     236,   403,   237,   500,   264,   470,   219,   730,   220,   731,
     602,   883,   564,   368,   370,   577,   812,   379,   594,   718,
     719,   225,   619,   620,   621,   851,   774,   660,   493,   494,
     661,   767,   768,   261,   262,   434,   627,   302,    76,    77,
     342,   352,   390,   880,   714,   815,   816,   872,   875,    78,
      79,    80,   287,   483,   656,   761,   657,   765,    81,   259,
      82,    83,   214,    85,   215,   361,   563,   575,   746,   629,
     576,   437,   438,   439,   440,   441,   663,   664,   442,   443,
     444,   445,   757,   666,   667,   446,   447,   448,   449,   450,
     631,   192,   369,    87,    88,   265,   238,   239,   111,   568,
     539,   240,    90,   373,   112,   409,   410,   686,   459,   380,
     114,   279
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -702
static const yytype_int16 yypact[] =
{
    2629,  6249,  4266,  8256,  4503,  5837,  -702,  7629,  7629,  2388,
    -702,  -702,  8100,  6364,  6364,  -702,  -702,  6364,  5104,   145,
    5708,    39,  -702,    63,  -702,  -702,  -702,  -702,  -702,  -702,
    -702,  6364,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,
    -702,  -702,  -702,  5579,  -702,   170,  -702,  6939,  6594,  3495,
    7629,  6089,  -702,   367,  -702,  7744,  7859,  7744,  7744,  -702,
    -702,   231,  -702,   695,   104,  -702,  -702,   252,   195,  -702,
     159,  8178,  -702,   206,  3880,    28,    29,    55,  -702,   219,
    -702,  -702,  -702,  -702,    53,  -702,    38,  4626,  4750,    59,
     173,  -702,  -702,   695,  -702,  -702,  -702,   185,   200,    39,
    -702,    48,   431,  3631,   367,  -702,    82,    32,  -702,  -702,
     370,    59,   176,  -702,   212,  -702,  -702,  -702,  -702,  -702,
    -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,
    -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,
    -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,    34,
      99,   113,   222,  -702,   226,   234,   243,  -702,  -702,  -702,
    -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,
    -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,
    -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,  -702,
    -702,  -702,   264,   301,   309,  -702,  -702,  -702,  -702,  -702,
    -702,  -702,  -702,  -702,   240,  -702,  -702,  -702,   364,  -702,
     170,    65,   284,    43,   427,   286,    65,   101,   374,  7629,
    7629,   170,  -702,  -702,   140,   359,    91,   199,  7744,  7744,
    7744,  -702,  7744,  -702,  1486,  -702,  -702,   257,   268,  -702,
    7744,  -702,  -702,  4020,  -702,  4143,  -702,  7629,    64,  -702,
    5449,   277,  3767,  -702,  -702,  -702,  -702,  -702,   219,  -702,
     180,  -702,   874,  1486,   209,   278,     4,  -702,   280,     8,
       4,  -702,  6249,  3495,   299,   393,   695,    39,   352,  -702,
     104,   170,  8334,   329,   319,   338,   327,   479,   339,  -702,
    -702,  -702,  -702,  -702,  7629,  7629,  7629,  7629,  6249,  7629,
    7629,    11,  -702,  7054,  -702,  3495,  6169,   342,  7054,  7744,
    7744,  7744,  7744,  7744,  7744,  7744,  7744,  7744,  7744,  7744,
    7744,  7744,  7744,  7744,  7744,  7744,  7744,  7744,  7744,  7744,
    7744,  7744,  7744,  7744,  7744,  2477,  6364,  2477,  7977,  7977,
      11,    11,  -702,  -702,  -702,  -702,  7859,  7859,  6364,  -702,
     381,  6364,  -702,  -702,   361,  -702,  -702,   341,  -702,   393,
    -702,  -702,  -702,  7892,  6364,  2477,  7629,  -702,  -702,  7629,
    1131,  -702,  -702,  -702,  -702,  -702,  2852,   442,  -702,  2979,
     478,  7744,  2477,  6364,  2477,  7744,  7744,  3233,   374,  7169,
     485,    98,    98,   116,  2477,  6364,  2477,  -702,  3880,  3880,
    3880,  7744,  6479,  -702,  -702,  6709,  -702,  3880,    39,   375,
    -702,    73,  -702,  -702,  -702,  -702,  5837,  7629,  3880,  3767,
     373,  7744,  -702,  -702,  -702,  -702,  -702,  -702,   769,  -702,
    -702,  -702,  -702,  1183,    70,  -702,  -702,  -702,  -702,  -702,
     401,  -702,   406,   449,   410,  -702,   412,   449,   415,   449,
    -702,   419,  7744,  -702,  2477,  2477,  -702,  6939,  -702,  -702,
    -702,  6824,  -702,  -702,   695,    39,  -702,  -702,  4370,   435,
      39,   341,  -702,   441,  8334,  2477,  6364,  2477,  7744,  -702,
    -702,  -702,  -702,  -702,   104,   104,   104,   104,  -702,  -702,
    -702,  -702,   653,  3894,  -702,  -702,  7744,  -702,  3092,   451,
    -702,    39,  8334,   465,  -702,  -702,  2966,  -702,  2838,  4004,
    4004,  4124,  4246,  4350,  4350,  4350,  4350,  4350,  4350,   946,
     946,   946,   946,  1025,  1025,   981,   574,   574,   614,   614,
     339,   339,   339,   339,  5219,  -702,  1970,  2716,   173,  -702,
      39,  -702,  -702,  -702,  -702,   230,   323,  5334,  -702,  -702,
    2226,  2226,  3894,  3767,  -702,  3618,  -702,  3880,    39,    11,
    -702,  -702,  -702,  7629,  -702,  -702,    20,   391,   173,    39,
     173,   101,   539,    73,  1183,  -702,   393,  5966,  5837,   596,
     569,  -702,  3754,  4874,  4989,    39,   493,   516,   608,   601,
    3880,    52,  -702,  -702,  -702,  -702,  -702,    54,    83,    39,
     122,   129,  7629,  3880,  7744,  1486,  -702,   268,  -702,  -702,
    -702,  -702,  -702,    73,   499,  -702,  3880,   769,   568,  -702,
     501,    39,  -702,  -702,   512,  3894,  3767,  -702,  1183,  -702,
      40,  -702,  -702,  -702,    56,  -702,  1183,  -702,  -702,   753,
    -702,  -702,  -702,  3880,     4,   228,   695,  -702,  8334,   441,
      20,   426,    39,    74,   102,   339,  -702,   515,   669,  -702,
     512,   525,   522,  -702,   529,   410,  -702,   531,   544,  -702,
     584,  8412,  -702,   670,  3219,  7284,  -702,   441,  8334,  7744,
     586,  7859,  7859,  -702,   381,   553,   382,  7859,  7859,  -702,
    -702,   381,  -702,  -702,  -702,  -702,  -702,   689,   576,  7744,
    -702,  3767,   101,   694,  -702,  -702,  -702,  -702,    39,   698,
    -702,  -702,  -702,  7399,   608,  -702,  7629,  3369,   699,  -702,
    -702,  7744,  7744,   526,  7744,  7744,   702,  -702,  7514,  3106,
    3894,  3894,   134,    98,  3880,  -702,  -702,  -702,    39,   590,
     575,   840,  -702,    39,   704,   592,  -702,  -702,   594,   598,
    -702,   410,  -702,   609,  -702,   610,   609,  7744,  -702,  -702,
     328,  3767,  -702,  -702,  -702,  -702,  -702,   613,  -702,  -702,
    -702,   620,  -702,  1183,  -702,    40,  -702,  -702,  1183,  -702,
     753,  -702,  8412,   170,   354,  -702,  7744,  3355,   441,  3880,
    7744,  -702,  3880,  -702,  3880,  -702,  -702,  7859,  -702,  3880,
    -702,  3880,  -702,  -702,  -702,  3880,   593,  -702,  -702,   728,
    -702,  -702,  1131,  3092,  -702,    67,   679,  -702,   736,    65,
    -702,  -702,  7744,  -702,  3880,   109,   737,   742,  -702,  -702,
     616,   590,   580,  -702,  -702,  -702,  -702,  1183,  -702,   753,
    -702,  -702,   753,  -702,   753,  -702,  -702,  3880,   629,   669,
    -702,  -702,   630,   631,  -702,   410,   633,  -702,   634,   633,
     354,  -702,  2477,  3481,  3880,  -702,  3880,  -702,  -702,  -702,
    2852,  8490,  -702,  -702,  3894,  -702,  -702,  2979,  -702,  -702,
    -702,  -702,  -702,  3894,   590,   616,   590,   637,   609,   643,
     609,   609,  -702,  -702,  1183,  -702,   753,  -702,  -702,   753,
    -702,   753,  -702,  -702,   647,   770,   596,   170,  -702,   376,
     338,  -702,   569,   771,   616,   590,  -702,   753,  -702,  -702,
    -702,   633,   663,   633,   633,  -702,  -702,   341,  2477,  6364,
    2477,  -702,  -702,   616,   609,  -702,   753,  -702,  -702,  -702,
      20,    60,    39,    88,    90,  -702,   633,    94,  -702
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -702,  -702,  -702,  -359,  -702,  -702,   445,  -702,    41,  -702,
    -702,   190,   -32,  -702,  -406,     5,   782,  -182,   -19,   -68,
    -702,  -415,    10,  1745,   -89,   795,     0,    -3,  -702,  -702,
       1,  -702,  1487,  -702,   -44,   118,  -333,    75,   128,  -702,
     -76,  -218,   -41,  -282,   294,  -702,  -702,  -702,  -702,  -702,
    -702,  -702,  -702,  -702,  -702,  -702,  -702,  -214,  -352,  -113,
    -541,  -702,  -673,  -664,   186,   312,   110,  -702,  -269,  -702,
    -509,  -702,   -39,  -702,  -702,  -702,  -702,  -702,  -702,  -702,
    -702,   734,  -363,  -702,  -101,  -702,  -702,  -702,  -702,  -702,
     776,   -13,  -702,  -702,  -702,  -702,  -702,  -702,     9,  -702,
    -702,    66,  1274,  -702,  1844,  -702,  -702,    14,  -206,  -205,
    -225,  -610,  -368,  -595,  -379,   182,    45,  -702,  -702,  -702,
     179,  -584,  -702,  -701,    58,   208,  -702,  -416,  -702,  -248,
    -409,  -702,  -702,  1000,    -4,  -702,    16,  -315,   741,  -327,
    -702,   857,  -702,  -702,    44,    13,   256,  -515,  -242,    49,
     538,   856
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -541
static const yytype_int16 yytable[] =
{
     194,   207,   387,   307,   188,   189,   189,   269,   538,   572,
     547,   550,   551,   356,   205,   558,   207,   249,   233,   233,
     406,   189,   233,   233,   289,   589,   507,   462,   463,   205,
     278,   569,   247,   747,   250,   635,   233,   436,   570,   207,
     596,    63,    93,   257,   189,   349,   353,   726,   769,    91,
     585,   113,   754,   218,   704,   538,   435,   547,   113,   649,
     622,   283,   599,   266,   270,   378,   345,   349,   833,   570,
     723,   552,   553,   -96,  -509,   830,   668,   857,   378,   -98,
     378,   340,   422,   359,   732,   340,  -264,   677,   235,   241,
     608,   465,   242,   244,   335,   338,   299,   300,   363,   -89,
    -436,   -97,   427,   -95,   299,   300,   253,   -94,   -96,   382,
     491,   414,   354,   662,  -509,   743,   -93,   231,   427,   286,
     625,   415,   492,   501,   717,   593,   389,   -87,   570,   299,
     300,   347,   299,   300,   457,   299,   300,   760,   461,   626,
     246,   -99,   608,   652,   246,   -93,   346,   -97,  -264,  -264,
     570,   771,  -520,   430,   -95,   362,   336,   337,   339,   -94,
     364,   365,   432,  -436,   435,  -437,   -89,   885,   -92,   430,
     358,   383,   384,   818,  -509,   246,  -509,  -509,   747,  -434,
    -509,   341,   728,   -85,   -90,   341,  -520,    59,    60,   252,
     360,   348,  -519,   857,   -87,   -96,   -96,   211,   216,   217,
      59,    60,    59,    60,   -89,   703,   394,   -89,   624,   246,
     -89,   833,   749,   -88,   472,  -519,   709,    59,    60,    34,
     755,   -85,   914,   -97,   -97,   -95,   -95,   435,  -437,   -94,
     -94,   293,   -87,    59,    60,   -87,    59,    60,   -87,   769,
     280,   637,  -434,   640,   669,   345,   -91,   747,   233,   622,
     740,   933,   -89,   754,   278,   776,   777,   753,   451,   -87,
     756,   388,   499,   788,   -86,  -259,   113,   499,   395,   396,
      35,   497,   -99,   421,   565,   454,   505,   245,   795,   458,
     301,   246,   458,   458,   878,   802,   278,   303,  -440,   304,
     701,   366,  -438,   544,    75,    75,   107,   107,   308,    54,
    -439,   213,   213,   213,   747,   348,   224,   213,   213,  -435,
     -91,   213,   213,   464,   554,   556,   503,   243,   408,   467,
     -66,   544,   -97,   687,   113,   213,   606,  -259,  -259,   606,
     371,   537,   245,   546,   549,   549,   534,   383,   455,   488,
     544,   233,   841,    75,   213,   284,   809,   367,   591,   708,
     213,  -440,   544,   233,  -380,  -438,   233,   853,   -80,   567,
     -89,   549,   858,  -439,   534,   284,   451,  -432,   435,   233,
     374,   622,  -435,   622,   375,  -428,   381,   729,   584,   386,
     587,   828,   752,   534,   397,   475,   752,   402,   233,   735,
     598,   389,   601,   372,  -380,   534,  -518,    75,   405,   856,
     233,   859,   758,   419,  -258,   456,   472,  -265,   562,   391,
     392,   544,   207,   612,   460,   -95,   688,   189,   607,    36,
     862,   889,    39,    40,   623,   205,   561,   947,    44,   451,
    -432,   814,   466,   544,   472,   411,  -432,   413,  -428,   633,
     595,   595,   928,   638,   -65,   641,   898,   476,   477,   478,
     584,   587,  -522,   -87,  -380,   534,  -380,  -380,   888,   474,
     890,   334,   879,   622,   887,   891,  -258,  -258,   559,  -265,
    -265,   651,   504,   654,   -94,   797,   534,   644,   922,   355,
      34,   233,   364,   365,   484,   485,   486,   487,   670,   489,
     490,   581,  -522,   417,   683,   592,  -518,    33,  -432,  -432,
     615,   412,  -518,   689,   929,   930,   689,   689,   610,   646,
     905,   642,   -86,   213,   213,   921,   622,   923,   622,   -93,
     385,   680,   924,  -519,   683,   669,   689,   752,    59,    60,
     669,   628,   669,   469,   -99,   421,   630,   213,   934,   213,
     634,   213,   636,   838,   840,   639,    75,   622,   843,   706,
     845,   846,  -522,   685,  -522,  -522,   571,   946,  -519,   573,
     479,   480,   762,   481,   482,   -80,    75,    75,   763,   764,
     451,   648,    41,   207,   207,   712,   284,   711,   189,   189,
     759,   675,   685,   716,   717,   -97,   724,   205,   213,   213,
     213,   213,    75,   213,   213,   678,   942,   213,   685,    75,
     284,   870,   213,   570,   713,   877,   435,   613,   -95,   725,
     472,   727,   685,   623,   623,   423,    33,   424,   -94,   822,
     705,   425,   426,   717,   451,   710,   737,   423,    33,   424,
     213,   741,   451,   425,   426,   623,   772,   423,    33,   424,
     213,   213,   213,   425,   426,   213,   669,   658,   669,   791,
     793,   669,   773,   669,   770,   798,   800,   458,   213,   775,
     213,   778,   617,   213,   609,   685,   684,   611,   693,   695,
      75,   665,   499,    75,   780,   691,   782,   213,   694,   696,
     785,    75,   790,   916,   918,   919,   920,   796,   669,   213,
     329,   330,   331,   332,   333,   422,   334,   420,   739,   803,
     423,    33,   424,   804,   808,   831,   425,   426,   811,   821,
     886,   213,   823,    75,   835,   427,   423,    33,   424,   836,
     867,   645,   425,   426,   837,   428,   647,   429,   839,   945,
     231,   850,   331,   332,   333,   623,   334,   623,   868,   842,
     844,    89,    89,   849,   871,   874,   884,   881,    89,    89,
      89,   807,   882,   702,    89,    89,   892,   676,    89,    89,
     894,   896,    75,   899,   901,   865,   430,   915,   284,   670,
     213,   431,    89,   917,   670,   432,   623,   779,   781,  -520,
     925,   932,   595,   294,   295,   296,   297,    75,   658,   298,
      89,    89,   733,   936,   222,   422,   284,    89,   115,   931,
     423,    33,   424,   738,   659,   926,   425,   426,   451,   751,
     893,   343,   750,   468,   700,   427,   423,    33,   424,   258,
     854,   579,   425,   426,   580,   428,   869,   623,   623,   707,
     231,   852,   588,   451,     0,   623,   748,     0,   623,     0,
     623,   617,     0,   618,    89,   770,    75,    75,     0,     0,
       0,     0,     0,     0,     0,     0,    92,   213,   567,     0,
       0,     0,     0,   534,   614,     0,   430,     0,     0,   736,
       0,     0,     0,     0,     0,   432,     0,   742,     0,     0,
     623,     0,   623,     0,     0,     0,   544,   423,    33,   424,
     670,     0,   623,   425,   426,   623,   213,   623,     0,     0,
       0,     0,     0,   267,   271,     0,   819,     0,     0,     0,
       0,   623,   617,   623,   832,     0,   422,     0,     0,    75,
      75,   423,    33,   424,   941,     0,   944,   425,   426,   534,
       0,     0,   623,   344,   233,     0,   427,     0,   673,     0,
       0,     0,   284,     0,     0,     0,   428,     0,   429,     0,
       0,   231,   665,     0,   855,   685,     0,   665,     0,   665,
      89,    89,   895,   897,   810,   784,   900,     0,   902,   903,
       0,     0,   284,     0,     0,   213,   213,     0,     0,     0,
       0,   213,   213,     0,    89,     0,    89,   430,    89,     0,
       0,     0,   431,    89,   829,    75,   432,   697,   698,   834,
       0,     0,   110,   110,   193,   206,   433,     0,     0,     0,
     213,    75,     0,    89,    89,     0,     0,     0,     0,     0,
     206,     0,     0,    75,    75,    75,     0,     0,     0,     0,
       0,   935,   937,   938,   939,    89,    89,    89,    89,    89,
      89,    89,     0,   206,    89,   260,    89,     0,     0,    89,
       0,     0,     0,     0,     0,    75,   948,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,     0,   334,     0,
     744,   745,     0,   665,     0,   665,   860,    89,   665,     0,
     665,     0,     0,     0,     0,     0,     0,    89,    89,    89,
       0,   213,    89,   404,   404,   327,   328,   329,   330,   331,
     332,   333,   357,   334,     0,    89,     0,    89,     0,     0,
      89,     0,     0,     0,     0,   665,     0,    89,   453,   452,
      89,     0,     0,     0,    89,     0,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    89,     0,   326,   327,
     328,   329,   330,   331,   332,   333,   806,   334,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   495,    89,     0,
      89,     0,   820,     0,    75,   909,     0,     0,    75,     0,
       0,    75,     0,   422,   825,   826,   827,    75,   423,    33,
     424,     0,     0,     0,   425,   426,     0,     0,     0,     0,
       0,     0,   542,   427,     0,     0,   495,   495,     0,     0,
       0,     0,     0,   428,   542,   429,   848,     0,   231,    89,
     377,     0,     0,     0,     0,     0,     0,    89,     0,     0,
     542,   393,     0,   213,     0,   422,   453,   452,     0,     0,
     423,    33,   424,     0,    89,     0,   425,   426,     0,   542,
       0,     0,     0,     0,   430,   427,     0,     0,     0,   431,
       0,   542,     0,   432,     0,   428,     0,   429,     0,     0,
     231,     0,     0,   574,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,    84,   108,   108,   191,     0,
       0,   471,     0,     0,     0,     0,   226,     0,     0,   453,
     452,     0,     0,    89,    89,     0,   430,     0,   632,     0,
     632,   431,     0,     0,    89,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   906,     0,     0,     0,   911,
       0,     0,   912,    84,     0,   285,     0,     0,   913,     0,
       0,     0,   542,     0,     0,   536,     0,   545,   548,   548,
       0,     0,     0,    89,     0,   285,     0,     0,   672,   671,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   566,     0,   548,    89,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,   583,     0,   586,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   597,     0,   600,     0,     0,     0,
       0,     0,     0,   692,     0,     0,   692,   692,     0,     0,
       0,     0,     0,     0,     0,   495,   206,     0,     0,     0,
       0,     0,    89,    89,     0,     0,   692,     0,    89,    89,
     453,   452,     0,     0,     0,   715,   720,     0,     0,     0,
       0,     0,    89,     0,   720,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   583,   586,     0,    89,    89,     0,
       0,     0,     0,   404,     0,     0,     0,     0,     0,     0,
      89,    89,    89,     0,     0,   650,     0,   653,     0,     0,
     672,     0,     0,     0,     0,   452,     0,   452,     0,     0,
       0,     0,     0,   452,     0,     0,   452,     0,     0,     0,
     234,   234,    89,     0,   234,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   672,     0,   234,     0,
     632,   632,     0,     0,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,   263,   234,     0,     0,    89,     0,
       0,     0,   288,   290,   291,   292,    84,    84,     0,     0,
       0,   401,     0,     0,     0,     0,   285,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   817,
     720,     0,    84,     0,     0,     0,     0,   206,   206,    84,
     285,   309,     0,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   632,   334,     0,
       0,    89,     0,     0,     0,    89,     0,     0,    89,     0,
       0,     0,     0,     0,    89,     0,     0,     0,     0,     0,
     671,     0,   671,     0,     0,   671,     0,   671,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    84,     0,     0,     0,     0,     0,     0,   453,   452,
      89,     0,   873,     0,   876,     0,     0,     0,     0,     0,
       0,   720,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,   452,     0,   452,     0,     0,   452,
       0,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   632,     0,     0,     0,   398,   399,   400,     0,   290,
       0,     0,     0,     0,     0,     0,     0,   407,     0,     0,
     234,     0,     0,     0,     0,     0,     0,   418,     0,     0,
       0,     0,    84,     0,     0,    73,    73,     0,   285,     0,
       0,   671,     0,   671,     0,     0,   671,   223,   671,     0,
       0,     0,   715,     0,     0,     0,     0,    84,   720,     0,
       0,     0,     0,     0,   452,     0,   285,     0,     0,     0,
       0,     0,     0,   861,     0,   542,     0,     0,     0,     0,
     498,     0,     0,   671,    73,   506,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,   234,     0,     0,    84,    84,     0,     0,
       0,     0,     0,   555,   557,   234,     0,     0,   234,     0,
       0,     0,     0,     0,    86,    86,   109,   109,    73,     0,
       0,   234,     0,     0,     0,     0,   227,     0,     0,     0,
       0,     0,   904,     0,     0,     0,     0,     0,   582,     0,
     234,     0,   555,   557,     0,     0,   590,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,   603,   605,
       0,     0,   263,    86,     0,   227,     0,     0,     0,    84,
      84,     0,     0,     0,     0,     0,     0,   927,   616,     0,
       0,     0,     0,     0,     0,   227,     0,     0,     0,     0,
       0,     0,   285,     0,     0,     0,     0,     0,   940,     0,
     943,     0,     0,     0,     0,     0,     0,     0,     0,   643,
       0,     0,     0,     0,   263,   108,     0,    86,   605,     0,
       0,     0,   285,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,     0,   655,     0,     0,     0,     0,
    -264,     0,     0,     0,     0,    84,     0,     0,  -264,  -264,
    -264,     0,     0,   674,  -264,  -264,     0,  -264,     0,     0,
       0,    84,     0,     0,     0,     0,     0,    73,     0,     0,
       0,  -264,  -264,    84,    84,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    73,    73,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,  -264,     0,     0,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
      73,     0,     0,     0,     0,     0,   108,     0,  -264,  -264,
    -264,  -264,   -98,   681,  -264,  -264,     0,  -264,  -264,  -264,
    -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,
    -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,
    -264,   734,  -264,     0,     0,     0,    86,  -264,  -264,  -264,
     -90,     0,     0,  -264,     0,  -264,  -264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    86,     0,     0,
       0,    73,     0,     0,    73,     0,   227,     0,     0,     0,
       0,     0,    73,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86,     0,    84,   910,     0,     0,    84,    86,
     227,    84,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,   787,     0,    73,     0,   789,     0,   792,   794,
       0,     0,     0,     0,   799,   801,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   805,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     813,     0,     0,     0,     0,     0,     0,     0,   792,   794,
       0,   799,   801,    73,     0,   824,     0,     0,     0,     0,
      86,     0,     0,    86,     0,     0,  -540,     0,     0,     0,
       0,    86,     0,     0,  -540,  -540,  -540,     0,    73,  -540,
    -540,  -540,     0,  -540,   847,     0,     0,     0,     0,     0,
       0,     0,     0,  -540,     0,     0,     0,  -540,  -540,     0,
       0,     0,     0,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   863,     0,     0,     0,   864,     0,     0,
       0,     0,     0,     0,   866,     0,     0,     0,     0,     0,
       0,     0,  -540,     0,     0,     0,     0,    73,    73,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   866,
       0,     0,    86,     0,  -540,  -540,  -540,  -540,   227,     0,
    -540,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,     0,
       0,     0,     0,     0,     0,     0,   227,     0,     0,     0,
       0,     0,     0,  -540,     0,  -540,     0,     0,   348,  -540,
       0,  -540,  -540,     0,     0,     0,     0,     0,     0,     0,
      73,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     2,     3,     4,     0,     6,    86,    86,     0,     7,
       8,     0,     0,     0,     9,     0,    10,    11,    12,    13,
      14,    15,    16,     0,     0,     0,   234,    17,    18,     0,
       0,    19,     0,    21,     0,     0,    24,    25,    26,    27,
       0,    28,    29,    30,    31,    32,    33,    34,     0,    35,
      36,    37,    38,    39,    40,    41,    73,     0,    43,    44,
       0,     0,     0,     0,     0,   210,    46,     0,    47,    48,
     103,    50,    73,     0,    52,     0,     0,    53,    54,    86,
      86,     0,     0,     0,    73,    73,    73,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   227,     0,     0,     0,     0,     0,     0,     0,
       0,   766,     0,     0,     0,     0,    73,     0,     0,    55,
       0,    56,    57,    58,     0,   109,     0,     0,     0,   153,
       0,     0,   227,    59,    60,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,   159,     0,     0,     0,    86,     0,     0,     0,     0,
       0,   160,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    86,    86,     0,     0,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,     0,     0,    86,     0,     0,   186,   348,
       0,     0,     0,     0,     0,    73,   908,     0,     0,    73,
       0,     0,    73,     0,     0,     0,   109,     0,    73,  -540,
       1,     0,     2,     3,     4,     5,     6,     0,     0,     0,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      13,    14,    15,    16,     0,     0,     0,     0,    17,    18,
       0,     0,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,    30,    31,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,    42,    43,
      44,     0,     0,     0,     0,     0,    45,    46,     0,    47,
      48,    49,    50,    51,     0,    52,     0,     0,    53,    54,
       0,     0,     0,     0,    86,   109,  -522,     0,    86,     0,
       0,    86,     0,     0,  -522,  -522,  -522,    86,     0,     0,
    -522,  -522,     0,  -522,     0,     0,     0,     0,     0,     0,
       0,     0,  -522,     0,     0,     0,     0,  -522,  -522,     0,
      55,     0,    56,    57,    58,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -522,  -522,  -522,  -522,   -96,   682,
    -522,  -522,     0,  -522,  -522,  -522,  -522,  -522,  -522,  -522,
    -522,  -522,  -522,  -522,  -522,  -522,  -522,  -522,  -522,  -522,
    -522,  -522,  -522,  -522,  -522,  -522,  -522,     0,  -522,     0,
       0,     0,  -522,  -522,  -522,  -522,   -88,     0,     0,  -522,
       0,  -522,  -522,   272,     0,     2,     3,     4,     5,     6,
    -540,  -540,  -540,     7,     8,     0,     0,  -540,     9,     0,
      10,    11,    12,    13,    14,    15,    16,     0,     0,     0,
       0,    17,    18,     0,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,    30,    31,    32,
      33,    34,     0,    35,    36,    37,    38,    39,    40,    41,
       0,    42,    43,    44,     0,     0,     0,     0,     0,    45,
      46,     0,    47,    48,    49,    50,    51,     0,    52,     0,
       0,    53,    54,   309,     0,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,   246,    56,    57,    58,     0,     0,
     272,     0,     2,     3,     4,     5,     6,  -540,  -540,  -540,
       7,     8,     0,  -540,  -540,     9,     0,    10,    11,    12,
      13,    14,    15,    16,     0,     0,     0,     0,    17,    18,
       0,     0,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,    30,    31,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,    42,    43,
      44,     0,     0,     0,     0,     0,    45,    46,     0,    47,
      48,    49,    50,    51,     0,    52,     0,     0,    53,    54,
     679,   309,     0,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,     0,   334,     0,
       0,     0,     0,     0,     0,     0,  -249,     0,     0,     0,
      55,     0,    56,    57,    58,     0,     0,   272,     0,     2,
       3,     4,     5,     6,  -540,  -540,  -540,     7,     8,     0,
       0,  -540,     9,  -540,    10,    11,    12,    13,    14,    15,
      16,     0,     0,     0,     0,    17,    18,     0,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,    30,    31,    32,    33,    34,     0,    35,    36,    37,
      38,    39,    40,    41,     0,    42,    43,    44,     0,     0,
       0,     0,     0,    45,    46,     0,    47,    48,    49,    50,
      51,     0,    52,     0,     0,    53,    54,   309,     0,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,     0,   334,     0,     0,     0,     0,     0,
       0,     0,  -249,     0,     0,     0,     0,    55,     0,    56,
      57,    58,     0,     0,   272,     0,     2,     3,     4,     5,
       6,  -540,  -540,  -540,     7,     8,     0,     0,  -540,     9,
       0,    10,    11,    12,    13,    14,    15,    16,     0,     0,
       0,     0,    17,    18,     0,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,    30,    31,
      32,    33,    34,     0,    35,    36,    37,    38,    39,    40,
      41,     0,    42,    43,    44,     0,     0,     0,     0,     0,
      45,    46,     0,    47,    48,    49,    50,    51,     0,    52,
       0,     0,    53,    54,   309,     0,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
       0,   334,     0,     0,     0,     0,     0,     0,     0,  -250,
       0,     0,     0,     0,    55,     0,    56,    57,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -540,  -540,
     272,     0,     2,     3,     4,     5,     6,     0,  -540,  -540,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      13,    14,    15,    16,     0,     0,     0,     0,    17,    18,
       0,     0,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,    30,    31,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,    42,    43,
      44,     0,     0,     0,     0,     0,    45,    46,     0,    47,
      48,    49,    50,    51,     0,    52,     0,     0,    53,    54,
     309,     0,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,     0,   334,     0,     0,
       0,     0,     0,     0,     0,  -251,     0,     0,     0,     0,
      55,     0,    56,    57,    58,     0,   272,     0,     2,     3,
       4,     5,     6,     0,  -540,  -540,     7,     8,     0,     0,
       0,     9,     0,    10,    11,    12,    13,    14,    15,    16,
       0,     0,     0,     0,    17,    18,     0,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
      30,    31,    32,    33,    34,     0,    35,    36,    37,    38,
      39,    40,    41,     0,    42,    43,    44,     0,     0,     0,
       0,     0,    45,    46,     0,    47,    48,   273,    50,    51,
       0,    52,     0,     0,    53,    54,   309,     0,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,     0,   334,     0,     0,     0,     0,     0,     0,
       0,  -252,     0,     0,     0,     0,    55,     0,    56,    57,
      58,     0,     0,     0,     0,     0,     0,     0,  -540,     0,
    -540,  -540,   272,     0,     2,     3,     4,     5,     6,     0,
       0,     0,     7,     8,     0,     0,     0,     9,     0,    10,
      11,    12,    13,    14,    15,    16,     0,     0,     0,     0,
      17,    18,     0,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,    30,    31,    32,    33,
      34,     0,    35,    36,    37,    38,    39,    40,    41,     0,
      42,    43,    44,     0,     0,     0,     0,     0,    45,    46,
       0,    47,    48,    49,    50,    51,     0,    52,     0,     0,
      53,    54,   699,   309,     0,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,     0,
     334,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,    56,    57,    58,     0,     0,     0,
       0,     0,     0,     0,  -540,     0,  -540,  -540,   272,     0,
       2,     3,     4,     5,     6,     0,     0,     0,     7,     8,
       0,     0,     0,     9,     0,    10,    11,    12,    13,    14,
      15,    16,     0,     0,     0,     0,    17,    18,     0,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,    30,    31,    32,    33,    34,     0,    35,    36,
      37,    38,    39,    40,    41,     0,    42,    43,    44,     0,
       0,     0,     0,     0,    45,    46,     0,    47,    48,    49,
      50,    51,     0,    52,     0,     0,    53,    54,   679,   309,
       0,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
      56,    57,    58,     0,  -540,   272,     0,     2,     3,     4,
       5,     6,  -540,  -540,  -540,     7,     8,     0,     0,     0,
       9,     0,    10,    11,    12,    13,    14,    15,    16,     0,
       0,     0,     0,    17,    18,     0,     0,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,    30,
      31,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,    42,    43,    44,     0,     0,     0,     0,
       0,    45,    46,     0,    47,    48,    49,    50,    51,     0,
      52,     0,     0,    53,    54,   309,     0,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,    56,    57,    58,
       0,     0,     0,     2,     3,     4,     0,     6,     0,  -540,
    -540,     7,     8,     0,     0,     0,     9,     0,    10,    11,
      12,    13,    14,    15,    16,     0,     0,     0,     0,    17,
      18,     0,     0,    98,     0,    21,     0,     0,    24,    25,
      26,    27,   228,    28,    29,    30,    31,    32,    33,    34,
       0,    35,    36,    37,    38,    39,    40,    41,     0,     0,
      43,    44,   229,     0,     0,     0,     0,   210,    46,     0,
      47,    48,   103,    50,   230,     0,    52,   231,     0,    53,
      54,  -541,  -541,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,   232,    57,    58,     2,     3,     4,     0,
       6,     0,     0,     0,     7,     8,   246,     0,     0,     9,
       0,    10,    11,    12,    13,    14,    15,    16,     0,     0,
       0,     0,    17,    18,     0,     0,    19,     0,    21,     0,
       0,    24,    25,    26,    27,     0,    28,    29,    30,    31,
      32,    33,    34,     0,    35,    36,    37,    38,    39,    40,
      41,     0,     0,    43,    44,     0,     0,     0,     0,     0,
     210,    46,     0,    47,    48,   103,    50,     0,     0,    52,
       0,     0,    53,    54,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,     0,   334,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,    56,    57,    58,     2,
       3,     4,     0,     6,     0,     0,     0,     7,     8,   246,
       0,     0,     9,     0,    10,    11,    12,    94,    95,    15,
      16,     0,     0,     0,     0,    96,    97,     0,     0,    98,
       0,    99,     0,     0,    24,    25,    26,    27,     0,    28,
      29,    30,   100,    32,   101,    34,     0,    35,    36,    37,
      38,    39,    40,    41,     0,     0,    43,    44,     0,     0,
       0,     0,     0,   102,    46,     0,    47,    48,   103,    50,
       0,     0,    52,     0,     0,   104,    54,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,     0,   334,     0,
       0,     0,     0,     2,     3,     4,     5,     6,     0,     0,
       0,     7,     8,     0,     0,     0,     9,     0,    10,    11,
      12,    13,    14,    15,    16,     0,     0,     0,     0,    17,
      18,    59,    60,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,    30,    31,    32,    33,    34,
       0,    35,    36,    37,    38,    39,    40,    41,     0,    42,
      43,    44,     0,     0,     0,     0,     0,    45,    46,     0,
      47,    48,    49,    50,    51,     0,    52,     0,     0,    53,
      54,  -541,  -541,  -541,  -541,  -541,  -541,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,     0,   334,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,     0,    56,    57,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   367,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
       0,     0,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      32,    33,    34,     0,     0,     0,    37,    38,     0,     0,
       0,     0,     0,     0,     0,     0,   158,   159,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   160,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,  -518,
    -518,  -518,     0,  -518,   186,   187,     0,  -518,  -518,     0,
       0,     0,  -518,     0,  -518,  -518,  -518,  -518,  -518,  -518,
    -518,     0,  -518,     0,     0,  -518,  -518,     0,     0,  -518,
       0,  -518,     0,     0,  -518,  -518,  -518,  -518,  -518,  -518,
    -518,  -518,  -518,  -518,  -518,  -518,     0,  -518,  -518,  -518,
    -518,  -518,  -518,  -518,     0,     0,  -518,  -518,  -518,     0,
       0,     0,     0,  -518,  -518,     0,  -518,  -518,  -518,  -518,
    -518,     0,  -518,  -518,     0,  -518,  -518,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -518,     0,  -518,
    -518,  -518,  -518,  -517,  -517,  -517,     0,  -517,  -518,     0,
       0,  -517,  -517,     0,     0,     0,  -517,     0,  -517,  -517,
    -517,  -517,  -517,  -517,  -517,     0,  -517,     0,     0,  -517,
    -517,     0,     0,  -517,     0,  -517,     0,     0,  -517,  -517,
    -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,  -517,
       0,  -517,  -517,  -517,  -517,  -517,  -517,  -517,     0,     0,
    -517,  -517,  -517,     0,     0,     0,     0,  -517,  -517,     0,
    -517,  -517,  -517,  -517,  -517,     0,  -517,  -517,     0,  -517,
    -517,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -517,     0,  -517,  -517,  -517,  -517,  -520,  -520,  -520,
       0,  -520,  -517,     0,     0,  -520,  -520,     0,     0,     0,
    -520,     0,  -520,  -520,  -520,  -520,  -520,  -520,  -520,     0,
       0,     0,     0,  -520,  -520,     0,     0,  -520,     0,  -520,
       0,     0,  -520,  -520,  -520,  -520,  -520,  -520,  -520,  -520,
    -520,  -520,  -520,  -520,     0,  -520,  -520,  -520,  -520,  -520,
    -520,  -520,     0,     0,  -520,  -520,  -520,     0,     0,     0,
       0,  -520,  -520,     0,  -520,  -520,  -520,  -520,  -520,     0,
    -520,  -520,     0,  -520,  -520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -98,   721,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -519,  -519,  -519,  -520,  -519,  -520,  -520,  -520,
    -519,  -519,     0,     0,     0,  -519,  -520,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,     0,     0,     0,     0,  -519,  -519,
       0,     0,  -519,     0,  -519,     0,     0,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,     0,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,     0,     0,  -519,
    -519,  -519,     0,     0,     0,     0,  -519,  -519,     0,  -519,
    -519,  -519,  -519,  -519,     0,  -519,  -519,     0,  -519,  -519,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -96,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     3,     4,
    -519,     6,  -519,  -519,  -519,     7,     8,     0,     0,     0,
       9,  -519,    10,    11,    12,    13,    14,    15,    16,     0,
       0,     0,     0,    17,    18,     0,     0,    98,     0,    21,
       0,     0,    24,    25,    26,    27,   228,    28,    29,    30,
      31,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,     0,    43,    44,   229,     0,     0,     0,
       0,   210,    46,     0,    47,    48,   103,    50,   230,     0,
      52,   231,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -521,  -521,  -521,    55,  -521,   232,    57,    58,
    -521,  -521,     0,     0,     0,  -521,   243,  -521,  -521,  -521,
    -521,  -521,  -521,  -521,     0,     0,     0,     0,  -521,  -521,
       0,     0,  -521,     0,  -521,     0,     0,  -521,  -521,  -521,
    -521,  -521,  -521,  -521,  -521,  -521,  -521,  -521,  -521,     0,
    -521,  -521,  -521,  -521,  -521,  -521,  -521,     0,     0,  -521,
    -521,  -521,     0,     0,     0,     0,  -521,  -521,     0,  -521,
    -521,  -521,  -521,  -521,     0,  -521,  -521,     0,  -521,  -521,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -243,  -243,  -243,
    -521,  -243,  -521,  -521,  -521,  -243,  -243,     0,     0,     0,
    -243,  -521,  -243,  -243,  -243,  -243,  -243,  -243,  -243,     0,
       0,     0,     0,  -243,  -243,     0,     0,  -243,     0,  -243,
       0,     0,  -243,  -243,  -243,  -243,  -243,  -243,  -243,  -243,
    -243,  -243,  -243,  -243,     0,  -243,  -243,  -243,  -243,  -243,
    -243,  -243,     0,     0,  -243,  -243,  -243,     0,     0,     0,
       0,  -243,  -243,     0,  -243,  -243,  -243,  -243,  -243,     0,
    -243,  -243,     0,  -243,  -243,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     4,  -243,     6,  -243,  -243,  -243,
       7,     8,     0,     0,     0,     9,   348,    10,    11,    12,
      94,    95,    15,    16,     0,     0,     0,     0,    96,    97,
       0,     0,    98,     0,    21,     0,     0,    24,    25,    26,
      27,     0,    28,    29,    30,   100,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,     0,     0,     0,     0,     0,   210,    46,     0,    47,
      48,   103,    50,     0,     0,    52,     0,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,   232,    57,    58,     0,     0,     0,     0,     0,
       0,   417,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,     0,     0,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   195,   196,   197,
     198,   153,   199,   200,   201,   157,   254,    33,    34,     0,
      35,     0,   255,   256,     0,     0,     0,     0,     0,     0,
       0,     0,   158,   159,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   160,     0,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,     0,     0,     0,     0,     0,
     186,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,     0,     0,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   195,   196,   197,   198,
     153,   199,   200,   201,   157,   248,    33,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,   158,   159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   160,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,     0,     0,     0,     0,     0,   186,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,     0,     0,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   195,   196,   197,   198,   153,
     199,   200,   201,   157,     0,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
     158,   159,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   160,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,     0,     0,     0,     0,     0,   186,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,     0,     0,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   195,   196,   197,   198,   153,   199,
     200,   201,   157,     0,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   158,
     159,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     160,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,     2,     3,     4,     0,     6,   186,     0,     0,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      94,    95,    15,    16,     0,     0,     0,     0,    96,    97,
       0,     0,    98,     0,    99,     0,     0,    24,    25,    26,
      27,     0,    28,    29,    30,   100,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,     0,     0,     0,     0,     0,   281,    46,     0,    47,
      48,   103,    50,     0,     0,    52,     0,     0,   104,    54,
       0,     0,     2,     3,     4,     0,     6,     0,     0,     0,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      94,    95,    15,    16,     0,     0,     0,     0,    96,    97,
       0,     0,    98,     0,    99,     0,     0,    24,    25,    26,
      27,     0,    28,    29,    30,   100,    32,    33,    34,   282,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,     0,     0,     0,     0,     0,   281,    46,     0,    47,
      48,   103,    50,     0,     0,    52,     0,     0,   104,    54,
       0,     0,     2,     3,     4,     5,     6,     0,     0,     0,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      13,    14,    15,    16,     0,     0,     0,     0,    17,    18,
       0,     0,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,    30,    31,    32,    33,    34,   502,
      35,    36,    37,    38,    39,    40,    41,     0,    42,    43,
      44,     0,     0,     0,     0,     0,    45,    46,     0,    47,
      48,    49,    50,    51,     0,    52,     0,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     3,     4,
      55,     6,    56,    57,    58,     7,     8,     0,     0,     0,
       9,     0,    10,    11,    12,    13,    14,    15,    16,     0,
       0,     0,     0,    17,    18,     0,     0,    98,     0,    21,
       0,     0,    24,    25,    26,    27,   228,    28,    29,    30,
      31,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,     0,    43,    44,   229,     0,     0,     0,
       0,   210,    46,     0,    47,    48,   103,    50,   230,     0,
      52,   231,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     4,    55,     6,   232,    57,    58,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      94,    95,    15,    16,     0,     0,     0,     0,    96,    97,
       0,     0,    98,     0,    21,     0,     0,    24,    25,    26,
      27,   228,    28,    29,    30,   100,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,   229,     0,     0,     0,     0,   210,    46,     0,    47,
      48,   103,    50,   604,     0,    52,   231,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     3,     4,
      55,     6,   232,    57,    58,     7,     8,     0,     0,     0,
       9,     0,    10,    11,    12,    94,    95,    15,    16,     0,
       0,     0,     0,    96,    97,     0,     0,    98,     0,    21,
       0,     0,    24,    25,    26,    27,   228,    28,    29,    30,
     100,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,     0,    43,    44,     0,     0,     0,     0,
       0,   210,    46,     0,    47,    48,   103,    50,   230,     0,
      52,   231,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     4,    55,     6,   232,    57,    58,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      94,    95,    15,    16,     0,     0,     0,     0,    96,    97,
       0,     0,    98,     0,    21,     0,     0,    24,    25,    26,
      27,   228,    28,    29,    30,   100,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,   229,     0,     0,     0,     0,   210,    46,     0,    47,
      48,   103,    50,     0,     0,    52,   231,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     3,     4,
      55,     6,   232,    57,    58,     7,     8,     0,     0,     0,
       9,     0,    10,    11,    12,    94,    95,    15,    16,     0,
       0,     0,     0,    96,    97,     0,     0,    98,     0,    21,
       0,     0,    24,    25,    26,    27,   228,    28,    29,    30,
     100,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,     0,    43,    44,     0,     0,     0,     0,
       0,   210,    46,     0,    47,    48,   103,    50,   604,     0,
      52,   231,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     4,    55,     6,   232,    57,    58,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      94,    95,    15,    16,     0,     0,     0,     0,    96,    97,
       0,     0,    98,     0,    21,     0,     0,    24,    25,    26,
      27,   228,    28,    29,    30,   100,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,     0,     0,     0,     0,     0,   210,    46,     0,    47,
      48,   103,    50,     0,     0,    52,   231,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     3,     4,
      55,     6,   232,    57,    58,     7,     8,     0,     0,     0,
       9,     0,    10,    11,    12,    13,    14,    15,    16,     0,
       0,     0,     0,    17,    18,     0,     0,    98,     0,    21,
       0,     0,    24,    25,    26,    27,     0,    28,    29,    30,
      31,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,     0,    43,    44,     0,     0,     0,     0,
       0,   210,    46,     0,    47,    48,   103,    50,   496,     0,
      52,     0,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     4,    55,     6,   232,    57,    58,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      94,    95,    15,    16,     0,     0,     0,     0,    96,    97,
       0,     0,    98,     0,    21,     0,     0,    24,    25,    26,
      27,     0,    28,    29,    30,   100,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,     0,     0,     0,     0,     0,   210,    46,     0,    47,
      48,   103,    50,   230,     0,    52,     0,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     3,     4,
      55,     6,   232,    57,    58,     7,     8,     0,     0,     0,
       9,     0,    10,    11,    12,    94,    95,    15,    16,     0,
       0,     0,     0,    96,    97,     0,     0,    98,     0,    21,
       0,     0,    24,    25,    26,    27,     0,    28,    29,    30,
     100,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,     0,    43,    44,     0,     0,     0,     0,
       0,   210,    46,     0,    47,    48,   103,    50,   786,     0,
      52,     0,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     4,    55,     6,   232,    57,    58,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      94,    95,    15,    16,     0,     0,     0,     0,    96,    97,
       0,     0,    98,     0,    21,     0,     0,    24,    25,    26,
      27,     0,    28,    29,    30,   100,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,     0,     0,     0,     0,     0,   210,    46,     0,    47,
      48,   103,    50,   496,     0,    52,     0,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     3,     4,
      55,     6,   232,    57,    58,     7,     8,     0,     0,     0,
       9,     0,    10,    11,    12,    94,    95,    15,    16,     0,
       0,     0,     0,    96,    97,     0,     0,    98,     0,    21,
       0,     0,    24,    25,    26,    27,     0,    28,    29,    30,
     100,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,     0,    43,    44,     0,     0,     0,     0,
       0,   210,    46,     0,    47,    48,   103,    50,   604,     0,
      52,     0,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     4,    55,     6,   232,    57,    58,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      13,    14,    15,    16,     0,     0,     0,     0,    17,    18,
       0,     0,    19,     0,    21,     0,     0,    24,    25,    26,
      27,     0,    28,    29,    30,    31,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,     0,     0,     0,     0,     0,   210,    46,     0,    47,
      48,   103,    50,     0,     0,    52,     0,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     3,     4,
      55,     6,    56,    57,    58,     7,     8,     0,     0,     0,
       9,     0,    10,    11,    12,    94,    95,    15,    16,     0,
       0,     0,     0,    96,    97,     0,     0,    98,     0,    21,
       0,     0,    24,    25,    26,    27,     0,    28,    29,    30,
     100,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,     0,    43,    44,     0,     0,     0,     0,
       0,   210,    46,     0,    47,    48,   103,    50,     0,     0,
      52,     0,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     4,    55,     6,   232,    57,    58,
       7,     8,     0,     0,     0,     9,     0,    10,    11,    12,
      13,    14,    15,    16,     0,     0,     0,     0,    17,    18,
       0,     0,    98,     0,    21,     0,     0,    24,    25,    26,
      27,     0,    28,    29,    30,    31,    32,    33,    34,     0,
      35,    36,    37,    38,    39,    40,    41,     0,     0,    43,
      44,     0,     0,     0,     0,     0,   210,    46,     0,    47,
      48,   103,    50,     0,   153,    52,     0,     0,    53,    54,
     101,    34,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   158,   159,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   160,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,   232,    57,    58,     0,     0,     0,     0,     0,
       0,     0,     0,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,     0,   153,
       0,     0,     0,   186,   348,    33,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,   159,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   160,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,     2,     3,     4,     0,     6,   186,     0,
       0,     7,     8,     0,     0,     0,     9,     0,    10,    11,
      12,    94,    95,    15,    16,     0,     0,     0,     0,    96,
      97,     0,     0,    98,     0,    99,     0,     0,    24,    25,
      26,    27,     0,    28,    29,    30,   100,    32,    33,    34,
       0,    35,    36,    37,    38,    39,    40,    41,     0,     0,
      43,    44,     0,     0,     0,     0,     0,   221,    46,     0,
      47,    48,    49,    50,    51,     0,    52,     0,     0,   104,
      54,     2,     3,     4,     0,     6,     0,     0,     0,     7,
       8,     0,     0,     0,     9,     0,    10,    11,    12,    94,
      95,    15,    16,     0,     0,     0,     0,    96,    97,     0,
       0,    98,     0,    99,     0,     0,    24,    25,    26,    27,
       0,    28,    29,    30,   100,    32,    33,    34,     0,    35,
      36,    37,    38,    39,    40,    41,     0,     0,    43,    44,
       0,     0,     0,     0,     0,   281,    46,     0,    47,    48,
     305,    50,   306,     0,    52,     0,     0,   104,    54,     2,
       3,     4,     0,     6,     0,     0,     0,     7,     8,     0,
       0,     0,     9,     0,    10,    11,    12,    94,    95,    15,
      16,     0,     0,     0,     0,    96,    97,     0,     0,    98,
       0,    99,     0,     0,    24,    25,    26,    27,     0,    28,
      29,    30,   100,    32,   101,    34,     0,    35,    36,    37,
      38,    39,    40,    41,     0,     0,    43,    44,     0,     0,
       0,     0,     0,   102,    46,     0,    47,    48,   103,    50,
       0,     0,    52,     0,     0,   104,    54,     2,     3,     4,
       0,     6,     0,     0,     0,     7,     8,     0,     0,     0,
       9,     0,    10,    11,    12,    94,    95,    15,    16,     0,
       0,     0,     0,    96,    97,     0,     0,    98,     0,    99,
       0,     0,    24,    25,    26,    27,     0,    28,    29,    30,
     100,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,     0,     0,    43,    44,     0,     0,     0,     0,
       0,   281,    46,     0,    47,    48,   305,    50,     0,     0,
      52,     0,     0,   104,    54,     2,     3,     4,     0,     6,
       0,     0,     0,     7,     8,     0,     0,     0,     9,     0,
      10,    11,    12,    94,    95,    15,    16,     0,     0,     0,
       0,    96,    97,     0,     0,    98,     0,    99,     0,     0,
      24,    25,    26,    27,     0,    28,    29,    30,   100,    32,
      33,    34,     0,    35,    36,    37,    38,    39,    40,    41,
       0,     0,    43,    44,     0,     0,     0,     0,     0,   783,
      46,     0,    47,    48,   103,    50,     0,     0,    52,     0,
       0,   104,    54,     2,     3,     4,     0,     6,     0,     0,
       0,     7,     8,     0,     0,     0,     9,     0,    10,    11,
      12,    94,    95,    15,    16,     0,     0,     0,     0,    96,
      97,     0,     0,    98,     0,    99,     0,     0,    24,    25,
      26,    27,     0,    28,    29,    30,   100,    32,    33,    34,
       0,    35,    36,    37,    38,    39,    40,    41,     0,     0,
      43,    44,     0,     0,     0,     0,     0,   907,    46,     0,
      47,    48,   103,    50,     0,     0,    52,     0,     0,   104,
      54
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-702)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-541)))

static const yytype_int16 yycheck[] =
{
       4,     5,   216,    71,     4,     4,     5,    48,   335,   368,
     337,   338,   339,   102,     5,   348,    20,    20,    13,    14,
     238,    20,    17,    18,    56,   388,   308,   269,   270,    20,
      49,   364,    19,   628,    21,   444,    31,   262,   365,    43,
     392,     0,     1,    43,    43,    89,    90,   588,   658,     0,
     383,     2,   636,     9,   569,   382,   262,   384,     9,   474,
     428,    51,   395,    47,    48,    13,    79,   111,   741,   396,
     585,   340,   341,    13,    26,   739,   492,   778,    13,    25,
      13,    26,    42,     1,   599,    26,    66,   502,    13,    14,
     405,   273,    17,    18,    66,    66,    31,    32,    66,    25,
      66,    13,    62,    13,    31,    32,    31,    13,    25,    66,
      99,    47,    99,   492,    66,   624,    25,    77,    62,    53,
      50,    57,   111,   305,    15,    27,    17,    25,   455,    31,
      32,    93,    31,    32,   130,    31,    32,   652,   130,    69,
     136,    25,   457,   476,   136,    92,    93,    25,   128,   129,
     477,   660,   132,   113,    25,   106,   128,   129,   129,    25,
     128,   129,   122,   129,   370,    66,    92,   831,   130,   113,
     104,   128,   129,   714,   126,   136,   128,   129,   773,    66,
     132,   126,   130,   130,   130,   126,   132,   135,   136,   126,
     108,   132,   132,   894,    92,   135,   136,     7,     8,     9,
     135,   136,   135,   136,   130,   564,    66,   133,   433,   136,
     136,   884,   628,   130,   282,   132,   575,   135,   136,    49,
     636,   130,   886,   135,   136,   135,   136,   433,   129,   135,
     136,     0,   130,   135,   136,   133,   135,   136,   136,   849,
      50,   446,   129,   448,   492,   258,   130,   842,   243,   617,
     618,   915,   130,   837,   273,   664,   665,   636,   262,   130,
     639,   217,   303,   678,   130,    66,   217,   308,   128,   129,
      51,   303,    92,    93,   363,    66,   308,   132,   684,   266,
      28,   136,   269,   270,   825,   691,   305,    92,    66,   130,
     559,   115,    66,   337,     0,     1,     2,     3,    92,    80,
      66,     7,     8,     9,   899,   132,    12,    13,    14,    66,
     130,    17,    18,   272,   346,   347,   306,   132,   243,   275,
      92,   365,    92,    93,   275,    31,   402,   128,   129,   405,
      66,   335,   132,   337,   338,   339,   335,   128,   129,   298,
     384,   336,   751,    49,    50,    51,   705,   135,   389,   574,
      56,   129,   396,   348,    26,   129,   351,   773,   130,   363,
     130,   365,   778,   129,   363,    71,   370,    66,   574,   364,
     130,   739,   129,   741,    10,    66,    92,   591,   382,    93,
     384,   733,   630,   382,    25,    66,   634,   130,   383,   607,
     394,    17,   396,   129,    66,   394,    26,   103,   130,   778,
     395,   780,   644,   126,    66,   127,   474,    66,   359,   219,
     220,   455,   416,   416,   134,    92,    93,   416,   402,    52,
      66,   837,    55,    56,   428,   416,   351,   942,    61,   433,
     129,   713,   133,   477,   502,   245,    66,   247,   129,   443,
     391,   392,    66,   447,    92,   449,   855,   128,   129,   122,
     454,   455,    26,   130,   126,   454,   128,   129,   837,   130,
     839,   122,   825,   831,   832,   844,   128,   129,    87,   128,
     129,   475,   130,   477,    92,    93,   475,   461,   894,    48,
      49,   476,   128,   129,   294,   295,   296,   297,   492,   299,
     300,    13,    66,   132,   538,    10,   126,    48,   128,   129,
     127,   245,   132,   547,   128,   129,   550,   551,   133,   468,
     869,    92,   130,   219,   220,   894,   884,   896,   886,    92,
      93,   508,   901,   132,   568,   773,   570,   775,   135,   136,
     778,   130,   780,   277,    92,    93,   130,   243,   917,   245,
     130,   247,   130,   748,   749,   130,   252,   915,   753,    10,
     755,   756,   126,   540,   128,   129,   366,   936,   132,   369,
      81,    82,    47,    84,    85,   130,   272,   273,    53,    54,
     574,   130,    57,   577,   578,   578,   282,   577,   577,   578,
     648,   130,   569,    14,    15,    92,    93,   578,   294,   295,
     296,   297,   298,   299,   300,   130,   929,   303,   585,   305,
     306,   815,   308,   930,     8,   819,   812,   417,    92,    93,
     678,    10,   599,   617,   618,    47,    48,    49,    92,    93,
     571,    53,    54,    15,   628,   576,   127,    47,    48,    49,
     336,   130,   636,    53,    54,   639,   111,    47,    48,    49,
     346,   347,   348,    53,    54,   351,   894,   135,   896,   681,
     682,   899,   130,   901,   658,   687,   688,   644,   364,   130,
     366,   130,    72,   369,   408,   652,   538,   411,   550,   551,
     376,   492,   713,   379,   130,   547,    92,   383,   550,   551,
      10,   387,    96,   888,   889,   890,   891,   134,   936,   395,
     116,   117,   118,   119,   120,    42,   122,   252,   130,    10,
      47,    48,    49,   127,    10,   130,    53,    54,    10,    10,
     130,   417,    10,   419,    10,    62,    47,    48,    49,   127,
     127,   465,    53,    54,   130,    72,   470,    74,   130,   934,
      77,   111,   118,   119,   120,   739,   122,   741,    10,   130,
     130,     0,     1,   130,    65,     9,   130,    10,     7,     8,
       9,   702,    10,   563,    13,    14,   127,   501,    17,    18,
     130,   130,   468,   130,   130,   797,   113,   130,   474,   773,
     476,   118,    31,   130,   778,   122,   780,   667,   668,   132,
      10,    10,   733,    88,    89,    90,    91,   493,   135,    94,
      49,    50,   602,   130,    12,    42,   502,    56,     3,   912,
      47,    48,    49,   617,   492,   906,    53,    54,   812,   630,
     849,    77,   630,   275,   558,    62,    47,    48,    49,    43,
     775,   376,    53,    54,   379,    72,   812,   831,   832,   573,
      77,   773,   387,   837,    -1,   839,   628,    -1,   842,    -1,
     844,    72,    -1,    74,   103,   849,   552,   553,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,   563,   862,    -1,
      -1,    -1,    -1,   862,   419,    -1,   113,    -1,    -1,   613,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   621,    -1,    -1,
     884,    -1,   886,    -1,    -1,    -1,   930,    47,    48,    49,
     894,    -1,   896,    53,    54,   899,   602,   901,    -1,    -1,
      -1,    -1,    -1,    47,    48,    -1,   716,    -1,    -1,    -1,
      -1,   915,    72,   917,    74,    -1,    42,    -1,    -1,   625,
     626,    47,    48,    49,   928,    -1,   930,    53,    54,   928,
      -1,    -1,   936,    77,   929,    -1,    62,    -1,   493,    -1,
      -1,    -1,   648,    -1,    -1,    -1,    72,    -1,    74,    -1,
      -1,    77,   773,    -1,   775,   942,    -1,   778,    -1,   780,
     219,   220,   852,   853,   708,   671,   856,    -1,   858,   859,
      -1,    -1,   678,    -1,    -1,   681,   682,    -1,    -1,    -1,
      -1,   687,   688,    -1,   243,    -1,   245,   113,   247,    -1,
      -1,    -1,   118,   252,   738,   701,   122,   552,   553,   743,
      -1,    -1,     2,     3,     4,     5,   132,    -1,    -1,    -1,
     716,   717,    -1,   272,   273,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,   729,   730,   731,    -1,    -1,    -1,    -1,
      -1,   921,   922,   923,   924,   294,   295,   296,   297,   298,
     299,   300,    -1,    43,   303,    45,   305,    -1,    -1,   308,
      -1,    -1,    -1,    -1,    -1,   761,   946,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    -1,   122,    -1,
     625,   626,    -1,   894,    -1,   896,   782,   336,   899,    -1,
     901,    -1,    -1,    -1,    -1,    -1,    -1,   346,   347,   348,
      -1,   797,   351,   237,   238,   114,   115,   116,   117,   118,
     119,   120,   102,   122,    -1,   364,    -1,   366,    -1,    -1,
     369,    -1,    -1,    -1,    -1,   936,    -1,   376,   262,   262,
     379,    -1,    -1,    -1,   383,    -1,    -1,    -1,   387,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   395,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   701,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   301,   417,    -1,
     419,    -1,   717,    -1,   870,   871,    -1,    -1,   874,    -1,
      -1,   877,    -1,    42,   729,   730,   731,   883,    47,    48,
      49,    -1,    -1,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,   336,    62,    -1,    -1,   340,   341,    -1,    -1,
      -1,    -1,    -1,    72,   348,    74,   761,    -1,    77,   468,
     210,    -1,    -1,    -1,    -1,    -1,    -1,   476,    -1,    -1,
     364,   221,    -1,   929,    -1,    42,   370,   370,    -1,    -1,
      47,    48,    49,    -1,   493,    -1,    53,    54,    -1,   383,
      -1,    -1,    -1,    -1,   113,    62,    -1,    -1,    -1,   118,
      -1,   395,    -1,   122,    -1,    72,    -1,    74,    -1,    -1,
      77,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     0,     1,     2,     3,     4,    -1,
      -1,   281,    -1,    -1,    -1,    -1,    12,    -1,    -1,   433,
     433,    -1,    -1,   552,   553,    -1,   113,    -1,   442,    -1,
     444,   118,    -1,    -1,   563,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   870,    -1,    -1,    -1,   874,
      -1,    -1,   877,    49,    -1,    51,    -1,    -1,   883,    -1,
      -1,    -1,   476,    -1,    -1,   335,    -1,   337,   338,   339,
      -1,    -1,    -1,   602,    -1,    71,    -1,    -1,   492,   492,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   363,    -1,   365,   625,   626,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,   382,    -1,   384,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   394,    -1,   396,    -1,    -1,    -1,
      -1,    -1,    -1,   547,    -1,    -1,   550,   551,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   559,   416,    -1,    -1,    -1,
      -1,    -1,   681,   682,    -1,    -1,   570,    -1,   687,   688,
     574,   574,    -1,    -1,    -1,   579,   580,    -1,    -1,    -1,
      -1,    -1,   701,    -1,   588,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   454,   455,    -1,   716,   717,    -1,
      -1,    -1,    -1,   607,    -1,    -1,    -1,    -1,    -1,    -1,
     729,   730,   731,    -1,    -1,   475,    -1,   477,    -1,    -1,
     624,    -1,    -1,    -1,    -1,   628,    -1,   630,    -1,    -1,
      -1,    -1,    -1,   636,    -1,    -1,   639,    -1,    -1,    -1,
      13,    14,   761,    -1,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   660,    -1,    31,    -1,
     664,   665,    -1,    -1,    -1,    -1,   252,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    48,    -1,    -1,   797,    -1,
      -1,    -1,    55,    56,    57,    58,   272,   273,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,   282,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   713,
     714,    -1,   298,    -1,    -1,    -1,    -1,   577,   578,   305,
     306,    95,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   751,   122,    -1,
      -1,   870,    -1,    -1,    -1,   874,    -1,    -1,   877,    -1,
      -1,    -1,    -1,    -1,   883,    -1,    -1,    -1,    -1,    -1,
     773,    -1,   775,    -1,    -1,   778,    -1,   780,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     376,    -1,    -1,   379,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   387,    -1,    -1,    -1,    -1,    -1,    -1,   812,   812,
     929,    -1,   816,    -1,   818,    -1,    -1,    -1,    -1,    -1,
      -1,   825,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   419,   837,    -1,   839,    -1,    -1,   842,
      -1,   844,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   855,    -1,    -1,    -1,   228,   229,   230,    -1,   232,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   240,    -1,    -1,
     243,    -1,    -1,    -1,    -1,    -1,    -1,   250,    -1,    -1,
      -1,    -1,   468,    -1,    -1,     0,     1,    -1,   474,    -1,
      -1,   894,    -1,   896,    -1,    -1,   899,    12,   901,    -1,
      -1,    -1,   906,    -1,    -1,    -1,    -1,   493,   912,    -1,
      -1,    -1,    -1,    -1,   917,    -1,   502,    -1,    -1,    -1,
      -1,    -1,    -1,   783,    -1,   929,    -1,    -1,    -1,    -1,
     303,    -1,    -1,   936,    49,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,    -1,   336,    -1,    -1,   552,   553,    -1,    -1,
      -1,    -1,    -1,   346,   347,   348,    -1,    -1,   351,    -1,
      -1,    -1,    -1,    -1,     0,     1,     2,     3,   103,    -1,
      -1,   364,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   862,    -1,    -1,    -1,    -1,    -1,   381,    -1,
     383,    -1,   385,   386,    -1,    -1,   389,    -1,    -1,    -1,
      -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,   402,
      -1,    -1,   405,    49,    -1,    51,    -1,    -1,    -1,   625,
     626,    -1,    -1,    -1,    -1,    -1,    -1,   907,   421,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,   648,    -1,    -1,    -1,    -1,    -1,   928,    -1,
     930,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   452,
      -1,    -1,    -1,    -1,   457,   671,    -1,   103,   461,    -1,
      -1,    -1,   678,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   476,    -1,   478,    -1,    -1,    -1,    -1,
       0,    -1,    -1,    -1,    -1,   701,    -1,    -1,     8,     9,
      10,    -1,    -1,   496,    14,    15,    -1,    17,    -1,    -1,
      -1,   717,    -1,    -1,    -1,    -1,    -1,   252,    -1,    -1,
      -1,    31,    32,   729,   730,   731,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   272,   273,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   761,    66,    -1,    -1,    -1,
      -1,    -1,    -1,   298,    -1,    -1,    -1,    -1,    -1,    -1,
     305,    -1,    -1,    -1,    -1,    -1,   782,    -1,    88,    89,
      90,    91,    92,    93,    94,    95,    -1,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   604,   122,    -1,    -1,    -1,   252,   127,   128,   129,
     130,    -1,    -1,   133,    -1,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   272,   273,    -1,    -1,
      -1,   376,    -1,    -1,   379,    -1,   282,    -1,    -1,    -1,
      -1,    -1,   387,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   298,    -1,   870,   871,    -1,    -1,   874,   305,
     306,   877,    -1,    -1,    -1,    -1,    -1,   883,    -1,    -1,
      -1,    -1,   675,    -1,   419,    -1,   679,    -1,   681,   682,
      -1,    -1,    -1,    -1,   687,   688,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   699,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     713,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   721,   722,
      -1,   724,   725,   468,    -1,   728,    -1,    -1,    -1,    -1,
     376,    -1,    -1,   379,    -1,    -1,     0,    -1,    -1,    -1,
      -1,   387,    -1,    -1,     8,     9,    10,    -1,   493,    13,
      14,    15,    -1,    17,   757,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,   419,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   786,    -1,    -1,    -1,   790,    -1,    -1,
      -1,    -1,    -1,    -1,   797,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,   552,   553,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   822,
      -1,    -1,   468,    -1,    88,    89,    90,    91,   474,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   493,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   502,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,    -1,   132,   133,
      -1,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     625,   626,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,   552,   553,    -1,    11,
      12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,   929,    29,    30,    -1,
      -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      52,    53,    54,    55,    56,    57,   701,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    73,   717,    -1,    76,    -1,    -1,    79,    80,   625,
     626,    -1,    -1,    -1,   729,   730,   731,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   648,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   657,    -1,    -1,    -1,    -1,   761,    -1,    -1,   121,
      -1,   123,   124,   125,    -1,   671,    -1,    -1,    -1,    42,
      -1,    -1,   678,   135,   136,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,   701,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   717,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   729,   730,   731,    -1,    -1,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,    -1,   761,    -1,    -1,   131,   132,
      -1,    -1,    -1,    -1,    -1,   870,   871,    -1,    -1,   874,
      -1,    -1,   877,    -1,    -1,    -1,   782,    -1,   883,     0,
       1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    76,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,   870,   871,     0,    -1,   874,    -1,
      -1,   877,    -1,    -1,     8,     9,    10,   883,    -1,    -1,
      14,    15,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    31,    32,    -1,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    89,    90,    91,    92,    93,
      94,    95,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    -1,   122,    -1,
      -1,    -1,   126,   127,   128,   129,   130,    -1,    -1,   133,
      -1,   135,   136,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    29,    30,    -1,    -1,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    45,    46,    47,
      48,    49,    -1,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    73,    74,    -1,    76,    -1,
      -1,    79,    80,    95,    -1,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,   136,   123,   124,   125,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,   135,   136,    10,
      11,    12,    -1,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    76,    -1,    -1,    79,    80,
      94,    95,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,    -1,    -1,     1,    -1,     3,
       4,     5,     6,     7,   135,   136,    10,    11,    12,    -1,
      -1,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    68,    -1,    70,    71,    72,    73,
      74,    -1,    76,    -1,    -1,    79,    80,    95,    -1,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,   121,    -1,   123,
     124,   125,    -1,    -1,     1,    -1,     3,     4,     5,     6,
       7,   135,   136,    10,    11,    12,    -1,    -1,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    29,    30,    -1,    -1,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    73,    74,    -1,    76,
      -1,    -1,    79,    80,    95,    -1,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,   121,    -1,   123,   124,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,   136,
       1,    -1,     3,     4,     5,     6,     7,    -1,     9,    10,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    76,    -1,    -1,    79,    80,
      95,    -1,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,    -1,     1,    -1,     3,     4,
       5,     6,     7,    -1,   135,   136,    11,    12,    -1,    -1,
      -1,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    61,    -1,    -1,    -1,
      -1,    -1,    67,    68,    -1,    70,    71,    72,    73,    74,
      -1,    76,    -1,    -1,    79,    80,    95,    -1,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,   121,    -1,   123,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
     135,   136,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      29,    30,    -1,    -1,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      -1,    70,    71,    72,    73,    74,    -1,    76,    -1,    -1,
      79,    80,    94,    95,    -1,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,   123,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,   135,   136,     1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    45,    46,    47,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    -1,    70,    71,    72,
      73,    74,    -1,    76,    -1,    -1,    79,    80,    94,    95,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,   124,   125,    -1,   127,     1,    -1,     3,     4,     5,
       6,     7,   135,   136,    10,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    -1,    -1,    79,    80,    95,    -1,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,   124,   125,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,   135,
     136,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      30,    -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      60,    61,    62,    -1,    -1,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    73,    74,    -1,    76,    77,    -1,    79,
      80,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   136,    -1,    -1,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,    -1,
      -1,    38,    39,    40,    41,    -1,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,
      67,    68,    -1,    70,    71,    72,    73,    -1,    -1,    76,
      -1,    -1,    79,    80,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,   124,   125,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,   136,
      -1,    -1,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    33,
      -1,    35,    -1,    -1,    38,    39,    40,    41,    -1,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    68,    -1,    70,    71,    72,    73,
      -1,    -1,    76,    -1,    -1,    79,    80,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    -1,   122,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      30,   135,   136,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    73,    74,    -1,    76,    -1,    -1,    79,
      80,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    -1,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,     3,
       4,     5,    -1,     7,   131,   132,    -1,    11,    12,    -1,
      -1,    -1,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    26,    -1,    -1,    29,    30,    -1,    -1,    33,
      -1,    35,    -1,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    60,    61,    62,    -1,
      -1,    -1,    -1,    67,    68,    -1,    70,    71,    72,    73,
      74,    -1,    76,    77,    -1,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
     124,   125,   126,     3,     4,     5,    -1,     7,   132,    -1,
      -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    -1,    -1,    29,
      30,    -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      60,    61,    62,    -1,    -1,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    73,    74,    -1,    76,    77,    -1,    79,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,     3,     4,     5,
      -1,     7,   132,    -1,    -1,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    62,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    77,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,   132,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    62,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    76,    77,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
     121,     7,   123,   124,   125,    11,    12,    -1,    -1,    -1,
      16,   132,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    62,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    77,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,   132,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    62,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    76,    77,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
     121,     7,   123,   124,   125,    11,    12,    -1,    -1,    -1,
      16,   132,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    62,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    77,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,   132,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    -1,    -1,    76,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,   132,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
     131,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,   131,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,    -1,    -1,    -1,    -1,   131,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,     3,     4,     5,    -1,     7,   131,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    -1,    -1,    76,    -1,    -1,    79,    80,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,   130,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    -1,    -1,    76,    -1,    -1,    79,    80,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,   130,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    76,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
     121,     7,   123,   124,   125,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    62,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    77,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    62,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    76,    77,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
     121,     7,   123,   124,   125,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    77,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    62,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    -1,    -1,    76,    77,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
     121,     7,   123,   124,   125,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    77,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    -1,    -1,    76,    77,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
     121,     7,   123,   124,   125,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    76,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
     121,     7,   123,   124,   125,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    74,    -1,    76,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
     121,     7,   123,   124,   125,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    74,    -1,
      76,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    -1,    -1,    76,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
     121,     7,   123,   124,   125,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    -1,    -1,
      76,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,   121,     7,   123,   124,   125,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,
      41,    -1,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,
      71,    72,    73,    -1,    42,    76,    -1,    -1,    79,    80,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    42,
      -1,    -1,    -1,   131,   132,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,     3,     4,     5,    -1,     7,   131,    -1,
      -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      30,    -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    73,    74,    -1,    76,    -1,    -1,    79,
      80,     3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,
      12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,    -1,
      -1,    33,    -1,    35,    -1,    -1,    38,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    67,    68,    -1,    70,    71,
      72,    73,    74,    -1,    76,    -1,    -1,    79,    80,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,    -1,
      -1,    -1,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    33,
      -1,    35,    -1,    -1,    38,    39,    40,    41,    -1,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    60,    61,    -1,    -1,
      -1,    -1,    -1,    67,    68,    -1,    70,    71,    72,    73,
      -1,    -1,    76,    -1,    -1,    79,    80,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    -1,    -1,    33,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    -1,    70,    71,    72,    73,    -1,    -1,
      76,    -1,    -1,    79,    80,     3,     4,     5,    -1,     7,
      -1,    -1,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    29,    30,    -1,    -1,    33,    -1,    35,    -1,    -1,
      38,    39,    40,    41,    -1,    43,    44,    45,    46,    47,
      48,    49,    -1,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    67,
      68,    -1,    70,    71,    72,    73,    -1,    -1,    76,    -1,
      -1,    79,    80,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      30,    -1,    -1,    33,    -1,    35,    -1,    -1,    38,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      60,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,    -1,
      70,    71,    72,    73,    -1,    -1,    76,    -1,    -1,    79,
      80
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,    11,    12,    16,
      18,    19,    20,    21,    22,    23,    24,    29,    30,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    43,    44,
      45,    46,    47,    48,    49,    51,    52,    53,    54,    55,
      56,    57,    59,    60,    61,    67,    68,    70,    71,    72,
      73,    74,    76,    79,    80,   121,   123,   124,   125,   135,
     136,   138,   139,   145,   148,   149,   150,   152,   153,   155,
     156,   157,   159,   160,   169,   181,   215,   216,   226,   227,
     228,   235,   237,   238,   239,   240,   241,   270,   271,   275,
     279,   286,   288,   145,    21,    22,    29,    30,    33,    35,
      46,    48,    67,    72,    79,   161,   162,   181,   239,   241,
     270,   275,   281,   286,   287,   162,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    63,    64,
      74,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   131,   132,   163,   167,
     168,   239,   268,   270,   271,    38,    39,    40,    41,    43,
      44,    45,   163,   164,   165,   235,   270,   271,   140,   141,
      67,   148,   160,   181,   239,   241,   148,   148,   281,   183,
     185,    67,   153,   160,   181,   198,   239,   241,    42,    62,
      74,    77,   123,   152,   169,   174,   177,   179,   273,   274,
     278,   174,   174,   132,   174,   132,   136,   282,    47,   164,
     282,   147,   126,   174,    47,    53,    54,   163,   227,   236,
     270,   210,   211,   169,   181,   272,   273,   288,   170,   179,
     273,   288,     1,    72,   143,   144,   145,   154,   155,   288,
     148,    67,   130,   159,   181,   239,   238,   229,   169,   149,
     169,   169,   169,     0,    88,    89,    90,    91,    94,    31,
      32,    28,   214,    92,   130,    72,    74,   156,    92,    95,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   122,    66,   128,   129,    66,   129,
      26,   126,   217,   218,   288,   228,    93,    93,   132,   171,
     175,   176,   218,   171,   282,    48,   161,   270,   238,     1,
     108,   242,   286,    66,   128,   129,   115,   135,   190,   269,
     191,    66,   129,   280,   130,    10,   142,   270,    13,   194,
     286,    92,    66,   128,   129,    93,    93,   194,   281,    17,
     219,   148,   148,   270,    66,   128,   129,    25,   169,   169,
     169,    65,   130,   178,   288,   130,   178,   169,   174,   282,
     283,   148,   283,   148,    47,    57,   146,   132,   169,   126,
     143,    93,    42,    47,    49,    53,    54,    62,    72,    74,
     113,   118,   122,   132,   212,   245,   247,   248,   249,   250,
     251,   252,   255,   256,   257,   258,   262,   263,   264,   265,
     266,   271,   278,   288,    66,   129,   127,   130,   282,   285,
     134,   130,   285,   285,   145,   154,   133,   281,   287,   283,
     182,   270,   156,   158,   130,    66,   128,   129,   122,    81,
      82,    84,    85,   230,   148,   148,   148,   148,   145,   148,
     148,    99,   111,   205,   206,   288,    74,   149,   169,   179,
     180,   154,   130,   159,   130,   149,   169,   180,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   167,   171,   270,   271,   276,   277,
     173,   174,   288,   167,   171,   270,   271,   276,   270,   271,
     276,   276,   205,   205,   149,   169,   149,   169,   173,    87,
     151,   174,   286,   243,   189,   161,   270,   271,   276,   173,
     276,   148,   140,   148,   132,   244,   247,   192,   166,   143,
     143,    13,   169,   270,   271,   173,   270,   271,   143,   219,
     169,   179,    10,    27,   195,   286,   195,   270,   271,   173,
     270,   271,   187,   169,    74,   169,   177,   273,   274,   283,
     133,   283,   164,   148,   143,   127,   169,    72,    74,   199,
     200,   201,   249,   271,   247,    50,    69,   213,   130,   246,
     130,   267,   288,   271,   130,   267,   130,   246,   271,   130,
     246,   271,    92,   169,   273,   283,   145,   283,   130,   158,
     270,   271,   173,   270,   271,   169,   231,   233,   135,   202,
     204,   207,   251,   253,   254,   257,   260,   261,   264,   266,
     271,   278,   288,   143,   169,   130,   283,   158,   130,    94,
     282,    93,    93,   171,   175,   282,   284,    93,    93,   171,
     172,   175,   288,   172,   175,   172,   175,   143,   143,    94,
     283,   205,   148,   140,   284,   286,    10,   283,   247,   140,
     286,   163,   164,     8,   221,   288,    14,    15,   196,   197,
     288,    93,    93,   284,    93,    93,   197,    10,   130,   194,
     184,   186,   284,   148,   169,   178,   283,   127,   201,   130,
     249,   130,   283,   207,   143,   143,   245,   250,   262,   264,
     252,   257,   266,   251,   258,   264,   251,   259,   285,   156,
     284,   232,    47,    53,    54,   234,   241,   208,   209,   248,
     271,   207,   111,   130,   203,   130,   267,   267,   130,   203,
     130,   203,    92,    67,   181,    10,    74,   169,   158,   169,
      96,   149,   169,   149,   169,   151,   134,    93,   149,   169,
     149,   169,   151,    10,   127,   169,   143,   286,    10,   140,
     283,    10,   193,   169,   180,   222,   223,   288,   197,   148,
     143,    10,    93,    10,   169,   143,   143,   143,   195,   283,
     200,   130,    74,   199,   283,    10,   127,   130,   246,   130,
     246,   267,   130,   246,   130,   246,   246,   169,   143,   130,
     111,   202,   261,   264,   253,   257,   251,   260,   264,   251,
     181,   270,    66,   169,   169,   149,   169,   127,    10,   244,
     194,    65,   224,   288,     9,   225,   288,   194,   197,   219,
     220,    10,    10,   188,   130,   200,   130,   249,   251,   264,
     251,   251,   127,   209,   130,   203,   130,   203,   267,   130,
     203,   130,   203,   203,   270,   140,   143,    67,   160,   181,
     239,   143,   143,   143,   200,   130,   246,   130,   246,   246,
     246,   251,   264,   251,   251,    10,   221,   270,    66,   128,
     129,   196,    10,   200,   251,   203,   130,   203,   203,   203,
     270,   271,   173,   270,   271,   246,   251,   284,   203
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

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
      yyerror (parser, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, parser)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, parser); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct parser_t *parser)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    struct parser_t *parser;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (parser);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, struct parser_t *parser)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, parser)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    struct parser_t *parser;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, struct parser_t *parser)
#else
static void
yy_reduce_print (yyvsp, yyrule, parser)
    YYSTYPE *yyvsp;
    int yyrule;
    struct parser_t *parser;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , parser);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, parser); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, struct parser_t *parser)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, parser)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    struct parser_t *parser;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (parser);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 138: /* top_compstmt */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3825 "parser.c"
        break;
      case 139: /* top_stmt */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3832 "parser.c"
        break;
      case 140: /* bodystmt */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3839 "parser.c"
        break;
      case 143: /* compstmt */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3846 "parser.c"
        break;
      case 144: /* stmts */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3853 "parser.c"
        break;
      case 145: /* stmt */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3860 "parser.c"
        break;
      case 148: /* expr */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3867 "parser.c"
        break;
      case 149: /* command_call */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3874 "parser.c"
        break;
      case 150: /* block_command */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3881 "parser.c"
        break;
      case 151: /* cmd_brace_block */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3888 "parser.c"
        break;
      case 152: /* command */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3895 "parser.c"
        break;
      case 153: /* mlhs */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3902 "parser.c"
        break;
      case 154: /* mlhs_inner */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3909 "parser.c"
        break;
      case 155: /* mlhs_basic */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3916 "parser.c"
        break;
      case 156: /* mlhs_item */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3923 "parser.c"
        break;
      case 157: /* mlhs_head */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3930 "parser.c"
        break;
      case 158: /* mlhs_post */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3937 "parser.c"
        break;
      case 159: /* mlhs_node */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3944 "parser.c"
        break;
      case 160: /* lhs */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3951 "parser.c"
        break;
      case 161: /* cname */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3958 "parser.c"
        break;
      case 162: /* cpath */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3965 "parser.c"
        break;
      case 163: /* fname */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3972 "parser.c"
        break;
      case 164: /* fsym */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3979 "parser.c"
        break;
      case 165: /* undef_list */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3986 "parser.c"
        break;
      case 169: /* arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 3993 "parser.c"
        break;
      case 170: /* aref_args */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4000 "parser.c"
        break;
      case 171: /* paren_args */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4007 "parser.c"
        break;
      case 172: /* opt_paren_args */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4014 "parser.c"
        break;
      case 173: /* opt_call_args */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4021 "parser.c"
        break;
      case 174: /* call_args */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4028 "parser.c"
        break;
      case 175: /* command_args */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4035 "parser.c"
        break;
      case 177: /* block_arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4042 "parser.c"
        break;
      case 178: /* opt_block_arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4049 "parser.c"
        break;
      case 179: /* args */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4056 "parser.c"
        break;
      case 180: /* mrhs */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4063 "parser.c"
        break;
      case 181: /* primary */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4070 "parser.c"
        break;
      case 196: /* if_tail */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4077 "parser.c"
        break;
      case 197: /* opt_else */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4084 "parser.c"
        break;
      case 198: /* for_var */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4091 "parser.c"
        break;
      case 199: /* f_marg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4098 "parser.c"
        break;
      case 200: /* f_marg_list */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4105 "parser.c"
        break;
      case 201: /* f_margs */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4112 "parser.c"
        break;
      case 202: /* block_args_tail */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4119 "parser.c"
        break;
      case 203: /* opt_block_args_tail */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4126 "parser.c"
        break;
      case 204: /* block_param */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4133 "parser.c"
        break;
      case 205: /* opt_block_param */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4140 "parser.c"
        break;
      case 206: /* block_param_def */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4147 "parser.c"
        break;
      case 207: /* opt_bv_decl */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4154 "parser.c"
        break;
      case 208: /* bv_decls */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4161 "parser.c"
        break;
      case 209: /* bvar */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4168 "parser.c"
        break;
      case 210: /* lambda */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4175 "parser.c"
        break;
      case 212: /* f_larglist */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4182 "parser.c"
        break;
      case 213: /* lambda_body */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4189 "parser.c"
        break;
      case 214: /* do_block */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4196 "parser.c"
        break;
      case 215: /* block_call */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4203 "parser.c"
        break;
      case 216: /* method_call */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4210 "parser.c"
        break;
      case 217: /* opt_brace_block */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4217 "parser.c"
        break;
      case 218: /* brace_block */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4224 "parser.c"
        break;
      case 219: /* case_body */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4231 "parser.c"
        break;
      case 220: /* cases */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4238 "parser.c"
        break;
      case 221: /* opt_rescue */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4245 "parser.c"
        break;
      case 222: /* rescue_arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4252 "parser.c"
        break;
      case 223: /* exc_list */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4259 "parser.c"
        break;
      case 224: /* exc_var */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4266 "parser.c"
        break;
      case 225: /* opt_ensure */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4273 "parser.c"
        break;
      case 226: /* literal */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4280 "parser.c"
        break;
      case 227: /* strings */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4287 "parser.c"
        break;
      case 228: /* string */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4294 "parser.c"
        break;
      case 229: /* string_contents */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4301 "parser.c"
        break;
      case 230: /* string_content */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4308 "parser.c"
        break;
      case 234: /* string_dvar */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4315 "parser.c"
        break;
      case 235: /* symbol */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4322 "parser.c"
        break;
      case 236: /* sym */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4329 "parser.c"
        break;
      case 237: /* numeric */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4336 "parser.c"
        break;
      case 238: /* simple_numeric */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4343 "parser.c"
        break;
      case 239: /* variable */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4350 "parser.c"
        break;
      case 240: /* other_vars */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4357 "parser.c"
        break;
      case 241: /* backref */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4364 "parser.c"
        break;
      case 242: /* superclass */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4371 "parser.c"
        break;
      case 244: /* f_arglist */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4378 "parser.c"
        break;
      case 245: /* args_tail */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4385 "parser.c"
        break;
      case 246: /* opt_args_tail */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4392 "parser.c"
        break;
      case 247: /* f_args */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4399 "parser.c"
        break;
      case 248: /* f_bad_arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4406 "parser.c"
        break;
      case 249: /* f_norm_arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4413 "parser.c"
        break;
      case 250: /* f_arg_item */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4420 "parser.c"
        break;
      case 251: /* f_arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4427 "parser.c"
        break;
      case 252: /* f_kw */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4434 "parser.c"
        break;
      case 253: /* f_block_kw */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4441 "parser.c"
        break;
      case 254: /* f_block_kwarg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4448 "parser.c"
        break;
      case 255: /* f_kwarg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4455 "parser.c"
        break;
      case 257: /* f_kwrest */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4462 "parser.c"
        break;
      case 258: /* f_opt */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4469 "parser.c"
        break;
      case 260: /* f_block_opt */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4476 "parser.c"
        break;
      case 261: /* f_block_optarg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4483 "parser.c"
        break;
      case 262: /* f_optarg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4490 "parser.c"
        break;
      case 264: /* f_rest_arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4497 "parser.c"
        break;
      case 266: /* f_block_arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4504 "parser.c"
        break;
      case 267: /* opt_f_block_arg */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4511 "parser.c"
        break;
      case 268: /* singleton */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4518 "parser.c"
        break;
      case 270: /* const */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4525 "parser.c"
        break;
      case 271: /* base */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4532 "parser.c"
        break;
      case 272: /* assoc_list */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4539 "parser.c"
        break;
      case 273: /* assocs */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4546 "parser.c"
        break;
      case 274: /* assoc */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4553 "parser.c"
        break;
      case 275: /* operation */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4560 "parser.c"
        break;
      case 276: /* operation2 */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4567 "parser.c"
        break;
      case 277: /* operation3 */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4574 "parser.c"
        break;
      case 278: /* label */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4581 "parser.c"
        break;
      case 279: /* super */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4588 "parser.c"
        break;
      case 288: /* none */
/* Line 1393 of yacc.c  */
#line 255 "parser.y"
        { free_ast(((*yyvaluep).n)); };
/* Line 1393 of yacc.c  */
#line 4595 "parser.c"
        break;

      default:
        break;
    }
}




/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (struct parser_t *parser)
#else
int
yyparse (parser)
    struct parser_t *parser;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

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

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

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
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
      yychar = YYLEX;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1787 of yacc.c  */
#line 284 "parser.y"
    { parser->ast = (yyvsp[(1) - (1)].n); (yyval.n) = 0; YYACCEPT; }
    break;

  case 3:
/* Line 1787 of yacc.c  */
#line 285 "parser.y"
    { (yyval.n) = 0; YYACCEPT; }
    break;

  case 6:
/* Line 1787 of yacc.c  */
#line 290 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 7:
/* Line 1787 of yacc.c  */
#line 294 "parser.y"
    {
        (yyval.num) = parser->line;
    }
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 297 "parser.y"
    {
        (yyval.num) = parser->column;
    }
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 301 "parser.y"
    {
        (yyval.n) = alloc_ensure(token_body, (yyvsp[(3) - (6)].n), (yyvsp[(4) - (6)].n), (yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n));
        pop_end(parser, (yyval.n)); /* Every bodystmt ends with tEND */
        (yyval.n)->pos.start_line = (yyvsp[(1) - (6)].num);
        (yyval.n)->pos.start_col = (yyvsp[(2) - (6)].num);
    }
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 309 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (2)].n); }
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 314 "parser.y"
    { (yyval.n) = ((yyvsp[(1) - (3)].n) == NULL) ? (yyvsp[(3) - (3)].n) : update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 315 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 318 "parser.y"
    { lex_state = EXPR_FNAME; }
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 319 "parser.y"
    {
        (yyval.n) = alloc_node(token_alias, (yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n));
    }
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 323 "parser.y"
    {
        /* Ugly as hell, but it works */
        struct node *l = alloc_node(token_object, NULL, NULL);
        l->flags = global;
        struct node *r = alloc_node(token_object, NULL, NULL);
        r->flags = global;
        pop_pos(parser, r);
        pop_pos(parser, l);
        pop_stack(parser, l);
        pop_stack(parser, r);
        (yyval.n) = alloc_node(token_alias, l, r);
    }
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 336 "parser.y"
    {
        yyerror(parser, "can't make alias for the number variables");
        (yyval.n) = 0;
    }
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 341 "parser.y"
    {
        (yyval.n) = alloc_node(token_undef, NULL, (yyvsp[(2) - (2)].n));;
    }
    break;

  case 20:
/* Line 1787 of yacc.c  */
#line 345 "parser.y"
    {
        (yyval.n) = alloc_cond(token_if, (yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), NULL);
    }
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 349 "parser.y"
    {
        (yyval.n) = alloc_cond(token_unless, (yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), NULL);
    }
    break;

  case 22:
/* Line 1787 of yacc.c  */
#line 353 "parser.y"
    {
        (yyval.n) = alloc_cond(token_while, (yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), NULL);
    }
    break;

  case 23:
/* Line 1787 of yacc.c  */
#line 357 "parser.y"
    {
        (yyval.n) = alloc_cond(token_until, (yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), NULL);
    }
    break;

  case 24:
/* Line 1787 of yacc.c  */
#line 361 "parser.y"
    {
        (yyval.n) = alloc_cond(token_rescue, (yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), NULL);
    }
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 365 "parser.y"
    {
        if (parser->in_def)
            yyerror(parser, "BEGIN in method");
    }
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 370 "parser.y"
    {
        (yyval.n) = alloc_node(token_up_begin, (yyvsp[(4) - (5)].n), NULL);
        discard_pos(); /* } */
        discard_pos(); /* { */
    }
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 376 "parser.y"
    {
        (yyval.n) = alloc_node(token_up_end, (yyvsp[(3) - (4)].n), NULL);
        discard_pos(); /* } */
        discard_pos(); /* { */
    }
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 381 "parser.y"
    { (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 382 "parser.y"
    { (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 383 "parser.y"
    { (yyval.n) = alloc_node(token_op_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 385 "parser.y"
    {
        struct node *aux = alloc_node(token_array_value, (yyvsp[(1) - (6)].n), (yyvsp[(3) - (6)].n));
        (yyval.n) = alloc_node(token_op_assign, aux, (yyvsp[(6) - (6)].n));
    }
    break;

  case 32:
/* Line 1787 of yacc.c  */
#line 390 "parser.y"
    {
        struct node *aux = alloc_node(token_object, (yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n));
        (yyval.n) = alloc_node(token_op_assign, aux, (yyvsp[(5) - (5)].n));
    }
    break;

  case 33:
/* Line 1787 of yacc.c  */
#line 395 "parser.y"
    {
        struct node *aux = alloc_node(token_object, (yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n));
        (yyval.n) = alloc_node(token_op_assign, aux, (yyvsp[(5) - (5)].n));
    }
    break;

  case 34:
/* Line 1787 of yacc.c  */
#line 400 "parser.y"
    {
        yyerror(parser, "constant re-assignment");
        (yyval.n) = NULL;
        DISPOSE3((yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n));
    }
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 406 "parser.y"
    {
        struct node *aux = alloc_node(token_object, (yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n));
        (yyval.n) = alloc_node(token_op_assign, aux, (yyvsp[(5) - (5)].n));
    }
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 410 "parser.y"
    { (yyval.n) = alloc_node(token_op_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 411 "parser.y"
    { (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 412 "parser.y"
    { (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 413 "parser.y"
    { (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 415 "parser.y"
    { (yyval.n) = alloc_node(token__end__, NULL, NULL); }
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 419 "parser.y"
    { (yyval.n) = alloc_node(token_kw_and, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));   }
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 420 "parser.y"
    { (yyval.n) = alloc_node(token_kw_or, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 421 "parser.y"
    { (yyval.n) = alloc_node(token_kw_not, (yyvsp[(3) - (3)].n), NULL); }
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 422 "parser.y"
    { (yyval.n) = alloc_node(token_not, (yyvsp[(2) - (2)].n), NULL);    }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 431 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
        (yyval.n) = alloc_node(token_method_call, aux, (yyvsp[(4) - (4)].n));
    }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 436 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
        (yyval.n) = alloc_node(token_method_call, aux, (yyvsp[(4) - (4)].n));
    }
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 443 "parser.y"
    {
        (yyval.n) = ALLOC_N(token_block, (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].n));
        pop_start(parser, (yyval.n));
    }
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 450 "parser.y"
    {
        (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 454 "parser.y"
    {
        (yyval.n) = alloc_cond(token_method_call, (yyvsp[(3) - (3)].n), (yyvsp[(1) - (3)].n), (yyvsp[(2) - (3)].n));
    }
    break;

  case 56:
/* Line 1787 of yacc.c  */
#line 458 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
        (yyval.n) = alloc_node(token_method_call, aux, (yyvsp[(4) - (4)].n));
    }
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 463 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n));
        (yyval.n) = alloc_cond(token_method_call, (yyvsp[(5) - (5)].n), aux, (yyvsp[(4) - (5)].n));
    }
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 468 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
        (yyval.n) = alloc_node(token_method_call, aux, (yyvsp[(4) - (4)].n));
    }
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 473 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n));
        (yyval.n) = alloc_cond(token_method_call, (yyvsp[(5) - (5)].n), aux, (yyvsp[(4) - (5)].n));
    }
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 477 "parser.y"
    { (yyval.n) = alloc_node(token_super, (yyvsp[(2) - (2)].n), NULL); }
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 478 "parser.y"
    { (yyval.n) = alloc_node(token_yield, (yyvsp[(2) - (2)].n), NULL);       }
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 479 "parser.y"
    { (yyval.n) = alloc_node(token_return, (yyvsp[(2) - (2)].n), NULL);      }
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 480 "parser.y"
    { (yyval.n) = alloc_node(token_break, (yyvsp[(2) - (2)].n), NULL);       }
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 481 "parser.y"
    { (yyval.n) = alloc_node(token_next, (yyvsp[(2) - (2)].n), NULL);        }
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 485 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 489 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 493 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 495 "parser.y"
    {
        (yyvsp[(3) - (3)].n)->flags = kwrest;
        (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
    }
    break;

  case 72:
/* Line 1787 of yacc.c  */
#line 500 "parser.y"
    {
        (yyvsp[(3) - (5)].n)->flags = kwrest;
        (yyval.n) = concat_list((yyvsp[(1) - (5)].n), update_list((yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n)));
    }
    break;

  case 73:
/* Line 1787 of yacc.c  */
#line 505 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, NULL, NULL);
        (yyval.n)->flags = star;
        (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyval.n));
    }
    break;

  case 74:
/* Line 1787 of yacc.c  */
#line 511 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, NULL, NULL);
        (yyval.n)->flags = star;
        (yyval.n) = update_list((yyvsp[(1) - (4)].n), (yyval.n));
        (yyval.n) = concat_list((yyval.n), (yyvsp[(4) - (4)].n));
    }
    break;

  case 75:
/* Line 1787 of yacc.c  */
#line 517 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); (yyval.n)->flags = kwrest; }
    break;

  case 76:
/* Line 1787 of yacc.c  */
#line 518 "parser.y"
    { (yyval.n) = update_list((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); (yyvsp[(2) - (4)].n)->flags = kwrest; }
    break;

  case 77:
/* Line 1787 of yacc.c  */
#line 520 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, NULL, NULL);
        (yyval.n)->flags = star;
    }
    break;

  case 78:
/* Line 1787 of yacc.c  */
#line 525 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, NULL, NULL);
        (yyval.n)->flags = star;
        (yyval.n) = update_list((yyval.n), (yyvsp[(3) - (3)].n));
    }
    break;

  case 80:
/* Line 1787 of yacc.c  */
#line 533 "parser.y"
    { (yyval.n) = alloc_node(token_object, (yyvsp[(2) - (3)].n), NULL); }
    break;

  case 81:
/* Line 1787 of yacc.c  */
#line 536 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (2)].n); }
    break;

  case 82:
/* Line 1787 of yacc.c  */
#line 537 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(2) - (3)].n)); }
    break;

  case 83:
/* Line 1787 of yacc.c  */
#line 540 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 84:
/* Line 1787 of yacc.c  */
#line 541 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 86:
/* Line 1787 of yacc.c  */
#line 546 "parser.y"
    {
        (yyval.n) = alloc_node(token_array_value, (yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
    }
    break;

  case 87:
/* Line 1787 of yacc.c  */
#line 549 "parser.y"
    { (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 88:
/* Line 1787 of yacc.c  */
#line 550 "parser.y"
    { (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 89:
/* Line 1787 of yacc.c  */
#line 551 "parser.y"
    { (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 90:
/* Line 1787 of yacc.c  */
#line 553 "parser.y"
    {
        if (parser->in_def)
            yyerror(parser, "dynamic constant assignment");
        (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
    }
    break;

  case 91:
/* Line 1787 of yacc.c  */
#line 559 "parser.y"
    {
        if (parser->in_def)
            yyerror(parser, "dynamic constant assignment");
        (yyval.n) = (yyvsp[(2) - (2)].n);
    }
    break;

  case 94:
/* Line 1787 of yacc.c  */
#line 569 "parser.y"
    {
        (yyval.n) = alloc_node(token_array_value, (yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
    }
    break;

  case 95:
/* Line 1787 of yacc.c  */
#line 572 "parser.y"
    { (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 96:
/* Line 1787 of yacc.c  */
#line 573 "parser.y"
    { (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 97:
/* Line 1787 of yacc.c  */
#line 574 "parser.y"
    { (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 98:
/* Line 1787 of yacc.c  */
#line 576 "parser.y"
    {
        if (parser->in_def)
            yyerror(parser, "dynamic constant assignment");
        (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
    }
    break;

  case 99:
/* Line 1787 of yacc.c  */
#line 582 "parser.y"
    {
        if (parser->in_def)
            yyerror(parser, "dynamic constant assignment");
        (yyval.n) = (yyvsp[(2) - (2)].n);
    }
    break;

  case 100:
/* Line 1787 of yacc.c  */
#line 590 "parser.y"
    {
      yyerror(parser, "class/module name must be CONSTANT");
      (yyval.n) = 0;
    }
    break;

  case 102:
/* Line 1787 of yacc.c  */
#line 597 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 103:
/* Line 1787 of yacc.c  */
#line 598 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 104:
/* Line 1787 of yacc.c  */
#line 599 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 107:
/* Line 1787 of yacc.c  */
#line 605 "parser.y"
    {
        lex_state = EXPR_ENDFN;
        (yyval.n) = alloc_node(token_object, NULL, NULL);
        (yyval.n)->name = parser->aux;
        (yyval.n)->pos.start_line = (yyval.n)->pos.end_line = parser->line;
        (yyval.n)->pos.end_col = parser->column;
        (yyval.n)->pos.start_col = (yyval.n)->pos.end_col - strlen(parser->aux); 
		
    }
    break;

  case 108:
/* Line 1787 of yacc.c  */
#line 615 "parser.y"
    {
        lex_state = EXPR_ENDFN;
        (yyval.n) = alloc_node(token_object, NULL, NULL);
    }
    break;

  case 112:
/* Line 1787 of yacc.c  */
#line 625 "parser.y"
    { lex_state = EXPR_FNAME; }
    break;

  case 113:
/* Line 1787 of yacc.c  */
#line 625 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 114:
/* Line 1787 of yacc.c  */
#line 628 "parser.y"
    { copy_op("|"); }
    break;

  case 115:
/* Line 1787 of yacc.c  */
#line 628 "parser.y"
    { copy_op("^"); }
    break;

  case 116:
/* Line 1787 of yacc.c  */
#line 628 "parser.y"
    { copy_op("&"); }
    break;

  case 117:
/* Line 1787 of yacc.c  */
#line 629 "parser.y"
    { copy_op("<=>"); }
    break;

  case 118:
/* Line 1787 of yacc.c  */
#line 629 "parser.y"
    { copy_op("=="); }
    break;

  case 119:
/* Line 1787 of yacc.c  */
#line 629 "parser.y"
    { copy_op("===");}
    break;

  case 120:
/* Line 1787 of yacc.c  */
#line 630 "parser.y"
    { copy_op("=~"); }
    break;

  case 121:
/* Line 1787 of yacc.c  */
#line 630 "parser.y"
    {copy_op("!~");}
    break;

  case 122:
/* Line 1787 of yacc.c  */
#line 630 "parser.y"
    { copy_op(">");}
    break;

  case 123:
/* Line 1787 of yacc.c  */
#line 631 "parser.y"
    { copy_op(">="); }
    break;

  case 124:
/* Line 1787 of yacc.c  */
#line 631 "parser.y"
    { copy_op("<"); }
    break;

  case 125:
/* Line 1787 of yacc.c  */
#line 631 "parser.y"
    { copy_op("<="); }
    break;

  case 126:
/* Line 1787 of yacc.c  */
#line 632 "parser.y"
    {copy_op("!=");}
    break;

  case 127:
/* Line 1787 of yacc.c  */
#line 632 "parser.y"
    {copy_op("<<");}
    break;

  case 128:
/* Line 1787 of yacc.c  */
#line 632 "parser.y"
    {copy_op(">>");}
    break;

  case 129:
/* Line 1787 of yacc.c  */
#line 633 "parser.y"
    { copy_op("+"); }
    break;

  case 130:
/* Line 1787 of yacc.c  */
#line 633 "parser.y"
    { copy_op("-"); }
    break;

  case 131:
/* Line 1787 of yacc.c  */
#line 633 "parser.y"
    { copy_op("*"); }
    break;

  case 132:
/* Line 1787 of yacc.c  */
#line 634 "parser.y"
    { copy_op("*"); }
    break;

  case 133:
/* Line 1787 of yacc.c  */
#line 634 "parser.y"
    { copy_op("/"); }
    break;

  case 134:
/* Line 1787 of yacc.c  */
#line 634 "parser.y"
    { copy_op("%"); }
    break;

  case 135:
/* Line 1787 of yacc.c  */
#line 635 "parser.y"
    { copy_op("**"); }
    break;

  case 136:
/* Line 1787 of yacc.c  */
#line 635 "parser.y"
    { copy_op("[]"); }
    break;

  case 137:
/* Line 1787 of yacc.c  */
#line 635 "parser.y"
    { copy_op("`");}
    break;

  case 138:
/* Line 1787 of yacc.c  */
#line 636 "parser.y"
    { copy_op("+"); }
    break;

  case 139:
/* Line 1787 of yacc.c  */
#line 636 "parser.y"
    { copy_op("[]="); }
    break;

  case 140:
/* Line 1787 of yacc.c  */
#line 637 "parser.y"
    { copy_op("-"); }
    break;

  case 141:
/* Line 1787 of yacc.c  */
#line 637 "parser.y"
    { copy_op("**"); }
    break;

  case 142:
/* Line 1787 of yacc.c  */
#line 638 "parser.y"
    { copy_op("!"); }
    break;

  case 143:
/* Line 1787 of yacc.c  */
#line 638 "parser.y"
    { copy_op("~"); }
    break;

  case 185:
/* Line 1787 of yacc.c  */
#line 648 "parser.y"
    { (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 186:
/* Line 1787 of yacc.c  */
#line 650 "parser.y"
    {
        struct node *aux = alloc_cond(token_rescue, (yyvsp[(5) - (5)].n), (yyvsp[(3) - (5)].n), NULL);
        (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (5)].n), aux);
    }
    break;

  case 187:
/* Line 1787 of yacc.c  */
#line 654 "parser.y"
    { (yyval.n) = alloc_node(token_op_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 188:
/* Line 1787 of yacc.c  */
#line 656 "parser.y"
    {
        struct node *aux = alloc_cond(token_rescue, (yyvsp[(5) - (5)].n), (yyvsp[(3) - (5)].n), NULL);
        (yyval.n) = alloc_node(token_op_assign, (yyvsp[(1) - (5)].n), aux);
    }
    break;

  case 189:
/* Line 1787 of yacc.c  */
#line 661 "parser.y"
    {
        struct node *aux = alloc_node(token_array_value, (yyvsp[(1) - (6)].n), (yyvsp[(3) - (6)].n));
        (yyval.n) = alloc_node(token_op_assign, aux, (yyvsp[(6) - (6)].n));
    }
    break;

  case 190:
/* Line 1787 of yacc.c  */
#line 666 "parser.y"
    {
        struct node *aux = alloc_node(token_object, (yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n));
        (yyval.n) = alloc_node(token_op_assign, aux, (yyvsp[(5) - (5)].n));
    }
    break;

  case 191:
/* Line 1787 of yacc.c  */
#line 671 "parser.y"
    {
        struct node *aux = alloc_node(token_object, (yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n));
        (yyval.n) = alloc_node(token_op_assign, aux, (yyvsp[(5) - (5)].n));
    }
    break;

  case 192:
/* Line 1787 of yacc.c  */
#line 676 "parser.y"
    {
        struct node *aux = alloc_node(token_object, (yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n));
        (yyval.n) = alloc_node(token_op_assign, aux, (yyvsp[(5) - (5)].n));
    }
    break;

  case 193:
/* Line 1787 of yacc.c  */
#line 681 "parser.y"
    {
        yyerror(parser, "constant re-assignment");
        (yyval.n) = NULL;
        DISPOSE3((yyvsp[(1) - (5)].n), (yyvsp[(3) - (5)].n), (yyvsp[(5) - (5)].n));
    }
    break;

  case 194:
/* Line 1787 of yacc.c  */
#line 687 "parser.y"
    {
        yyerror(parser, "constant re-assignment");
        (yyval.n) = NULL;
        DISPOSE2((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n));
    }
    break;

  case 195:
/* Line 1787 of yacc.c  */
#line 692 "parser.y"
    { (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 196:
/* Line 1787 of yacc.c  */
#line 693 "parser.y"
    { (yyval.n) = alloc_node(token_dot2, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 197:
/* Line 1787 of yacc.c  */
#line 694 "parser.y"
    { (yyval.n) = alloc_node(token_dot3, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));}
    break;

  case 198:
/* Line 1787 of yacc.c  */
#line 695 "parser.y"
    { (yyval.n) = alloc_node(token_plus, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 199:
/* Line 1787 of yacc.c  */
#line 696 "parser.y"
    { (yyval.n) = alloc_node(token_minus, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));}
    break;

  case 200:
/* Line 1787 of yacc.c  */
#line 697 "parser.y"
    { (yyval.n) = alloc_node(token_mul, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));}
    break;

  case 201:
/* Line 1787 of yacc.c  */
#line 698 "parser.y"
    { (yyval.n) = alloc_node(token_div, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));}
    break;

  case 202:
/* Line 1787 of yacc.c  */
#line 699 "parser.y"
    { (yyval.n) = alloc_node(token_mod, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));}
    break;

  case 203:
/* Line 1787 of yacc.c  */
#line 700 "parser.y"
    { (yyval.n) = alloc_node(token_pow, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));}
    break;

  case 204:
/* Line 1787 of yacc.c  */
#line 702 "parser.y"
    {
        struct node *aux = alloc_node(token_pow, (yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n));
        (yyval.n) = alloc_node(token_unary_minus, aux, NULL);
    }
    break;

  case 205:
/* Line 1787 of yacc.c  */
#line 706 "parser.y"
    { (yyval.n) = alloc_node(token_unary_plus, (yyvsp[(2) - (2)].n), NULL);    }
    break;

  case 206:
/* Line 1787 of yacc.c  */
#line 707 "parser.y"
    { (yyval.n) = alloc_node(token_unary_minus, (yyvsp[(2) - (2)].n), NULL); }
    break;

  case 207:
/* Line 1787 of yacc.c  */
#line 708 "parser.y"
    { (yyval.n) = alloc_node(token_bit_or, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 208:
/* Line 1787 of yacc.c  */
#line 709 "parser.y"
    { (yyval.n) = alloc_node(token_bit_xor, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 209:
/* Line 1787 of yacc.c  */
#line 710 "parser.y"
    { (yyval.n) = alloc_node(token_bit_and, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 210:
/* Line 1787 of yacc.c  */
#line 711 "parser.y"
    { (yyval.n) = alloc_node(token_cmp, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 211:
/* Line 1787 of yacc.c  */
#line 712 "parser.y"
    { (yyval.n) = alloc_node(token_greater, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 212:
/* Line 1787 of yacc.c  */
#line 713 "parser.y"
    { (yyval.n) = alloc_node(token_geq, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 213:
/* Line 1787 of yacc.c  */
#line 714 "parser.y"
    { (yyval.n) = alloc_node(token_lesser, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 214:
/* Line 1787 of yacc.c  */
#line 715 "parser.y"
    { (yyval.n) = alloc_node(token_leq, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 215:
/* Line 1787 of yacc.c  */
#line 716 "parser.y"
    { (yyval.n) = alloc_node(token_eq, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 216:
/* Line 1787 of yacc.c  */
#line 717 "parser.y"
    { (yyval.n) = alloc_node(token_eqq, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 217:
/* Line 1787 of yacc.c  */
#line 718 "parser.y"
    { (yyval.n) = alloc_node(token_neq, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 218:
/* Line 1787 of yacc.c  */
#line 719 "parser.y"
    { (yyval.n) = alloc_node(token_match, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 219:
/* Line 1787 of yacc.c  */
#line 720 "parser.y"
    { (yyval.n) = alloc_node(token_nmatch, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));    }
    break;

  case 220:
/* Line 1787 of yacc.c  */
#line 721 "parser.y"
    { (yyval.n) = alloc_node(token_not, (yyvsp[(2) - (2)].n), NULL);    }
    break;

  case 221:
/* Line 1787 of yacc.c  */
#line 722 "parser.y"
    { (yyval.n) = alloc_node(token_neg, (yyvsp[(2) - (2)].n), NULL);    }
    break;

  case 222:
/* Line 1787 of yacc.c  */
#line 723 "parser.y"
    { (yyval.n) = alloc_node(token_lshift, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 223:
/* Line 1787 of yacc.c  */
#line 724 "parser.y"
    { (yyval.n) = alloc_node(token_rshift, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 224:
/* Line 1787 of yacc.c  */
#line 725 "parser.y"
    { (yyval.n) = alloc_node(token_and, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));  }
    break;

  case 225:
/* Line 1787 of yacc.c  */
#line 726 "parser.y"
    { (yyval.n) = alloc_node(token_or, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 226:
/* Line 1787 of yacc.c  */
#line 727 "parser.y"
    { (yyval.n) = alloc_node(token_defined, (yyvsp[(3) - (3)].n), NULL); }
    break;

  case 227:
/* Line 1787 of yacc.c  */
#line 729 "parser.y"
    {
        (yyval.n) = alloc_cond(token_ternary, (yyvsp[(1) - (6)].n), (yyvsp[(3) - (6)].n), (yyvsp[(6) - (6)].n));
    }
    break;

  case 230:
/* Line 1787 of yacc.c  */
#line 736 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (2)].n); }
    break;

  case 231:
/* Line 1787 of yacc.c  */
#line 737 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n)); }
    break;

  case 232:
/* Line 1787 of yacc.c  */
#line 738 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (2)].n); }
    break;

  case 233:
/* Line 1787 of yacc.c  */
#line 741 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 239:
/* Line 1787 of yacc.c  */
#line 751 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 240:
/* Line 1787 of yacc.c  */
#line 753 "parser.y"
    {
        struct node *aux = alloc_node(token_hash, (yyvsp[(1) - (2)].n), NULL);
        (yyval.n) = update_list(aux, (yyvsp[(2) - (2)].n));
    }
    break;

  case 241:
/* Line 1787 of yacc.c  */
#line 758 "parser.y"
    {
        struct node *aux = alloc_node(token_hash, (yyvsp[(3) - (4)].n), NULL);
        struct node *n = update_list(aux, (yyvsp[(4) - (4)].n));
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), n);
    }
    break;

  case 243:
/* Line 1787 of yacc.c  */
#line 767 "parser.y"
    {
        (yyval.num) = parser->cmdarg_stack;
        CMDARG_PUSH(1);
    }
    break;

  case 244:
/* Line 1787 of yacc.c  */
#line 771 "parser.y"
    {
        parser->cmdarg_stack = (yyval.num);
        (yyval.n) = (yyvsp[(2) - (2)].n);
    }
    break;

  case 245:
/* Line 1787 of yacc.c  */
#line 777 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 246:
/* Line 1787 of yacc.c  */
#line 780 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 247:
/* Line 1787 of yacc.c  */
#line 781 "parser.y"
    { (yyval.n) = NULL; }
    break;

  case 250:
/* Line 1787 of yacc.c  */
#line 786 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 251:
/* Line 1787 of yacc.c  */
#line 787 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 252:
/* Line 1787 of yacc.c  */
#line 788 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 253:
/* Line 1787 of yacc.c  */
#line 791 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 254:
/* Line 1787 of yacc.c  */
#line 792 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 255:
/* Line 1787 of yacc.c  */
#line 793 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 260:
/* Line 1787 of yacc.c  */
#line 800 "parser.y"
    { (yyval.n) = alloc_node(token_begin, (yyvsp[(2) - (3)].n), NULL); }
    break;

  case 261:
/* Line 1787 of yacc.c  */
#line 801 "parser.y"
    { lex_state = EXPR_ENDARG; }
    break;

  case 262:
/* Line 1787 of yacc.c  */
#line 801 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (4)].n); }
    break;

  case 263:
/* Line 1787 of yacc.c  */
#line 802 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 264:
/* Line 1787 of yacc.c  */
#line 804 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
        (yyval.n) = alloc_node(token_method_call, aux, NULL);
    }
    break;

  case 265:
/* Line 1787 of yacc.c  */
#line 808 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 266:
/* Line 1787 of yacc.c  */
#line 809 "parser.y"
    { (yyval.n) = alloc_node(token_array, NULL, NULL); }
    break;

  case 267:
/* Line 1787 of yacc.c  */
#line 810 "parser.y"
    { (yyval.n) = alloc_node(token_array, (yyvsp[(2) - (3)].n), NULL); }
    break;

  case 268:
/* Line 1787 of yacc.c  */
#line 812 "parser.y"
    {
        (yyval.n) = alloc_node(token_hash, (yyvsp[(2) - (3)].n), NULL);
        discard_pos();
    }
    break;

  case 269:
/* Line 1787 of yacc.c  */
#line 816 "parser.y"
    { (yyval.n) = alloc_node(token_return, NULL, NULL); }
    break;

  case 270:
/* Line 1787 of yacc.c  */
#line 817 "parser.y"
    { (yyval.n) = alloc_node(token_yield, (yyvsp[(3) - (4)].n), NULL); }
    break;

  case 271:
/* Line 1787 of yacc.c  */
#line 818 "parser.y"
    { (yyval.n) = alloc_node(token_yield, NULL, NULL); }
    break;

  case 272:
/* Line 1787 of yacc.c  */
#line 819 "parser.y"
    { (yyval.n) = alloc_node(token_yield, NULL, NULL); }
    break;

  case 273:
/* Line 1787 of yacc.c  */
#line 821 "parser.y"
    {
        (yyval.n) = alloc_node(token_defined, (yyvsp[(4) - (5)].n), NULL);
    }
    break;

  case 274:
/* Line 1787 of yacc.c  */
#line 824 "parser.y"
    { (yyval.n) = alloc_node(token_kw_not, (yyvsp[(3) - (4)].n), NULL); }
    break;

  case 275:
/* Line 1787 of yacc.c  */
#line 825 "parser.y"
    { (yyval.n) = alloc_node(token_kw_not, NULL, NULL); }
    break;

  case 276:
/* Line 1787 of yacc.c  */
#line 826 "parser.y"
    { (yyval.n) = alloc_cond(token_method_call, (yyvsp[(2) - (2)].n), (yyvsp[(1) - (2)].n), NULL); }
    break;

  case 277:
/* Line 1787 of yacc.c  */
#line 828 "parser.y"
    {
        (yyval.n) = (yyvsp[(1) - (2)].n);
        (yyval.n)->cond = (yyvsp[(2) - (2)].n);
    }
    break;

  case 278:
/* Line 1787 of yacc.c  */
#line 832 "parser.y"
    { (yyval.n) = alloc_cond(token_method_call, (yyvsp[(2) - (2)].n), NULL, NULL); }
    break;

  case 279:
/* Line 1787 of yacc.c  */
#line 834 "parser.y"
    {
        (yyval.n) = alloc_cond(token_if, (yyvsp[(2) - (6)].n), (yyvsp[(4) - (6)].n), (yyvsp[(5) - (6)].n));
        discard_pos(); /* tEND */
    }
    break;

  case 280:
/* Line 1787 of yacc.c  */
#line 839 "parser.y"
    {
        (yyval.n) = alloc_cond(token_unless, (yyvsp[(2) - (6)].n), (yyvsp[(4) - (6)].n), (yyvsp[(5) - (6)].n));
        discard_pos(); /* tEND */
    }
    break;

  case 281:
/* Line 1787 of yacc.c  */
#line 843 "parser.y"
    { COND_PUSH(1); }
    break;

  case 282:
/* Line 1787 of yacc.c  */
#line 843 "parser.y"
    { COND_POP(); }
    break;

  case 283:
/* Line 1787 of yacc.c  */
#line 844 "parser.y"
    {
        (yyval.n) = alloc_cond(token_while, (yyvsp[(3) - (7)].n), (yyvsp[(6) - (7)].n), NULL);
        discard_pos(); /* tEND */
    }
    break;

  case 284:
/* Line 1787 of yacc.c  */
#line 848 "parser.y"
    { COND_PUSH(1); }
    break;

  case 285:
/* Line 1787 of yacc.c  */
#line 848 "parser.y"
    { COND_POP(); }
    break;

  case 286:
/* Line 1787 of yacc.c  */
#line 849 "parser.y"
    {
        (yyval.n) = alloc_cond(token_while, (yyvsp[(3) - (7)].n), (yyvsp[(6) - (7)].n), NULL);
        discard_pos(); /* tEND */
    }
    break;

  case 287:
/* Line 1787 of yacc.c  */
#line 854 "parser.y"
    {
        (yyval.n) = alloc_cond(token_case, (yyvsp[(2) - (5)].n), (yyvsp[(4) - (5)].n), NULL);
        discard_pos(); /* tEND */
    }
    break;

  case 288:
/* Line 1787 of yacc.c  */
#line 859 "parser.y"
    {
        (yyval.n) = alloc_node(token_case, (yyvsp[(3) - (4)].n), NULL);
        discard_pos(); /* tEND */
    }
    break;

  case 289:
/* Line 1787 of yacc.c  */
#line 863 "parser.y"
    { COND_PUSH(1); }
    break;

  case 290:
/* Line 1787 of yacc.c  */
#line 863 "parser.y"
    { COND_POP(); }
    break;

  case 291:
/* Line 1787 of yacc.c  */
#line 864 "parser.y"
    {
        (yyval.n) = alloc_cond(token_for, (yyvsp[(5) - (9)].n), (yyvsp[(8) - (9)].n), (yyvsp[(2) - (9)].n));
        discard_pos(); /* tEND */
    }
    break;

  case 292:
/* Line 1787 of yacc.c  */
#line 869 "parser.y"
    {
        if (parser->in_def)
            yyerror(parser, "class definition in method body");
    }
    break;

  case 293:
/* Line 1787 of yacc.c  */
#line 874 "parser.y"
    {
        (yyval.n) = alloc_cond(token_class, (yyvsp[(3) - (6)].n), (yyvsp[(5) - (6)].n), (yyvsp[(2) - (6)].n));
        pop_comment(parser, (yyval.n));
    }
    break;

  case 294:
/* Line 1787 of yacc.c  */
#line 879 "parser.y"
    {
        (yyval.n) = alloc_node(token_singleton_class, (yyvsp[(6) - (7)].n), (yyvsp[(4) - (7)].n));
        pop_comment(parser, (yyval.n));
    }
    break;

  case 295:
/* Line 1787 of yacc.c  */
#line 884 "parser.y"
    {
        if (parser->in_def)
            yyerror(parser, "module definition in method body");
    }
    break;

  case 296:
/* Line 1787 of yacc.c  */
#line 889 "parser.y"
    {
        (yyval.n) = alloc_node(token_module, (yyvsp[(4) - (5)].n), (yyvsp[(2) - (5)].n));
        pop_comment(parser, (yyval.n));
    }
    break;

  case 297:
/* Line 1787 of yacc.c  */
#line 894 "parser.y"
    {
        parser->in_def++;
    }
    break;

  case 298:
/* Line 1787 of yacc.c  */
#line 898 "parser.y"
    {
        parser->in_def--;
        (yyval.n) = alloc_cond(token_function, (yyvsp[(2) - (6)].n), (yyvsp[(5) - (6)].n), (yyvsp[(4) - (6)].n));
        pop_comment(parser, (yyval.n));
    }
    break;

  case 299:
/* Line 1787 of yacc.c  */
#line 903 "parser.y"
    { lex_state = EXPR_FNAME; }
    break;

  case 300:
/* Line 1787 of yacc.c  */
#line 904 "parser.y"
    {
        lex_state = EXPR_ENDFN;
        parser->in_def++;
    }
    break;

  case 301:
/* Line 1787 of yacc.c  */
#line 909 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, (yyvsp[(2) - (9)].n), (yyvsp[(5) - (9)].n));
        (yyval.n) = alloc_cond(token_function, (yyval.n), (yyvsp[(8) - (9)].n), (yyvsp[(7) - (9)].n));
        (yyval.n)->flags = 1; /* Class method */
        pop_comment(parser, (yyval.n));
        parser->in_def--;
    }
    break;

  case 302:
/* Line 1787 of yacc.c  */
#line 916 "parser.y"
    { (yyval.n) = alloc_node(token_break, NULL, NULL);    }
    break;

  case 303:
/* Line 1787 of yacc.c  */
#line 917 "parser.y"
    { (yyval.n) = alloc_node(token_next, NULL, NULL);     }
    break;

  case 304:
/* Line 1787 of yacc.c  */
#line 918 "parser.y"
    { (yyval.n) = alloc_node(token_redo, NULL, NULL);     }
    break;

  case 305:
/* Line 1787 of yacc.c  */
#line 919 "parser.y"
    { (yyval.n) = alloc_node(token_retry, NULL, NULL);    }
    break;

  case 312:
/* Line 1787 of yacc.c  */
#line 932 "parser.y"
    {
        (yyval.n) = alloc_cond(token_if, (yyvsp[(2) - (5)].n), (yyvsp[(4) - (5)].n), (yyvsp[(5) - (5)].n));
    }
    break;

  case 314:
/* Line 1787 of yacc.c  */
#line 938 "parser.y"
    { (yyval.n) = alloc_node(token_if, (yyvsp[(2) - (2)].n), NULL);  }
    break;

  case 317:
/* Line 1787 of yacc.c  */
#line 944 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 318:
/* Line 1787 of yacc.c  */
#line 945 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 320:
/* Line 1787 of yacc.c  */
#line 949 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 321:
/* Line 1787 of yacc.c  */
#line 952 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 322:
/* Line 1787 of yacc.c  */
#line 953 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 323:
/* Line 1787 of yacc.c  */
#line 955 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (6)].n), update_list((yyvsp[(4) - (6)].n), (yyvsp[(6) - (6)].n)));
    }
    break;

  case 324:
/* Line 1787 of yacc.c  */
#line 959 "parser.y"
    {
        struct node *n = alloc_node(token_object, NULL, NULL);
        (yyval.n) = update_list((yyvsp[(1) - (3)].n), n);
    }
    break;

  case 325:
/* Line 1787 of yacc.c  */
#line 964 "parser.y"
    {
        struct node *n = alloc_node(token_object, NULL, NULL);
        (yyval.n) = concat_list((yyvsp[(1) - (5)].n), update_list(n, (yyvsp[(5) - (5)].n)));
    }
    break;

  case 326:
/* Line 1787 of yacc.c  */
#line 968 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 327:
/* Line 1787 of yacc.c  */
#line 969 "parser.y"
    { (yyval.n) = update_list((yyvsp[(2) - (4)].n), (yyvsp[(4) - (4)].n)); }
    break;

  case 328:
/* Line 1787 of yacc.c  */
#line 970 "parser.y"
    { (yyval.n) = alloc_node(token_object, NULL, NULL); }
    break;

  case 329:
/* Line 1787 of yacc.c  */
#line 972 "parser.y"
    {
        struct node *n = alloc_node(token_object, NULL, NULL);
        (yyval.n) = update_list(n, (yyvsp[(3) - (3)].n));
    }
    break;

  case 330:
/* Line 1787 of yacc.c  */
#line 979 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), update_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 331:
/* Line 1787 of yacc.c  */
#line 983 "parser.y"
    {
        (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 332:
/* Line 1787 of yacc.c  */
#line 987 "parser.y"
    {
        (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 333:
/* Line 1787 of yacc.c  */
#line 990 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 334:
/* Line 1787 of yacc.c  */
#line 993 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 335:
/* Line 1787 of yacc.c  */
#line 994 "parser.y"
    { (yyval.n) = 0; }
    break;

  case 336:
/* Line 1787 of yacc.c  */
#line 998 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (6)].n), concat_list((yyvsp[(3) - (6)].n), update_list((yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n))));
    }
    break;

  case 337:
/* Line 1787 of yacc.c  */
#line 1002 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (8)].n), concat_list((yyvsp[(3) - (8)].n), create_list((yyvsp[(5) - (8)].n), update_list((yyvsp[(7) - (8)].n), (yyvsp[(8) - (8)].n)))));
    }
    break;

  case 338:
/* Line 1787 of yacc.c  */
#line 1006 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), update_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 339:
/* Line 1787 of yacc.c  */
#line 1010 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (6)].n), concat_list((yyvsp[(3) - (6)].n), update_list((yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n))));
    }
    break;

  case 340:
/* Line 1787 of yacc.c  */
#line 1014 "parser.y"
    {
        (yyval.n) = update_list((yyvsp[(1) - (4)].n), update_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 341:
/* Line 1787 of yacc.c  */
#line 1017 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (2)].n); }
    break;

  case 342:
/* Line 1787 of yacc.c  */
#line 1019 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (6)].n), concat_list((yyvsp[(3) - (6)].n), update_list((yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n))));
    }
    break;

  case 343:
/* Line 1787 of yacc.c  */
#line 1022 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 344:
/* Line 1787 of yacc.c  */
#line 1024 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), update_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 345:
/* Line 1787 of yacc.c  */
#line 1028 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (6)].n), create_list((yyvsp[(3) - (6)].n), update_list((yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n))));
    }
    break;

  case 346:
/* Line 1787 of yacc.c  */
#line 1031 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 347:
/* Line 1787 of yacc.c  */
#line 1033 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), update_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 348:
/* Line 1787 of yacc.c  */
#line 1036 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 349:
/* Line 1787 of yacc.c  */
#line 1038 "parser.y"
    {
        (yyval.n) = create_list((yyvsp[(1) - (4)].n), update_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 352:
/* Line 1787 of yacc.c  */
#line 1046 "parser.y"
    {
      command_start = 1;
      (yyval.n) = (yyvsp[(1) - (1)].n);
    }
    break;

  case 353:
/* Line 1787 of yacc.c  */
#line 1052 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n);      }
    break;

  case 354:
/* Line 1787 of yacc.c  */
#line 1053 "parser.y"
    { (yyval.n) = NULL;    }
    break;

  case 355:
/* Line 1787 of yacc.c  */
#line 1054 "parser.y"
    { (yyval.n) = update_list((yyvsp[(2) - (4)].n), (yyvsp[(3) - (4)].n)); }
    break;

  case 357:
/* Line 1787 of yacc.c  */
#line 1059 "parser.y"
    {
        if (parser->version < ruby19) {
            yywarning("Block local variables are only available in Ruby 1.9.x or higher.");
        }
        (yyval.n) = (yyvsp[(2) - (2)].n);
    }
    break;

  case 359:
/* Line 1787 of yacc.c  */
#line 1068 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 361:
/* Line 1787 of yacc.c  */
#line 1073 "parser.y"
    {
      (yyval.n) = NULL;
      free_ast((yyvsp[(1) - (1)].n));
    }
    break;

  case 362:
/* Line 1787 of yacc.c  */
#line 1080 "parser.y"
    {
        (yyval.num) = parser->lpar_beg;
        parser->lpar_beg = ++parser->paren_nest;
    }
    break;

  case 363:
/* Line 1787 of yacc.c  */
#line 1085 "parser.y"
    {
        parser->lpar_beg = (yyvsp[(1) - (3)].num);
        (yyval.n) = alloc_node(token_block, (yyvsp[(3) - (3)].n), (yyvsp[(2) - (3)].n));
    }
    break;

  case 364:
/* Line 1787 of yacc.c  */
#line 1091 "parser.y"
    { (yyval.n) = update_list((yyvsp[(2) - (4)].n), (yyvsp[(3) - (4)].n)); }
    break;

  case 366:
/* Line 1787 of yacc.c  */
#line 1096 "parser.y"
    {
        (yyval.n) = (yyvsp[(2) - (3)].n);
        discard_pos(); /* } */
        discard_pos(); /* { */
    }
    break;

  case 367:
/* Line 1787 of yacc.c  */
#line 1102 "parser.y"
    {
        (yyval.n) = (yyvsp[(2) - (3)].n);
        discard_pos(); /* end */
    }
    break;

  case 368:
/* Line 1787 of yacc.c  */
#line 1109 "parser.y"
    {
        (yyval.n) = ALLOC_N(token_block, (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].n));
        pop_start(parser, (yyval.n));
    }
    break;

  case 369:
/* Line 1787 of yacc.c  */
#line 1115 "parser.y"
    { (yyvsp[(1) - (2)].n)->cond = (yyvsp[(2) - (2)].n); (yyval.n) = (yyvsp[(1) - (2)].n); }
    break;

  case 370:
/* Line 1787 of yacc.c  */
#line 1117 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
        (yyval.n) = update_list(aux, (yyvsp[(4) - (4)].n));
    }
    break;

  case 371:
/* Line 1787 of yacc.c  */
#line 1122 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
        (yyval.n) = update_list(aux, (yyvsp[(4) - (4)].n));
    }
    break;

  case 372:
/* Line 1787 of yacc.c  */
#line 1129 "parser.y"
    {
        (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 373:
/* Line 1787 of yacc.c  */
#line 1133 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
        (yyval.n) = alloc_node(token_method_call, aux, (yyvsp[(4) - (4)].n));
    }
    break;

  case 374:
/* Line 1787 of yacc.c  */
#line 1138 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
        (yyval.n) = alloc_node(token_method_call, aux, (yyvsp[(4) - (4)].n));
    }
    break;

  case 375:
/* Line 1787 of yacc.c  */
#line 1143 "parser.y"
    {
        struct node *aux = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
        (yyval.n) = alloc_node(token_method_call, aux, NULL);
    }
    break;

  case 376:
/* Line 1787 of yacc.c  */
#line 1148 "parser.y"
    {
        (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
    }
    break;

  case 377:
/* Line 1787 of yacc.c  */
#line 1152 "parser.y"
    {
        (yyval.n) = alloc_node(token_method_call, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
    }
    break;

  case 378:
/* Line 1787 of yacc.c  */
#line 1155 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (2)].n); (yyval.n)->r = (yyvsp[(2) - (2)].n); }
    break;

  case 380:
/* Line 1787 of yacc.c  */
#line 1158 "parser.y"
    {
        (yyval.n) = alloc_node(token_array_value, (yyvsp[(1) - (4)].n), (yyvsp[(3) - (4)].n));
    }
    break;

  case 383:
/* Line 1787 of yacc.c  */
#line 1168 "parser.y"
    {
        (yyval.n) = ALLOC_N(token_block, (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].n));
        pop_start(parser, (yyval.n));
    }
    break;

  case 384:
/* Line 1787 of yacc.c  */
#line 1173 "parser.y"
    {
        (yyval.n) = ALLOC_N(token_block, (yyvsp[(3) - (4)].n), (yyvsp[(2) - (4)].n));
        pop_start(parser, (yyval.n));
    }
    break;

  case 385:
/* Line 1787 of yacc.c  */
#line 1180 "parser.y"
    {
        (yyval.n) = alloc_cond(token_when, (yyvsp[(2) - (5)].n), (yyvsp[(4) - (5)].n), (yyvsp[(5) - (5)].n));
    }
    break;

  case 388:
/* Line 1787 of yacc.c  */
#line 1189 "parser.y"
    {
         (yyval.n) = alloc_node(token_rescue, (yyvsp[(2) - (5)].n), (yyvsp[(4) - (5)].n));
         (yyval.n)->ensure = (yyvsp[(5) - (5)].n);
    }
    break;

  case 390:
/* Line 1787 of yacc.c  */
#line 1197 "parser.y"
    {
        (yyval.n) = ((yyvsp[(1) - (2)].n) || (yyvsp[(2) - (2)].n)) ? alloc_node(token_rescue_arg, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)) : NULL;
    }
    break;

  case 395:
/* Line 1787 of yacc.c  */
#line 1205 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 397:
/* Line 1787 of yacc.c  */
#line 1209 "parser.y"
    { (yyval.n) = alloc_node(token_ensure, (yyvsp[(2) - (2)].n), NULL); }
    break;

  case 400:
/* Line 1787 of yacc.c  */
#line 1215 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 401:
/* Line 1787 of yacc.c  */
#line 1216 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n)); }
    break;

  case 402:
/* Line 1787 of yacc.c  */
#line 1220 "parser.y"
    {
        (yyval.n) = alloc_node(token_string, NULL, NULL);
    }
    break;

  case 403:
/* Line 1787 of yacc.c  */
#line 1224 "parser.y"
    {
        (yyval.n) = alloc_node(lex_strterm->token, (yyvsp[(2) - (3)].n), NULL);
        if (lex_strterm->word) {
            free(lex_strterm->word);
            lex_strterm->word = NULL;
        }
        free(lex_strterm);
        lex_strterm = NULL;
    }
    break;

  case 404:
/* Line 1787 of yacc.c  */
#line 1235 "parser.y"
    { (yyval.n) = 0; }
    break;

  case 405:
/* Line 1787 of yacc.c  */
#line 1237 "parser.y"
    {
        if ((yyvsp[(1) - (2)].n) != NULL)
            (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
        else
            (yyval.n) = (yyvsp[(2) - (2)].n);
    }
    break;

  case 406:
/* Line 1787 of yacc.c  */
#line 1245 "parser.y"
    { (yyval.n) = alloc_node(token_bare_string, NULL, NULL); (yyval.n)->value = strdup(parser->string_buf); printf("Ww\n"); printf("%s", parser->string_buf); memset(parser->string_buf, 0, sizeof(parser->string_buf));}
    break;

  case 407:
/* Line 1787 of yacc.c  */
#line 1247 "parser.y"
    {
        lex_state = EXPR_BEG;
        (yyval.num) = parser->cond_stack;
    }
    break;

  case 408:
/* Line 1787 of yacc.c  */
#line 1251 "parser.y"
    {
        (yyval.term) = lex_strterm;
        lex_strterm = NULL;
    }
    break;

  case 409:
/* Line 1787 of yacc.c  */
#line 1256 "parser.y"
    {
        parser->cond_stack = (yyvsp[(2) - (5)].num);
        lex_strterm = (yyvsp[(3) - (5)].term);
        (yyval.n) = (yyvsp[(4) - (5)].n);
        discard_pos(); /* } */
    }
    break;

  case 410:
/* Line 1787 of yacc.c  */
#line 1263 "parser.y"
    {
        (yyval.term) = lex_strterm;
        lex_strterm = NULL;
        lex_state = EXPR_BEG;
    }
    break;

  case 411:
/* Line 1787 of yacc.c  */
#line 1269 "parser.y"
    {
        lex_strterm = (yyvsp[(2) - (3)].term);
        (yyval.n) = (yyvsp[(3) - (3)].n);
    }
    break;

  case 413:
/* Line 1787 of yacc.c  */
#line 1276 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = global; POP_STACK; }
    break;

  case 414:
/* Line 1787 of yacc.c  */
#line 1277 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = ivar; POP_STACK; }
    break;

  case 415:
/* Line 1787 of yacc.c  */
#line 1278 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = cvar; POP_STACK; }
    break;

  case 416:
/* Line 1787 of yacc.c  */
#line 1282 "parser.y"
    {
        (yyval.n) = (yyvsp[(2) - (2)].n);
        (yyval.n)->kind = token_symbol;
        (yyval.n)->pos.start_col--;
    }
    break;

  case 419:
/* Line 1787 of yacc.c  */
#line 1291 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = global; POP_STACK; }
    break;

  case 420:
/* Line 1787 of yacc.c  */
#line 1292 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = ivar; POP_STACK; }
    break;

  case 421:
/* Line 1787 of yacc.c  */
#line 1293 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = cvar; POP_STACK; }
    break;

  case 423:
/* Line 1787 of yacc.c  */
#line 1298 "parser.y"
    {
        (yyval.n) = alloc_node(token_unary_minus, (yyvsp[(2) - (2)].n), NULL);
    }
    break;

  case 424:
/* Line 1787 of yacc.c  */
#line 1303 "parser.y"
    { (yyval.n) = alloc_node(token_numeric, NULL, NULL); (yyval.n)->flags = int_l; (yyval.n)->value = strdup(parser->string_val);}
    break;

  case 425:
/* Line 1787 of yacc.c  */
#line 1304 "parser.y"
    { (yyval.n) = alloc_node(token_numeric, NULL, NULL); (yyval.n)->flags = float_l; (yyval.n)->value = strdup(parser->string_val);}
    break;

  case 426:
/* Line 1787 of yacc.c  */
#line 1306 "parser.y"
    {
        if (parser->version < ruby21) {
            yywarning("Rational literals are only available in Ruby 2.1.x or higher.");
        }
        (yyval.n) = alloc_node(token_numeric, NULL, NULL); (yyval.n)->flags = rational_l;
		(yyval.n)->value = strdup(parser->string_val);
    }
    break;

  case 427:
/* Line 1787 of yacc.c  */
#line 1314 "parser.y"
    {
        if (parser->version < ruby21) {
            yywarning("Imaginary literals are only available in Ruby 2.1.x or higher.");
        }
        (yyval.n) = alloc_node(token_numeric, NULL, NULL); (yyval.n)->flags = imaginary_l;
		(yyval.n)->value = strdup(parser->string_val);
    }
    break;

  case 429:
/* Line 1787 of yacc.c  */
#line 1324 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = global; POP_STACK; }
    break;

  case 430:
/* Line 1787 of yacc.c  */
#line 1325 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = ivar; POP_STACK; }
    break;

  case 431:
/* Line 1787 of yacc.c  */
#line 1326 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = cvar; POP_STACK; }
    break;

  case 434:
/* Line 1787 of yacc.c  */
#line 1331 "parser.y"
    { (yyval.n) = alloc_node(token_nil, NULL, NULL);      }
    break;

  case 435:
/* Line 1787 of yacc.c  */
#line 1332 "parser.y"
    { (yyval.n) = alloc_node(token_self, NULL, NULL);     }
    break;

  case 436:
/* Line 1787 of yacc.c  */
#line 1333 "parser.y"
    { (yyval.n) = alloc_node(token_true, NULL, NULL);     }
    break;

  case 437:
/* Line 1787 of yacc.c  */
#line 1334 "parser.y"
    { (yyval.n) = alloc_node(token_false, NULL, NULL);    }
    break;

  case 438:
/* Line 1787 of yacc.c  */
#line 1335 "parser.y"
    { (yyval.n) = alloc_node(token_file, NULL, NULL);     }
    break;

  case 439:
/* Line 1787 of yacc.c  */
#line 1336 "parser.y"
    { (yyval.n) = alloc_node(token_line, NULL, NULL);     }
    break;

  case 440:
/* Line 1787 of yacc.c  */
#line 1337 "parser.y"
    { (yyval.n) = alloc_node(token_encoding, NULL, NULL); }
    break;

  case 441:
/* Line 1787 of yacc.c  */
#line 1340 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); POP_STACK; }
    break;

  case 442:
/* Line 1787 of yacc.c  */
#line 1343 "parser.y"
    { (yyval.n) = NULL; }
    break;

  case 443:
/* Line 1787 of yacc.c  */
#line 1345 "parser.y"
    {
        lex_state = EXPR_BEG;
        command_start = 1;
    }
    break;

  case 444:
/* Line 1787 of yacc.c  */
#line 1350 "parser.y"
    {
        (yyval.n) = (yyvsp[(3) - (4)].n);
    }
    break;

  case 445:
/* Line 1787 of yacc.c  */
#line 1353 "parser.y"
    { yyerrok; (yyval.n) = NULL; }
    break;

  case 446:
/* Line 1787 of yacc.c  */
#line 1357 "parser.y"
    {
        (yyval.n) = (yyvsp[(2) - (3)].n);
        lex_state = EXPR_BEG;
        command_start = 1;
    }
    break;

  case 447:
/* Line 1787 of yacc.c  */
#line 1363 "parser.y"
    {
        (yyval.n) = (yyvsp[(1) - (2)].n);
        lex_state = EXPR_BEG;
        command_start = 1;
    }
    break;

  case 448:
/* Line 1787 of yacc.c  */
#line 1371 "parser.y"
    {
        if (parser->version < ruby20) {
            yywarning("Keyword arguments are only available in Ruby 2.0.x or higher.");
        }
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), update_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 449:
/* Line 1787 of yacc.c  */
#line 1378 "parser.y"
    {
        if (parser->version < ruby20) {
            yywarning("Keyword arguments are only available in Ruby 2.0.x or higher.");
        }
        (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 450:
/* Line 1787 of yacc.c  */
#line 1385 "parser.y"
    {
        if (parser->version < ruby20) {
            yywarning("Keyword arguments are only available in Ruby 2.0.x or higher.");
        }
        (yyval.n) = update_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 451:
/* Line 1787 of yacc.c  */
#line 1392 "parser.y"
    {
        (yyval.n) = (yyvsp[(1) - (1)].n);
    }
    break;

  case 452:
/* Line 1787 of yacc.c  */
#line 1397 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 453:
/* Line 1787 of yacc.c  */
#line 1398 "parser.y"
    { (yyval.n) = 0;  }
    break;

  case 454:
/* Line 1787 of yacc.c  */
#line 1402 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (6)].n), concat_list((yyvsp[(3) - (6)].n), concat_list((yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n))));
    }
    break;

  case 455:
/* Line 1787 of yacc.c  */
#line 1406 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (8)].n), concat_list((yyvsp[(3) - (8)].n), create_list((yyvsp[(5) - (8)].n), concat_list((yyvsp[(7) - (8)].n), (yyvsp[(8) - (8)].n)))));
    }
    break;

  case 456:
/* Line 1787 of yacc.c  */
#line 1410 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), concat_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 457:
/* Line 1787 of yacc.c  */
#line 1414 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (6)].n), concat_list((yyvsp[(3) - (6)].n), concat_list((yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n))));
    }
    break;

  case 458:
/* Line 1787 of yacc.c  */
#line 1418 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), concat_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 459:
/* Line 1787 of yacc.c  */
#line 1422 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (6)].n), concat_list((yyvsp[(3) - (6)].n), concat_list((yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n))));
    }
    break;

  case 460:
/* Line 1787 of yacc.c  */
#line 1426 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 461:
/* Line 1787 of yacc.c  */
#line 1430 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), concat_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 462:
/* Line 1787 of yacc.c  */
#line 1434 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (6)].n), create_list((yyvsp[(3) - (6)].n), concat_list((yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n))));
    }
    break;

  case 463:
/* Line 1787 of yacc.c  */
#line 1438 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 464:
/* Line 1787 of yacc.c  */
#line 1442 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (4)].n), concat_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 465:
/* Line 1787 of yacc.c  */
#line 1446 "parser.y"
    {
        (yyval.n) = concat_list((yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 466:
/* Line 1787 of yacc.c  */
#line 1450 "parser.y"
    {
        (yyval.n) = create_list((yyvsp[(1) - (4)].n), concat_list((yyvsp[(3) - (4)].n), (yyvsp[(4) - (4)].n)));
    }
    break;

  case 469:
/* Line 1787 of yacc.c  */
#line 1457 "parser.y"
    { yyerror(parser, "formal argument cannot be a constant"); (yyval.n) = 0;             }
    break;

  case 470:
/* Line 1787 of yacc.c  */
#line 1458 "parser.y"
    { yyerror(parser, "formal argument cannot be an instance variable"); (yyval.n) = 0;   }
    break;

  case 471:
/* Line 1787 of yacc.c  */
#line 1459 "parser.y"
    { yyerror(parser, "formal argument cannot be a global variable"); (yyval.n) = 0;      }
    break;

  case 472:
/* Line 1787 of yacc.c  */
#line 1460 "parser.y"
    { yyerror(parser, "formal argument cannot be a class variable"); (yyval.n) = 0;       }
    break;

  case 476:
/* Line 1787 of yacc.c  */
#line 1467 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (3)].n); }
    break;

  case 478:
/* Line 1787 of yacc.c  */
#line 1471 "parser.y"
    { (yyval.n) = concat_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 479:
/* Line 1787 of yacc.c  */
#line 1475 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
        (yyval.n)->flags = label;
    }
    break;

  case 480:
/* Line 1787 of yacc.c  */
#line 1482 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
        (yyval.n)->flags = label;
    }
    break;

  case 481:
/* Line 1787 of yacc.c  */
#line 1488 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 482:
/* Line 1787 of yacc.c  */
#line 1489 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 483:
/* Line 1787 of yacc.c  */
#line 1492 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 484:
/* Line 1787 of yacc.c  */
#line 1493 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 487:
/* Line 1787 of yacc.c  */
#line 1500 "parser.y"
    {
        (yyval.n) = (yyvsp[(2) - (2)].n);
        (yyval.n)->flags = kwrest;
    }
    break;

  case 488:
/* Line 1787 of yacc.c  */
#line 1505 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, NULL, NULL);
        (yyval.n)->flags = kwrest;
    }
    break;

  case 489:
/* Line 1787 of yacc.c  */
#line 1512 "parser.y"
    {
        (yyval.num) = parser->column;
    }
    break;

  case 490:
/* Line 1787 of yacc.c  */
#line 1516 "parser.y"
    {
        (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (4)].n), (yyvsp[(4) - (4)].n));
        (yyvsp[(1) - (4)].n)->flags = opt; /* TODO: not sure about this */
        (yyvsp[(4) - (4)].n)->pos.start_col = (yyvsp[(3) - (4)].num);
        (yyvsp[(4) - (4)].n)->pos.end_col = parser->column;
        (yyvsp[(4) - (4)].n)->pos.offset = parser->lex_prev - parser->blob;
    }
    break;

  case 491:
/* Line 1787 of yacc.c  */
#line 1525 "parser.y"
    { (yyval.n) = alloc_node(token_assign, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 493:
/* Line 1787 of yacc.c  */
#line 1529 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 495:
/* Line 1787 of yacc.c  */
#line 1533 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 498:
/* Line 1787 of yacc.c  */
#line 1539 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); (yyval.n)->flags = kwrest; }
    break;

  case 499:
/* Line 1787 of yacc.c  */
#line 1540 "parser.y"
    { (yyval.n) = alloc_node(token_object, NULL, NULL); (yyval.n)->flags = kwrest; }
    break;

  case 502:
/* Line 1787 of yacc.c  */
#line 1546 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); (yyval.n)->flags = block; }
    break;

  case 503:
/* Line 1787 of yacc.c  */
#line 1549 "parser.y"
    { (yyval.n) = (yyvsp[(2) - (2)].n); }
    break;

  case 505:
/* Line 1787 of yacc.c  */
#line 1553 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (1)].n); }
    break;

  case 506:
/* Line 1787 of yacc.c  */
#line 1554 "parser.y"
    { lex_state = EXPR_BEG; }
    break;

  case 507:
/* Line 1787 of yacc.c  */
#line 1555 "parser.y"
    {
        if ((yyvsp[(3) - (4)].n) == 0)
            yyerror(parser, "can't define singleton method for ().");
        else {
            switch ((yyvsp[(3) - (4)].n)->kind) {
                case token_string:
                case token_regexp:
                case token_numeric:
                case token_symbol:
                case token_array:
                    yyerror(parser, "can't define singleton method for literals");
            }
        }
        (yyval.n) = (yyvsp[(3) - (4)].n);
    }
    break;

  case 508:
/* Line 1787 of yacc.c  */
#line 1572 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = constant; POP_STACK; }
    break;

  case 509:
/* Line 1787 of yacc.c  */
#line 1575 "parser.y"
    { (yyval.n) = ALLOC_N(token_object, NULL, NULL); (yyval.n)->flags = var; POP_STACK; }
    break;

  case 511:
/* Line 1787 of yacc.c  */
#line 1579 "parser.y"
    { (yyval.n) = (yyvsp[(1) - (2)].n); }
    break;

  case 513:
/* Line 1787 of yacc.c  */
#line 1583 "parser.y"
    { (yyval.n) = update_list((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n)); }
    break;

  case 514:
/* Line 1787 of yacc.c  */
#line 1587 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, (yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
    }
    break;

  case 515:
/* Line 1787 of yacc.c  */
#line 1591 "parser.y"
    {
        if (parser->version < ruby19) {
            yywarning("This syntax is only available in Ruby 1.9.x or higher.");
        }
        (yyval.n) = alloc_node(token_object, (yyvsp[(1) - (2)].n), (yyvsp[(2) - (2)].n));
    }
    break;

  case 516:
/* Line 1787 of yacc.c  */
#line 1598 "parser.y"
    {
        if (parser->version < ruby20) {
            yywarning("tDSTAR token is only available in Ruby 2.0.x or higher.");
        }
        (yyval.n) = (yyvsp[(2) - (2)].n);
    }
    break;

  case 521:
/* Line 1787 of yacc.c  */
#line 1612 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, NULL, NULL);
        (yyval.n)->name = parser->aux;
    }
    break;

  case 523:
/* Line 1787 of yacc.c  */
#line 1620 "parser.y"
    {
        (yyval.n) = alloc_node(token_object, NULL, NULL);
        (yyval.n)->name = parser->aux;
		
    }
    break;

  case 524:
/* Line 1787 of yacc.c  */
#line 1627 "parser.y"
    { (yyval.n) = ALLOC_N(token_symbol, NULL, NULL); POP_STACK; }
    break;

  case 525:
/* Line 1787 of yacc.c  */
#line 1630 "parser.y"
    { (yyval.n) = alloc_node(token_super, NULL, NULL); }
    break;

  case 536:
/* Line 1787 of yacc.c  */
#line 1651 "parser.y"
    {yyerrok;}
    break;

  case 539:
/* Line 1787 of yacc.c  */
#line 1654 "parser.y"
    {yyerrok;}
    break;

  case 540:
/* Line 1787 of yacc.c  */
#line 1657 "parser.y"
    { (yyval.n) = NULL; }
    break;


/* Line 1787 of yacc.c  */
#line 7757 "parser.c"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (parser, YY_("syntax error"));
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
        yyerror (parser, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



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
		      yytoken, &yylval, parser);
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, parser);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
  yyerror (parser, YY_("memory exhausted"));
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
                  yytoken, &yylval, parser);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, parser);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 1660 "parser.y"

#undef parser
#undef yylex

#include <ctype.h>
#include "hash.c"


/* Let's define some useful macros :D */

#define _unused_(c) (void) c;
#define multiline_comment(c) (*(c+1) == 'b' && *(c+2) == 'e' && *(c+3) == 'g' && *(c+4) == 'i' && *(c+5) == 'n')
#define multiline_end(c) (*c == '=' && *(c+1) == 'e' && *(c+2) == 'n' && *(c+3) == 'd')
#define not_sep(c) (is_valid_identifier(c) || is_utf8_digit(c) || *c == '_')
#define is_blank(c) (c == ' ' || c == '\t')
#define SWAP(a, b, aux) { aux = a; a = b; b = aux; }
#define is_special_method(buffer) ((strlen(buffer) > 4) && buffer[0] == '_' && \
                                                                buffer[1] == '_' && buffer[strlen(buffer) - 2] == '_' && \
                                                                buffer[strlen(buffer) - 1] == '_')
#define IS_EOF() ((unsigned int) (parser->lex_p - parser->blob) >= parser->length)
#define IS_ARG() IS_lex_state(EXPR_ARG_ANY)
#define IS_END() IS_lex_state(EXPR_END_ANY)
#define IS_BEG() IS_lex_state(EXPR_BEG_ANY)
#define IS_SPCARG(c) (IS_ARG() && space_seen && !isspace(c))
#define IS_LABEL_POSSIBLE() ((IS_lex_state(EXPR_BEG | EXPR_ENDFN) && !cmd_state) || IS_ARG())
#define IS_LABEL_SUFFIX() (*parser->lex_p == ':' && *(parser->lex_p + 1) != ':')
#define IS_AFTER_OPERATOR() IS_lex_state(EXPR_FNAME | EXPR_DOT)


/* Initialize the parser */
static void init_parser(struct parser_t *parser)
{
    parser->content_given = 0;
    parser->ast = NULL;
    parser->blob = NULL;
    parser->lex_p = NULL;
    parser->lex_prev = NULL;
    parser->lex_prevc = 0;
    parser->lex_pend = NULL;
    parser->line_pend = 0;
    parser->column_pend = 0;
    parser->here_found = 0;
    parser->eof_reached = 0;
    parser->cond_stack = 0;
    parser->cmdarg_stack = 0;
    parser->in_def = 0;
    parser->lpar_beg = 0;
    parser->paren_nest = 0;
    parser->sp = 0;
    parser->line = 1;
    parser->column = 0;
    parser->pos_stack = (struct pos_t *) malloc(SSIZE * sizeof(struct pos_t));
    parser->stack_scale = 0;
    parser->pos_size = 0;
    parser->errors = NULL;
    parser->last_error = NULL;
    parser->warning = 0;
    parser->unrecoverable = 0;
    parser->last_comment.comment = NULL;
    parser->last_comment.line = 0;
    parser->comment_index = 0;
    command_start = 1;
	parser->is_here_end = 0;
    lex_strterm = NULL;
    lex_state = EXPR_BEG;
}

/* Free the parser */
static void free_parser(struct parser_t *parser)
{
    int index;

    for (index = 0; index < parser->sp; index++)
        free(parser->stack[index]);
    if (parser->pos_stack != NULL)
        free(parser->pos_stack);
    if (lex_strterm && lex_strterm->word)
        free(lex_strterm->word);
    if (parser->last_comment.comment)
      free(parser->last_comment.comment);
    if (!parser->content_given)
        free(parser->blob);
}

/* Читаем текст файла и располагаем для последующего исследования. */
static int retrieve_source(struct parser_t *p, const char *path)
{
    int length;

    /* открываем указанный файл */
    FILE *fd = fopen(path, "r");
    if (!fd) {
        fprintf(stderr, "Cannot open file: %s\n", path);
        return 0; // неудача
    }

    fseek(fd, 0, SEEK_END);  // курсор в конец файла
    length = ftell(fd); // длина файла в байтах
    fseek(fd, 0, SEEK_SET);
	/* если файл пуст */
    if (!length)
        return 0;
    p->blob = (char *) malloc(sizeof(char) * length);  // выделили память

    if (!p->blob) {
        fprintf(stderr, "Cannot store contents\n"); // не смогли расположить контент в памяти
        return 0;
    }
    fread(p->blob, length, 1, fd); // прочитали весь файл как blob
    if (ferror(fd)) {
        fprintf(stderr, "Reading error\n");
        return 0;
    }
    p->length = length;
    p->lex_p = p->blob;
    fclose(fd);
    return 1;
}

/*
 * Макросы для упрощения работы с  UTF-8
 */
#define is_utf(c) ((c & 0xC0) != 0x80) // проверка, является ли UTF-8
#define is_special(c) (utf8_charsize(c) > 1) // не из ASCII
#define is_identchar(c) (is_utf8_alnum(c) || *c == '_')  // м.б. в идентификаторе

/*
 * This function is really simple. It steps over a char of
 * the string s, that is encoded in UTF-8. The result varies on the
 * number of bytes that encodes a single character following the UTF-8
 * rules. Therefore, this function will return 1 if the character
 * is in plain-ASCII, and greater than 1 otherwise.
 */
static int utf8_charsize(const char *s)
{
    int size = 0;
    int i = 0;

    do {
        i++;
        size++;
    } while (s[i] && !is_utf(s[i]));
    return size;
}

/* алфавитный */
static int is_utf8_alpha(const char *str)
{
    return is_special(str) ? 1 : isalpha(*str);
}

/* алфавитный или циферки [a-zA-Z0-9] */
static int is_utf8_alnum(const char *str)
{
    return is_special(str) ? 1 : isalnum(*str);
}

/* печатаемый */
static int is_utf8_graph(const char *str)
{
    return is_special(str) ? 1 : isgraph(*str);
}

//циферки
static int is_utf8_digit(const char *str)
{
    return is_special(str) ? 0 : isdigit(*str);
}

/* валидный идентификатор? */
static int is_valid_identifier(const char *c)
{
    if (is_utf8_alpha(c))
        return 1;
    else if (*c == '$' && is_utf8_graph(c + 1) && !is_utf8_digit(c + 1))
        return 1;
    else if ((*c == '_' || *c == '@') && is_utf8_alpha(c + 1))
        return 1;
    else if (*c == '@' && *(c + 1) == '@' && (is_utf8_alpha(c + 2) || *(c + 2) == '_'))
        return 1;
    return 0;
}

/* Получаем следующий символ и двигаем лексер вправо. */
static int parser_nextc(struct parser_t *parser)
{
    int c;

    if (parser->eof_reached || IS_EOF()) //если это последняя строка ):
        return -1;

    parser->lex_prev = parser->lex_p;
    parser->lex_prevc = parser->column; // помечаем посл. колонку
    c = (unsigned char) *parser->lex_p++;  // рассмотрим след. символ
    if (c == '\n') {
        if (parser->here_found) {
			//чтобы правильно обрабатать рубовский hereDoc
            parser->line = parser->line_pend;
            parser->column = parser->column_pend;
            parser->lex_p = parser->lex_pend + 1;
            parser->here_found = 0;
			//переместились за heredoc
        }
        parser->line++;
        parser->column = -1;
    }
    parser->column++; // к след. колонке
	parser->str_pos++;
    return c; // возвращаем символ
}
#define nextc() parser_nextc(parser) // делаем синоним

/*  Возвращаем лексер назад. */
static void parser_pushback(struct parser_t *parser)
{
    parser->column--;
    parser->lex_p--;
    if (*parser->lex_p == '\n') {
        parser->line--;
        parser->column = parser->lex_prevc;
    }
	parser->str_pos--;
}
#define pushback() parser_pushback(parser)

/* Парсит идентификатор heredoc и устанавливает новый lex_strterm */
static int parse_heredoc_identifier(struct parser_t *parser)
{
    char *buffer, *ptr;
    int count = SSIZE, scale = 0;
    char c = nextc();  // след. символ
    unsigned char quote_seen = 0, term = ' ';  //quote_seen ~ увидели кавычку
    unsigned char dash_seen = 0;  //dash_seen ~ увидели тире
	
	/*
		print <<"EOF";
		The price is #{$Price}.
		EOF
	*/

    /* проверяем случай <<-  */
    if (c == '-') {
        dash_seen = 1;
        c = nextc();
    }
    /* а теперь окружающие кавычки */
    if (c == '\'' || c == '"' || c == '`') {
        term = c; // запомнили как закрывающий
        c = nextc();
        quote_seen = 1;
    }
	// если после тире ни кавычка, ни идентификатор - все плохо
    if (!quote_seen && !is_identchar(parser->lex_prev)) {
        if (dash_seen)
            pushback();
        return 0;
    }

    buffer = (char *) malloc(SSIZE * sizeof(char));
    ptr = buffer;
    for (;;) {
        /* если была кавычка - идентификатор - запоминаем все, до закрывающей кавычки в переменной term */
        if (quote_seen) {
            if (c == term || !is_utf8_graph(parser->lex_prev))
                break;
        } else if (!is_identchar(parser->lex_prev))
            break;
        if (!count) {
            scale++;
            buffer = (char *) realloc(buffer, (SSIZE << scale) * sizeof(char));
        }
        *ptr++ = c;
        c = nextc();
        if (c < 0) {
            free(buffer);
            yyerror(parser, "unterminated here document identifier");  // конец идентификатора не наступил на строке
            return 0;
        }
    }
    *ptr = '\0';
    pushback();

	/* lex_strterm ~ parser->lex_strterm */
    lex_strterm = (struct term_t *) malloc(sizeof(struct term_t));
    lex_strterm->term = 1;
    lex_strterm->can_embed = dash_seen; // если есть тире в объявлении - могут быть отступы перед "end of here doc"
    lex_strterm->word = buffer; //идентификатор начала
    lex_strterm->token = token_heredoc;  // задаем тип токена
    lex_strterm->nestable = 0;
    lex_strterm->paren = 0;
    parser->lex_pend = parser->lex_p + quote_seen;  // позиция конца в буфере
    parser->line_pend = parser->line;  // позиция строки окончания просмотра
    parser->column_pend = parser->column; // позиция колонки
    return 1;
}

/* Парсим hereDoc  ;) */
static int parse_heredoc(struct parser_t *parser)
{
	// если в прошлый раз вернули строку вместо конца
	if(parser->is_here_end){
		pushback();
		parser->is_here_end = 0;
        return tSTRING_END; // конец блока
	}
	
	/* lex_strterm ~ parser->lex_strterm */
    int length = strlen(lex_strterm->word);
    char aux[length]; // выделяем буффер для ключевого слова here doc
    char c = nextc();
    int i = 0;
	int k=1;
    int ax = 0;
	char buf[500] = "\0";
	int len;
	size_t dest_size;
	

    /* пропускаем все что связано с объявлением  до след. линии */
    /*while (c != -1 && c != '\n'){
        c = nextc();
		printf("AAAAA%c", c);
	}
	*/
	
    do {
        c = nextc();
        /* в случае режима с дефисом - игнорируем вступительные пробелы */
        if (i == 0 && lex_strterm->can_embed)
            while (isspace(c) || c == '\n')
                c = nextc();
				
		len = strlen(buf);
		buf[len]=c;
		buf[len+1] = '\0';
		
        if (c == '#' && *(parser->lex_prev - 1) != '\\') {
            c = nextc();
            switch (c) {
				/* встречаем переменную -> передаем токен переменной */
                case '$': case '@':
					len = strlen(buf);
					buf[len-1]='\0';
					if(strlen(buf)>0){
						dest_size = sizeof (buf);
						memset(parser->string_buf, 0, sizeof(parser->string_buf));
						strncpy(parser->string_buf, buf, dest_size );
						parser->string_buf[dest_size - 1] = '\0';
						pushback(); pushback(); pushback();
						return tSTRING_CONTENT;
					}
                    parser->column -= ax;
                    pushback();
                    return tSTRING_DVAR;
                case '{':
					len = strlen(buf);
					buf[len-1]='\0';
					if(strlen(buf)>0){
						dest_size = sizeof (buf);
						memset(parser->string_buf, 0, sizeof(parser->string_buf));
						strncpy(parser->string_buf, buf, dest_size );
						parser->string_buf[dest_size - 1] = '\0';
						pushback(); pushback(); pushback();
						return tSTRING_CONTENT;
					}
                    parser->column -= ax;
                    command_start = 1;  // command_start ~ parser->parser_command_start
                    return tSTRING_DBEG;  // возвращаем токен начала блока внутри строки
				default:
					len = strlen(buf);
					buf[len] = c;
					buf[len+1] = '\0';
            }
        }
        aux[i] = c;
		//если достигнут конец here doc (lex_strterm->word)
        if (c == '\n') {
            if ((length == i) && !strncmp(lex_strterm->word, aux, i)) {
				len = strlen(buf);
				k = 1;
				while(k <= length + 2){
					buf[len-k]='\0';
					k++;
				}
				if(strlen(buf)>0){
					dest_size = sizeof (buf);
					memset(parser->string_buf, 0, sizeof(parser->string_buf));
					strncpy(parser->string_buf, buf, dest_size );
					parser->string_buf[dest_size - 1] = '\0';
					parser->is_here_end = 1;
					return tSTRING_CONTENT;
				}
				pushback();
				return tSTRING_END;

            }
            i = -1;
        } else
            ax += utf8_charsize(parser->lex_prev) - 1;
        if (i >= length)
            i = -1;
        i++;
    } while (c != -1);  // пока не достигнем конца файла

    parser->eof_reached = 1;  // конец достигнут
    if (lex_strterm->word) {
        free(lex_strterm->word);
        lex_strterm->word = NULL;
    }
    free(lex_strterm);  // освобождаем все, что связано со встроенными строками/heredoc
    lex_strterm = NULL;
    return token_invalid;  // парсерная ошибка
}

/* находим пару для  c */
static char closing_char(char c)
{
    switch (c) {
        case '[': return ']';
        case '(': return ')';
        case '<': return '>';
        case '{': return '}';
        default: return c;
    }
}

/* сокращения типа %w(foo bar) ~ ["foo", "bar"] */
static int guess_kind(struct parser_t *parser, char c)
{
    if (!isalpha(c))
        return token_string;

    switch (c) {
        case 'Q': case 'q': case 'x': return token_string;  // ~ "string", 'string'...
        case 'I': case 'i':
            if (parser->version < ruby20) {
                yywarning("This shortcut is only available in Ruby 2.0.x or higher.");
            }
        case 'W': case 'w': return token_array; //массивы
        case 's': return token_symbol;
        case 'r': return token_regexp;
        default:
            yyerror(parser, "unknown type of %string");
            return 0;
    }
}

/* Push name to the stack */
static void push_stack(struct parser_t *parser, const char *buf)
{
    parser->stack[parser->sp] = strdup(buf);
    parser->sp++;
}

/* Pop name from the stack. */
static void pop_stack(struct parser_t *parser, struct node *n)
{
    if (n != NULL)
        n->name = parser->stack[0];
    parser->stack[0] = parser->stack[1];
    parser->stack[1] = NULL;
    parser->sp--;
}

/* Push a position into the stack of positions */
static void push_pos(struct parser_t *parser, struct pos_t tokp)
{
    int scale = SSIZE * parser->stack_scale;

    parser->pos_size++;
    if (parser->pos_size > SSIZE) {
        parser->pos_size = 1;
        parser->stack_scale++;
        scale += SSIZE;
        parser->pos_stack = (struct pos_t *) realloc(parser->pos_stack, scale * sizeof(struct pos_t));
    }
    parser->pos_stack[parser->pos_size + scale - 1] = tokp;
}

/* Pop a position from the stack of positions and assign to the given node */
static void pop_pos(struct parser_t *parser, struct node *n)
{
    int scale = SSIZE * parser->stack_scale;
    int pos = parser->pos_size - 1 + scale;
    struct pos_t tokp = parser->pos_stack[pos];

    if (n != NULL) {
        n->pos.start_line = tokp.start_line;
        n->pos.start_col = tokp.start_col;
        n->pos.end_line = tokp.end_line;
        n->pos.end_col = tokp.end_col;
        n->pos.offset = tokp.offset;
    }
    parser->pos_size--;
    if (parser->pos_size == 0 && parser->stack_scale > 0) {
        parser->stack_scale--;
        parser->pos_size = SSIZE;
        scale -= SSIZE;
        parser->pos_stack = (struct pos_t *) realloc(parser->pos_stack, scale * sizeof(struct pos_t));
    }
}

/* Like pop_pos but it just copies the start position to the given node */
static void pop_start(struct parser_t *parser, struct node *n)
{
    n->pos.start_line = parser->pos_stack[parser->pos_size - 1].start_line;
    n->pos.start_col = parser->pos_stack[parser->pos_size - 1].start_col;
    pop_pos(parser, NULL);
}

/* Like pop_pos but it just copies the end position to the given node */
static void pop_end(struct parser_t *parser, struct node *n)
{
    n->pos.end_line = parser->pos_stack[parser->pos_size - 1].start_line;
    n->pos.end_col = parser->pos_stack[parser->pos_size - 1].start_col;
    pop_pos(parser, NULL);
}

/* Push the last comment that we've found to the stack of comments. */
static void push_last_comment(struct parser_t *parser)
{
    if ((parser->line - parser->last_comment.line) < 2)
        parser->comment_stack[parser->comment_index] = parser->last_comment.comment;
    else {
        parser->comment_stack[parser->comment_index] = NULL;
        if (parser->last_comment.comment)
            free(parser->last_comment.comment);
    }
    parser->comment_index++;
    parser->last_comment.comment = NULL;
}

/* Pop a comment from the stack of comments and assign it to the given node */
static void pop_comment(struct parser_t *parser, struct node *n)
{
    if (parser->comment_index > 0) {
        parser->comment_index--;
        n->comment = parser->comment_stack[parser->comment_index];
    }
}

#define __check_buffer_size(N) { \
  if (count > N) { \
    count = 0; \
    scale++; \
    buffer = (char *) realloc(buffer, scale * 1024); \
  } \
}

/* Store the given comment as the last comment seen */
static void store_comment(struct parser_t *parser, char *comment)
{
    if (parser->last_comment.comment != NULL)
        free(parser->last_comment.comment);
    parser->last_comment.comment = comment;
    parser->last_comment.line = parser->line;
}

/* Check if the given parameter points to an indented comment */
static int is_indented_comment(struct parser_t *parser)
{
    char *c = parser->lex_prev;
    char *original = c;

    for (; *c == ' ' || *c == '\t'; ++c);
    parser->lex_p += (c - original);
    parser->column += (c - original);
    return (*c == '#');
}

/* Read a comment and store it if possible */
static void set_comment(struct parser_t *parser)
{
    int c = ' ', count = 0, scale = 0;
    char *buffer = (char *) malloc(LSIZE);

    for (;; ++count) {
        if (c != '#' && !is_indented_comment(parser))
            break;
        c = *(parser->lex_p - 1);
        while (c == '#' && c != -1)
            c = nextc();
        if (c != '\n') {
            for (; c != -1; count++) {
                __check_buffer_size(1000);
                buffer[count] = c;
                c = nextc();
                if (c == '\n') {
                    buffer[++count] = c;
                    break;
                }
            }
        } else
            buffer[count] = c;
        c = nextc();
    }

    if (c != -1)
        pushback();
    buffer[count] = '\0';
    store_comment(parser, buffer);
}

/* Parse a string or a regexp */
static int parse_string(struct parser_t *parser)
{
	register int c;
	int c2;
	int next;
	char buf[500]="\0";
	int len;
	size_t dest_size;
	
	just_a_text:
	//printf("1%s", buf);
    c = *parser->lex_p;
    next = *(parser->lex_p + 1);

    if (c == '\\' && (next == '\\' || next == lex_strterm->term || next == lex_strterm->paren)) {
        parser->lex_p += 2;
        parser->column += 2;
		len = strlen(buf);
		buf[len]=c; buf[len+1]=next;
		buf[len+2] = '\0';
		goto just_a_text;
        //return tSTRING_CONTENT;
    }

    if (c == lex_strterm->term) {
        c2=nextc();
        if (lex_strterm->nestable) {
            lex_strterm->nest--;
            if (lex_strterm->nest > 0){
				len = strlen(buf);
				buf[len]=c2;
				buf[len+1] = '\0';
				goto just_a_text;
                //return tSTRING_CONTENT;
			}
        }
		if(strlen(buf)>0){
			pushback();
			dest_size = sizeof (buf);
			memset(parser->string_buf, 0, sizeof(parser->string_buf));
			strncpy(parser->string_buf, buf, dest_size);
			parser->string_buf[dest_size - 1] = '\0';
			return tSTRING_CONTENT;
		}
        return tSTRING_END;
    } else if (lex_strterm->nestable && lex_strterm->paren == c) {
        lex_strterm->nest++;
        c2=nextc();
		len = strlen(buf);
		buf[len]=c2;
		buf[len+1] = '\0';
		goto just_a_text;
        //return tSTRING_CONTENT;
    }

    if (IS_EOF()) {
        parser->eof_reached = 1;
        yyerror(parser, "unterminated string meets end of file");
        free(lex_strterm);
        lex_strterm = NULL;
        return token_invalid;
    }

    if (lex_strterm->can_embed && c == '#' && *(parser->lex_prev) != '\\') {
        nextc();
        switch (*parser->lex_p) {
            case '$': case '@':
				if(strlen(buf)>0){
					dest_size = sizeof (buf);
					memset(parser->string_buf, 0, sizeof(parser->string_buf));
					strncpy(parser->string_buf, buf, dest_size);
					parser->string_buf[dest_size - 1] = '\0';
					pushback();
					return tSTRING_CONTENT;
				}
                return tSTRING_DVAR;
            case '{':
				if(strlen(buf)>0){
					dest_size = sizeof (buf);
					memset(parser->string_buf, 0, sizeof(parser->string_buf));
					strncpy(parser->string_buf, buf, dest_size);
					parser->string_buf[dest_size - 1] = '\0';
					pushback();
					return tSTRING_CONTENT;
				}
                c = nextc();
                command_start = 1;
                return tSTRING_DBEG;
        }
        pushback();
    }

    /* Re-using the next and the c variables */
    next = utf8_charsize(parser->lex_p);
    c = next - 1;
    while (next-- > 0) {
        if (c2=nextc() < 0) {
            parser->eof_reached = 1;
            free(lex_strterm);
            lex_strterm = NULL;
            return token_invalid;
        }
    }
    parser->column -= c;
	len = strlen(buf);
	buf[len]=*(parser->lex_p-1);
	//printf("ADD%c", *(parser->lex_p-1));
	buf[len+1] = '\0';
	goto just_a_text;
    //return tSTRING_CONTENT;
}

/* Regular expressions can end with some options, read them */
static void parse_re_options(struct parser_t *parser)
{
    char aux[64];
    int c = *parser->lex_p;

    while (isalpha(c)) {
        if (c != 'i' && c != 'm' && c != 'x' && c != 'o' &&
            c != 'u' && c != 'e' && c != 's' && c != 'n') {
            sprintf(aux, "unknown regexp option - %c", c);
            yyerror(parser, aux);
            return;
        }
        c = nextc();
    }
    pushback();
}

/* Standard warning for ambiguous arguments */
static void arg_ambiguous_gen(struct parser_t *parser)
{
    yywarning("ambiguous first argument; put parentheses or even spaces");
}
#define arg_ambiguous() (arg_ambiguous_gen(parser), 1)

/*
 * Это лексер. Разбивает текст на токены и скармливает парсеру. Так же выставляются необходимые флаги, запоминаются значения
 */
static int parser_yylex(struct parser_t *parser)
{
    register int c;
    int bc = 0;
    char *cp;
    char lexbuf[SSIZE];
    unsigned char space_seen = 0;
    int cmd_state;
    struct pos_t tokp = {-1, -1, -1, -1, 0};

    /* Check for string terminations: string, regexp, heredoc, shortcut */
    if (lex_strterm) {
        if (lex_strterm->token == token_heredoc) {
            c = parse_heredoc(parser);
            if (c == tSTRING_END) {
                tokp.end_line = parser->line;
                tokp.end_col = parser->column;
                SWAP(parser->line, parser->line_pend, bc);
                SWAP(parser->column, parser->column_pend, bc);
                SWAP(parser->lex_p, parser->lex_pend, cp);
                parser->here_found = 1;
                lex_state = EXPR_END;

            }
        } else {
            c = parse_string(parser);
            if (c == tSTRING_END) {
                if (lex_strterm->token == token_regexp && isalpha(*parser->lex_p))
                    parse_re_options(parser);
                lex_state = EXPR_END;
            }
        }
        return c;
    }

    cmd_state = command_start;
    command_start = 0;
retry:
    c = nextc();

    tokp.start_line = parser->line;
    tokp.start_col = parser->column - 1;

    /* Check numeric values here instead of entering the main switch */
    if (isdigit(c)) {
        cp = lexbuf;
        goto tnum;
    }

    switch (c) {
        case '\0':      /* NULL */
        case EOF:       /* end of script */
            parser->eof_reached = 1;
            return token_invalid;

        /* white spaces */
        case ' ': case '\t': case '\f': case '\r':
        case '\13': /* vertical tab */
            space_seen = 1;
            goto retry;
        case '#':
            set_comment(parser);
        case '\n':
            if (IS_lex_state(EXPR_BEG | EXPR_VALUE | EXPR_CLASS | EXPR_FNAME | EXPR_DOT))
                goto retry;
            CMDARG_PUSH(0);
            lex_state = EXPR_BEG;
            command_start = 1;
            return '\n';
        case '=':
            lex_state = IS_AFTER_OPERATOR() ? EXPR_ARG : EXPR_BEG;
            bc = nextc();
            if (bc == '=') {
                if (nextc() == '=')
                    return tEQQ;
                pushback();
                return tEQ;
            }
            if (bc == '~')
                return tMATCH;
            if (bc == '>')
                return tASSOC;
            if (multiline_comment(parser->lex_prev - 1)) {
                parser->column += 4;
                parser->lex_p += 4;
                while (!multiline_end(parser->lex_prev))
                    nextc();
                parser->column += 3;
                parser->lex_p += 3;
                bc = 0;
                goto retry;
            }
            break;
        case '[':
            parser->paren_nest++;
            if (IS_AFTER_OPERATOR()) {
                lex_state = EXPR_ARG;
                bc = nextc();
                if (bc == ']') {
                    if (nextc() == '=')
                        return tASET;
                    c = tAREF;
                }
                break;
            } else if (IS_BEG())
                c = tLBRACKET;
            else if (IS_ARG() && space_seen)
                c = tLBRACKET;
            lex_state = EXPR_BEG;
            COND_PUSH(0);
            CMDARG_PUSH(0);
            return c;
        case ']':
            parser->paren_nest--;
            lex_state = EXPR_ENDARG;
            CMDARG_LEXPOP();
            COND_LEXPOP();
            return c;
        case '<':
            bc = nextc();
            if (bc == '<' && !IS_lex_state(EXPR_DOT | EXPR_CLASS) &&
                !IS_END() && (!IS_ARG() || space_seen)) {
                if (parse_heredoc_identifier(parser))
                    return tSTRING_BEG;
                pushback();
            }
            if (IS_AFTER_OPERATOR())
                lex_state = EXPR_ARG;
            else {
                if (IS_lex_state(EXPR_CLASS))
                    command_start = 1;
                lex_state = EXPR_BEG;
            }
            if (bc == '=') {
                if (nextc() == '>')
                    return tCMP;
                pushback();
                return tLEQ;
            }
            if (bc == '<') {
                if (nextc() == '=') {
                    lex_state = EXPR_BEG;
                    return tOP_ASGN;
                }
                c = tLSHIFT;
            }
            break;
        case '>':
            lex_state = IS_AFTER_OPERATOR() ? EXPR_ARG : EXPR_BEG;
            bc = nextc();
            if (bc == '=')
                return tGEQ;
            if (bc == '>') {
                if (nextc() == '=') {
                    lex_state = EXPR_BEG;
                    return tOP_ASGN;
                }
                c = tRSHIFT;
            }
            break;
        case '!':
            bc = nextc();
            if (IS_AFTER_OPERATOR()) {
                lex_state = EXPR_ARG;
                if (bc == '@')
                    return '!';
            } else
                lex_state = EXPR_BEG;
            if (bc == '=')
                return tNEQ;
            if (bc == '~')
                return tNMATCH;
            break;
        case '+':
            bc = nextc();
            if (IS_AFTER_OPERATOR()) {
                lex_state = EXPR_ARG;
                if (bc == '@')
                    return tUPLUS;
                pushback();
                return '+';
            }
            if (bc == '=') {
                lex_state = EXPR_BEG;
                return tOP_ASGN;
            }
            if (IS_BEG() || (IS_SPCARG(bc) && arg_ambiguous())) {
                lex_state = EXPR_BEG;
                pushback();
                return tUPLUS;
            }
            lex_state = EXPR_BEG;
            break;
        case '-':
            bc = nextc();
            if (IS_AFTER_OPERATOR()) {
                lex_state = EXPR_ARG;
                if (bc == '@')
                    return tUMINUS;
                pushback();
                return '-';
            }
            if (bc == '=') {
                lex_state = EXPR_BEG;
                return tOP_ASGN;
            }
            if (bc == '>') {
                lex_state = EXPR_ENDFN;
                return tLAMBDA;
            }
            if (IS_BEG() || (IS_SPCARG(bc) && arg_ambiguous())) {
                lex_state = EXPR_BEG;
                pushback();
                if (bc != -1 && isdigit(bc))
                    return tUMINUS_NUM;
                return tUMINUS;
            }
            lex_state = EXPR_BEG;
            break;
        case '*':
            bc = nextc();
            if (bc == '=') {
                lex_state = EXPR_BEG;
                return tOP_ASGN;
            }
            if (bc == '*') {
                bc = nextc();
                if (bc == '=') {
                    lex_state = EXPR_BEG;
                    return tOP_ASGN;
                }
                pushback();
                if (IS_SPCARG(bc)) {
                    yywarning("`**' interpreted as argument prefix");
                    c = tDSTAR;
                } else if (IS_BEG())
                    c = tDSTAR;
                else
                    c = tPOW;
                lex_state = IS_AFTER_OPERATOR() ? EXPR_ARG : EXPR_BEG;
                return c;
            }
            if (IS_SPCARG(bc)) {
                yywarning("`*' interpreted as argument prefix");
                c = tSTAR;
            } else if (IS_BEG())
                c = tSTAR;
            lex_state = IS_AFTER_OPERATOR() ? EXPR_ARG : EXPR_BEG;
            break;
        case '/':
            if (IS_lex_state(EXPR_BEG_ANY)) {
            regexp:
                lex_strterm = (struct term_t *) malloc(sizeof(struct term_t));
                lex_strterm->term = c;
                lex_strterm->can_embed = 1;
                lex_strterm->token = token_regexp;
                lex_strterm->word = NULL;
                lex_strterm->nestable = 0;
                lex_strterm->paren = 0;
                return tSTRING_BEG;
            }
            bc = nextc();
            if (bc == '=') {
                lex_state = EXPR_BEG;
                return tOP_ASGN;
            }
            pushback();
            if (IS_SPCARG(bc)) {
                arg_ambiguous_gen(parser);
                goto regexp;
            }
            lex_state = IS_AFTER_OPERATOR() ? EXPR_ARG : EXPR_BEG;
            return c;
        case '%':
            bc = nextc();
            if (IS_lex_state(EXPR_BEG_ANY) || IS_SPCARG(bc)) {
                lex_strterm = (struct term_t *) malloc(sizeof(struct term_t));
                lex_strterm->token = guess_kind(parser, bc);
                if (isalpha(bc))
                    bc = nextc();
                lex_strterm->term = closing_char(bc);
                lex_strterm->can_embed = 1;
                lex_strterm->word = NULL;
                lex_strterm->paren = bc;
                lex_strterm->nestable = (bc != lex_strterm->term);
                lex_strterm->nest = 1;
                return tSTRING_BEG;
            }
            if (bc == '=') {
                lex_state = EXPR_BEG;
                return tOP_ASGN;
            }
            lex_state = IS_AFTER_OPERATOR() ? EXPR_ARG : EXPR_BEG;
            break;
        case '&':
            bc = nextc();
            if (bc == '&') {
                lex_state = EXPR_BEG;
                if (nextc() == '=')
                    return tOP_ASGN;
                pushback();
		
                return tAND;
            }
            if (bc == '=') {
                lex_state = EXPR_BEG;
                return tOP_ASGN;
            }
            if (IS_SPCARG(bc)) {
                yywarning("`&' interpreted as argument prefix");
                c = tAMPER;
            } else if (IS_BEG())
                c = tAMPER;
            lex_state = IS_AFTER_OPERATOR() ? EXPR_ARG : EXPR_BEG;
            break;
        case '|':
            bc = nextc();
            if (bc == '|') {
                lex_state = EXPR_BEG;
                if (nextc() == '=')
                    return tOP_ASGN;
                pushback();
                return tOR;
            }
            if (bc == '=') {
                lex_state = EXPR_BEG;
                return tOP_ASGN;
            }
            lex_state = IS_AFTER_OPERATOR() ? EXPR_ARG : EXPR_BEG;
            break;
        case '.':
            lex_state = EXPR_BEG;
            bc = nextc();
            if (bc == '.') {
                if (nextc() == '.')
                    return tDOT3;
                pushback();
                return tDOT2;
            }
            lex_state = EXPR_DOT;
            break;
        case ':':
            bc = nextc();
            if (bc == ':') {
                if (IS_BEG() || IS_lex_state(EXPR_CLASS) || IS_SPCARG(-1)) {
                    lex_state = EXPR_BEG;
                    return tCOLON3;
                }
                lex_state = EXPR_DOT;
                return tCOLON2;
            }
            if (IS_END() || isspace(bc)) {
                lex_state = EXPR_BEG;
                break;
            }
            lex_state = EXPR_FNAME;
            pushback();
            return tSYMBEG;
        case '^':
            if (nextc() == '=') {
                lex_state = EXPR_BEG;
                return tOP_ASGN;
            }
            lex_state = IS_AFTER_OPERATOR() ? EXPR_ARG : EXPR_BEG;
            break;
        case ';':
            command_start = 1;
            /* fallthrough */
        case ',':
            lex_state = EXPR_BEG;
            return c;
        case '?':
            if (IS_END()) {
                lex_state = EXPR_VALUE;
                return c;
            }
            bc = nextc();
            if (isspace(bc)) {
                lex_state = EXPR_VALUE;
                break;
            }
            if (bc == '\\')
                nextc();
            if (IS_BEG()) {
                lex_state = EXPR_END;
                return tCHAR;
            }
            if (is_blank(*parser->lex_p) || *parser->lex_p == ':') {
                lex_state = EXPR_VALUE;
                break;
            }
            lex_state = EXPR_END;
            return tCHAR;
        case '`':
            if (IS_lex_state(EXPR_FNAME)) {
                lex_state = EXPR_ENDFN;
                return c;
            }
            if (IS_lex_state(EXPR_DOT)) {
                lex_state = (cmd_state) ? EXPR_CMDARG : EXPR_ARG;
                return c;
            }
            /* fallthrough */
        case '"':
            space_seen = 1;
            /* fallthrough */
        case '\'':
            lex_strterm = (struct term_t *) malloc(sizeof(struct term_t));
            lex_strterm->term = c;
            lex_strterm->can_embed = space_seen;
            lex_strterm->token = token_string;
            lex_strterm->word = NULL;
            lex_strterm->nestable = 0;
            lex_strterm->paren = 0;
            return tSTRING_BEG;
        case '\\':
            if (nextc() == '\n') {
                space_seen = 1;
                goto retry;
            }
            pushback();
            return c;
        case '(':
            if (IS_BEG())
                c = tLPAREN;
            else if (IS_SPCARG(-1))
                c = tLPAREN_ARG;
            parser->paren_nest++;
            lex_state = EXPR_BEG;
            COND_PUSH(0);
            CMDARG_PUSH(0);
            return c;
        case ')':
            parser->paren_nest--;
            lex_state = EXPR_ENDFN;
            CMDARG_LEXPOP();
            COND_LEXPOP();
            return c;
        case '{':
            if (parser->lpar_beg && parser->lpar_beg == parser->paren_nest) {
                lex_state = EXPR_BEG;
                parser->lpar_beg = 0;
                parser->paren_nest--;
                COND_PUSH(0);
                CMDARG_PUSH(0);
                push_pos(parser, tokp);
                if (parser->version < ruby19) {
                    yywarning("\"->\" syntax is only available in Ruby 1.9.x or higher.");
                }
                return tLAMBEG; /* this is a lambda ->() {} construction */
            }
            if (IS_ARG() || IS_lex_state(EXPR_END | EXPR_ENDFN))
                c = '{';
            else if (IS_lex_state(EXPR_ENDARG))
                c = tLBRACE_ARG; /* block (expr) */
            else
                c = tLBRACE; /* smells like a hash */
            COND_PUSH(0);
            CMDARG_PUSH(0);
            lex_state = EXPR_BEG;
            if (c != tLBRACE) {
                push_pos(parser, tokp);
                command_start = 1;
            }
            return c; /* block (primary) */
        case '}':
            CMDARG_LEXPOP();
            COND_LEXPOP();
            tokp.end_line = parser->line;
            tokp.end_col = parser->column;
            push_pos(parser, tokp);
            lex_state = EXPR_ENDARG;
            return c;
        case '@':
            cp = lexbuf;
            *cp++ = c;
            c = nextc();
            if (c != '@') {
                bc = IVAR;
            } else {
                *cp++ = c;
                c = nextc();
                bc = CVAR;
            }
            goto talpha;
        case '$':
            tokp.end_line = parser->line;
            cp = lexbuf;
            *cp++ = c;
            bc = nextc();
            switch (bc) {
                case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    c = bc;
                    while (isdigit(c)) {
                        *cp++ = c;
                        c = nextc();
                    }
                    *cp = '\0';
                    pushback();
                    c = tNTH_REF;
                    break;
                case '~': case '*': case '$': case '?': case '!': case '@':
                case '/': case '\\': case ';': case ',': case '.': case '=':
                case ':': case '<': case '>': case '\"':
                case '&': case '`': case '\'': case '+':
                case '0':
                    c = GLOBAL;
                    *cp++ = bc;
                    *cp = '\0';
                    break;
                case '-':
                    c = nextc();
                    *cp++ = bc;
                    bc = GLOBAL;
                    goto talpha;
                default:
                    c = bc;
                    bc = GLOBAL;
                    goto talpha;
            }
            lex_state = EXPR_END;
            tokp.end_col = parser->column;
            push_pos(parser, tokp);
            push_stack(parser, lexbuf);
            return c;
        case '~':
            if (IS_AFTER_OPERATOR()) {
                bc = nextc();
                if (bc != '@')
                    pushback();
                lex_state = EXPR_ARG;
            } else
                lex_state = EXPR_BEG;
            return c;
        default:
            cp = lexbuf;
            goto talpha;
    }
    pushback();
    return c;

talpha:
    {
        int step = 0;
        int ax = 0;

        /* It's time to parse the word */
        while (not_sep(parser->lex_prev)) {
            step = utf8_charsize(parser->lex_prev);
            ax += step - 1;
            while (step-- > 0) {
                *cp++ = c;
                c = nextc();
            }
            if (c < 0) {
                parser->eof_reached = 1;
                break;
            }
        }
        *cp = '\0';
        parser->column -= ax;
        tokp.end_line = tokp.start_line;
        tokp.end_col = parser->lex_prevc - ax;
        pushback();

        /* IVAR, CVAR, GLOBAL */
        if (bc > 0) {
            push_pos(parser, tokp);
            push_stack(parser, lexbuf);
            lex_state = EXPR_END;
            return bc;
        }

        /* Check for '!', '?' and '=' at the end of the word */
        if (c == '!' || c == '?') {
            *cp++ = c;
            *cp = '\0';
            tokp.end_col++;
            nextc();
            c = BASE;
        } else {
            c = 0;
            if (IS_lex_state(EXPR_FNAME)) {
                bc = nextc();
                if (bc == '=') {
                    bc = nextc();
                    if (bc != '>') {
                        *cp++ = '=';
                        *cp = '\0';
                        tokp.end_col++;
                        c = BASE;
                    } else
                        pushback();
                }
                pushback();
            }
            c = (!c && isupper(lexbuf[0])) ? CONST : BASE;
        }

        /* Check if this is just a hash key. */
        if (IS_LABEL_POSSIBLE() && IS_LABEL_SUFFIX()) {
            lex_state = EXPR_BEG;
            nextc();
            push_stack(parser, lexbuf);
            push_pos(parser, tokp);
            return tKEY;
        }

        /* Check if this is a keyword */
        const struct kwtable *kw = NULL;
        if (!IS_lex_state(EXPR_DOT)) {
            kw = rb_reserved_word(lexbuf, cp - lexbuf);
            if (kw) {
                enum lex_state_e state = lex_state;
                lex_state = kw->state;
                if (state == EXPR_FNAME)
                    return kw->id[0];
                if (lex_state == EXPR_BEG)
                    command_start = 1;
                switch (kw->id[0]) {
                    case tCLASS: case tMODULE: case tDEF:
                        push_last_comment(parser);
                        break;
                    case tDO:
                        if (parser->lpar_beg && parser->lpar_beg == parser->paren_nest) {
                            parser->lpar_beg = 0;
                            parser->paren_nest--;
                            return tDO_LAMBDA;
                        }
                        if (COND_P())
                            return tDO_COND;
                        push_pos(parser, tokp);
                        if (CMDARG_P() && state != EXPR_CMDARG)
                            return tDO_BLOCK;
                        return tDO;
                    case tEND:
                        push_pos(parser, tokp);
                        break;
                }
                if (IS_lex_state_for(state, EXPR_BEG | EXPR_VALUE))
                    return kw->id[0];
                else {
                    if (kw->id[0] != kw->id[1])
                        lex_state = EXPR_BEG;
                    return kw->id[1];
                }
            }
        }

        /* Maybe this is just some special method */
        if (is_special_method(lexbuf)) {
            if (!strcmp(lexbuf, "__END__")) {
                parser->eof_reached = 1;
                return tpEND;
            }
        }

        /* If this is not a keyword, push its position and the name */
        if (!kw) {
            push_stack(parser, lexbuf);
            push_pos(parser, tokp);
        }

        /* Update the state of the lexer */
        if (IS_lex_state(EXPR_BEG_ANY | EXPR_ARG_ANY | EXPR_DOT))
            lex_state = (cmd_state) ? EXPR_CMDARG : EXPR_ARG;
        else if (lex_state == EXPR_FNAME)
            lex_state = EXPR_ENDFN;
        else
            lex_state = EXPR_END;
        return c;
    }

tnum:
    {
        char hex, bin, has_point, aux;
        hex = bin = has_point = aux = 0;
		unsigned long start = parser->str_pos;
		int tlength;
		
        lex_state = EXPR_END;
        if (c == '0') {
            bc = nextc();
            if (toupper(bc) == 'X') {
                hex = 1;
                c = nextc();
            } else if (toupper(bc) == 'B') {
                bin = 1;
                c = nextc();
            }
            pushback();
        }
        while (c > 0 && ((isdigit(c) && !bin) || (!hex && !bin && !has_point && c == '.')
                    || (hex && toupper(c) >= 'A' && toupper(c) < 'G')
                    || (bin && (c == '1' || c == '0')) || c == '_')) {
            if (c == '.') {
                if (!isdigit(*parser->lex_p)) {
                    tokp.end_line = parser->line;
                    tokp.end_col = parser->column - 1;
                    pushback();
					
					//save value
					tlength = parser->str_pos - start + 1;
					memset(parser->string_val, 0, sizeof(parser->string_val));
					strncpy(parser->string_val, parser->lex_p - tlength, tlength);
					parser->string_val[tlength] = '\0';
                    return tINTEGER;
                }
                has_point = 1;
            }
            aux = 1;
            c = nextc();
        }
        if ((bin || hex) && !aux)
            yyerror(parser, "numeric literal without digits");

        /* is it an exponential number ? */
        if (!bin && !hex && toupper(c) == 'E') {
            c = nextc();
            if (isdigit(c) || ((c == '+' || c == '-') && isdigit(*(parser->lex_p))))
                c = nextc();
            while (c != -1 && isdigit(c))
                c = nextc();
        }

        if (c != -1)
            pushback();
        tokp.end_line = parser->line + 1;
        tokp.end_col = parser->column + 1;
        if (c == 'r') {
            nextc();
			//save value
			tlength = parser->str_pos - start + 1;
			memset(parser->string_val, 0, sizeof(parser->string_val));
			strncpy(parser->string_val, parser->lex_p - tlength, tlength);
			parser->string_val[tlength] = '\0';
            return tRATIONAL;
        } else if (c == 'i') {
            nextc();
			//save value
			tlength = parser->str_pos - start + 1;
			memset(parser->string_val, 0, sizeof(parser->string_val));
			strncpy(parser->string_val, parser->lex_p - tlength, tlength);
			parser->string_val[tlength] = '\0';
            return tIMAGINARY;
        }
        tokp.end_line--;
        tokp.end_col--;
		//save value
		tlength = parser->str_pos - start + 1;
		memset(parser->string_val, 0, sizeof(parser->string_val));
		strncpy(parser->string_val, parser->lex_p - tlength, tlength);
		parser->string_val[tlength] = '\0';
        return (has_point) ? tFLOAT : tINTEGER;
    }
}

/* Стандартный yylex. */
#if YYPURE
static int yylex(void *lval, void *p)
#else
static int yylex(void *p)
#endif
{
    struct parser_t *parser = (struct parser_t *) p;
    int t = token_invalid;
    _unused_(lval);

    t = parser_yylex(parser);
    return t;
}

/*
 * Error handling. Take the formmated string s and append the error
 * string to the list of errors p->errors.
 */
static void yyerror(struct parser_t *parser, const char *s)
{
    struct error_t *e = (struct error_t *) malloc(sizeof(struct error_t));

    e->msg = strdup(s);
    e->line = parser->line;
    e->column = parser->column;
    e->warning = parser->warning;
    e->next = e;
    if (parser->errors)
        parser->last_error->next = e;
    else
        parser->errors = e;
    parser->last_error = e;
    parser->last_error->next = NULL;

    parser->eof_reached = !e->warning;
    if (!parser->unrecoverable)
      parser->unrecoverable = !e->warning;
}

struct ast_t * rb_compile_file(struct options_t *opts)
{
    struct parser_t p;
    struct ast_t *result;

    /* Initialize parser */
    init_parser(&p);
    p.version = opts->version;
    if (!opts->contents) {
        if (!retrieve_source(&p, opts->path))
            return NULL;
    } else {
        p.content_given = 1;
        p.length = strlen(opts->contents);
        p.blob = opts->contents;
        p.lex_p = opts->contents;
    }

    /* Let's parse */
    result = (struct ast_t *) malloc(sizeof(struct ast_t));
    result->tree = NULL;
    result->unrecoverable = 0;
    for (;;) {
        yyparse(&p);
        if (p.ast != NULL) {
            if (result->tree == NULL)
                result->tree = p.ast;
            else
                update_list(result->tree, p.ast);
        }
        if (p.eof_reached) {
            result->errors = p.errors;
            result->unrecoverable = p.unrecoverable;
            break;
        }
    }
    free_parser(&p);

    return result;
}

#ifdef BUILD_TESTS
/*
 * Compile a file like the rb_compile_file function but printing
 * things directly to the stdout. This function is used for the tests.
 */
int rb_debug_file(struct options_t *opts)
{
    struct parser_t p;
    int index;

    /* Set up parser */
    init_parser(&p);
    p.version = opts->version;
    if (!retrieve_source(&p, opts->path))
        return 0;

    printf("Resulting AST's:");
    for (;;) {
        printf("\n");
        yyparse(&p);
        print_node(p.ast);
        if (p.ast != NULL) {
            if (p.ast->cond != NULL) {
                printf("\nCondition: ");
                print_node(p.ast->cond);
            }
            if (p.ast->l != NULL && p.ast->l->ensure != NULL) {
                if (p.ast->l->cond != NULL) {
                    printf("\nCondition: ");
                    print_node(p.ast->l->cond);
                }
                printf("\nEnsure: ");
                print_node(p.ast->l->ensure);
            }
            free_ast(p.ast);
            p.ast = NULL;
        }
        if (p.eof_reached) {
            if (p.errors)
                print_errors(p.errors);
            break;
        }
    }

    /* Check that all the stacks are empty */
    for (index = 0; index < p.sp; index++)
        printf("\nS: %s", p.stack[index]);
    printf("\n");

    for (index = 0; index < p.pos_size; index++)
        printf("\nP: %i:%i", p.pos_stack[index].start_line, p.pos_stack[index].start_col);
    printf("\n");
    free_parser(&p);
    return 1;
}
#endif
