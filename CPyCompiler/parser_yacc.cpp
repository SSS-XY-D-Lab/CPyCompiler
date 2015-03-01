#include "stdafx.h"
/* A Bison parser, made by GNU Bison 3.0.2.  */

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "cpyexp.y" /* yacc.c:339  */

#define YYSTYPE stnode::stnode*
#include "parser.h"
int yylex();
void yyerror(const char *);

#line 74 "parser_yacc.cpp" /* yacc.c:339  */

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
	ID = 258,
	NUMBER = 259,
	CHARA = 260,
	STR = 261,
	DELIM = 262,
	DTYPE = 263,
	COMMA = 264,
	SUB_LEFT = 265,
	SUB_RIGHT = 266,
	BRACKET_LEFT = 267,
	BRACKET_RIGHT = 268,
	INC = 269,
	DEC = 270,
	NOT = 271,
	LGNOT = 272,
	DIV = 273,
	MUL = 274,
	MOD = 275,
	ADD = 276,
	SUB = 277,
	SHL = 278,
	SHR = 279,
	BIG = 280,
	BIGEQU = 281,
	LES = 282,
	LESEQU = 283,
	EQU = 284,
	NEQU = 285,
	AND = 286,
	XOR = 287,
	BOR = 288,
	LGAND = 289,
	LGOR = 290,
	ASSIGN = 291,
	MODASS = 292,
	DIVASS = 293,
	MULASS = 294,
	ADDASS = 295,
	SUBASS = 296,
	SHLASS = 297,
	SHRASS = 298,
	ANDASS = 299,
	XORASS = 300,
	BORASS = 301,
	QMARK = 302,
	COLON = 303
};
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse(void);



/* Copy the second part of user declarations.  */

#line 171 "parser_yacc.cpp" /* yacc.c:358  */

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
void *malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free(void *); /* INFRINGES ON USER NAME SPACE */
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
#define YYFINAL  80
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   169

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  122

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
	0, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 1, 2, 3, 4,
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
	35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
	45, 46, 47, 48
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
	0, 14, 14, 15, 16, 17, 18, 20, 21, 22,
	23, 24, 25, 27, 28, 29, 30, 31, 32, 33,
	34, 35, 37, 38, 40, 41, 42, 43, 45, 46,
	47, 49, 50, 51, 53, 54, 55, 56, 57, 59,
	60, 61, 63, 64, 66, 67, 69, 70, 72, 73,
	75, 76, 78, 79, 81, 82, 83, 84, 85, 86,
	87, 88, 89, 90, 91, 92, 94, 95, 97
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
	"$end", "error", "$undefined", "ID", "NUMBER", "CHARA", "STR", "DELIM",
	"DTYPE", "COMMA", "SUB_LEFT", "SUB_RIGHT", "BRACKET_LEFT",
	"BRACKET_RIGHT", "INC", "DEC", "NOT", "LGNOT", "DIV", "MUL", "MOD",
	"ADD", "SUB", "SHL", "SHR", "BIG", "BIGEQU", "LES", "LESEQU", "EQU",
	"NEQU", "AND", "XOR", "BOR", "LGAND", "LGOR", "ASSIGN", "MODASS",
	"DIVASS", "MULASS", "ADDASS", "SUBASS", "SHLASS", "SHRASS", "ANDASS",
	"XORASS", "BORASS", "QMARK", "COLON", "$accept", "exp1", "exp2", "exp3",
	"exp4", "exp5", "exp6", "exp7", "exp8", "exp9", "exp10", "exp11",
	"exp12", "exp13", "exp14", "exp15", "exp16", "exp", "expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
(internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
	0, 256, 257, 258, 259, 260, 261, 262, 263, 264,
	265, 266, 267, 268, 269, 270, 271, 272, 273, 274,
	275, 276, 277, 278, 279, 280, 281, 282, 283, 284,
	285, 286, 287, 288, 289, 290, 291, 292, 293, 294,
	295, 296, 297, 298, 299, 300, 301, 302, 303
};
# endif

#define YYPACT_NINF -33

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-33)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
STATE-NUM.  */
static const yytype_int16 yypact[] =
{
	118, -9, -33, -33, -33, 78, 138, 138, 118, 118,
	118, 118, 118, 118, -33, -6, 28, -33, 24, 16,
	36, 80, 46, -3, 48, 56, 57, -8, -33, -33,
	22, 116, 98, 105, 21, 118, -33, -33, -33, -33,
	-33, -33, -33, -33, -33, 118, -33, -33, 118, 118,
	118, 118, 118, 118, 118, 118, 118, 118, 118, 118,
	118, 118, 118, 118, 118, 118, 118, 118, 118, 118,
	118, 118, 118, 118, 118, 118, 118, 118, -33, 118,
	-33, -33, 27, 118, -33, 87, -33, -33, -33, -33,
	-33, -33, -33, -33, -33, -33, -33, -33, -33, -33,
	24, 24, 16, 16, 36, 36, 36, 36, 80, 80,
	46, -3, 48, 56, 57, -7, -33, -33, -33, -33,
	118, -33
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
Performed when YYTABLE does not specify something else to do.  Zero
means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
	0, 2, 3, 5, 4, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 7, 13, 22, 24, 28, 31,
	34, 39, 42, 44, 46, 48, 50, 52, 54, 66,
	0, 0, 0, 0, 0, 0, 14, 15, 22, 20,
	21, 17, 18, 19, 16, 0, 11, 12, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 68, 0,
	1, 9, 0, 0, 6, 0, 55, 56, 57, 58,
	59, 60, 61, 62, 63, 64, 65, 26, 25, 27,
	29, 30, 32, 33, 35, 36, 37, 38, 40, 41,
	43, 45, 47, 49, 51, 0, 67, 10, 23, 8,
	0, 53
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
	-33, -33, -33, 0, 2, 25, 81, 59, 77, 64,
	58, 82, 63, 75, -33, 38, -32, 1, -33
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
	-1, 14, 15, 38, 17, 18, 19, 20, 21, 22,
	23, 24, 25, 26, 27, 28, 29, 34, 31
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
positive, shift that token.  If negative, reduce the rule whose
number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
	16, 30, 79, 32, 45, 16, 36, 37, 46, 47,
	39, 40, 41, 42, 43, 44, 86, 87, 88, 89,
	90, 91, 92, 93, 94, 95, 96, 76, 72, 78,
	79, 79, 16, 82, 84, 16, 79, 62, 63, 77,
	117, 120, 59, 60, 61, 16, 85, 116, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 64,
	65, 97, 98, 99, 48, 49, 50, 51, 52, 53,
	54, 55, 56, 57, 58, 70, 71, 16, 115, 16,
	73, 1, 2, 3, 4, 118, 33, 100, 101, 74,
	5, 75, 6, 7, 8, 9, 79, 10, 119, 11,
	12, 1, 2, 3, 4, 66, 67, 68, 69, 13,
	5, 81, 6, 7, 8, 9, 80, 10, 83, 11,
	12, 1, 2, 3, 4, 104, 105, 106, 107, 13,
	5, 111, 6, 7, 8, 9, 110, 10, 113, 11,
	12, 1, 2, 3, 4, 102, 103, 108, 109, 13,
	35, 114, 6, 7, 8, 9, 112, 10, 121, 11,
	12, 0, 0, 0, 0, 0, 0, 0, 0, 13
};

static const yytype_int8 yycheck[] =
{
	0, 0, 9, 12, 10, 5, 6, 7, 14, 15,
	8, 9, 10, 11, 12, 13, 48, 49, 50, 51,
	52, 53, 54, 55, 56, 57, 58, 35, 31, 7,
	9, 9, 32, 32, 13, 35, 9, 21, 22, 47,
	13, 48, 18, 19, 20, 45, 45, 79, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 23,
	24, 59, 60, 61, 36, 37, 38, 39, 40, 41,
	42, 43, 44, 45, 46, 29, 30, 77, 77, 79,
	32, 3, 4, 5, 6, 83, 8, 62, 63, 33,
	12, 34, 14, 15, 16, 17, 9, 19, 11, 21,
	22, 3, 4, 5, 6, 25, 26, 27, 28, 31,
	12, 13, 14, 15, 16, 17, 0, 19, 13, 21,
	22, 3, 4, 5, 6, 66, 67, 68, 69, 31,
	12, 73, 14, 15, 16, 17, 72, 19, 75, 21,
	22, 3, 4, 5, 6, 64, 65, 70, 71, 31,
	12, 76, 14, 15, 16, 17, 74, 19, 120, 21,
	22, -1, -1, -1, -1, -1, -1, -1, -1, 31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
	0, 3, 4, 5, 6, 12, 14, 15, 16, 17,
	19, 21, 22, 31, 50, 51, 52, 53, 54, 55,
	56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
	66, 67, 12, 8, 66, 12, 52, 52, 52, 53,
	53, 53, 53, 53, 53, 10, 14, 15, 36, 37,
	38, 39, 40, 41, 42, 43, 44, 45, 46, 18,
	19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 47, 7, 9,
	0, 13, 66, 13, 13, 66, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 65, 53, 53, 53,
	54, 54, 55, 55, 56, 56, 56, 56, 57, 57,
	58, 59, 60, 61, 62, 66, 65, 13, 53, 11,
	48, 64
};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
	0, 49, 50, 50, 50, 50, 50, 51, 51, 51,
	51, 51, 51, 52, 52, 52, 52, 52, 52, 52,
	52, 52, 53, 53, 54, 54, 54, 54, 55, 55,
	55, 56, 56, 56, 57, 57, 57, 57, 57, 58,
	58, 58, 59, 59, 60, 60, 61, 61, 62, 62,
	63, 63, 64, 64, 65, 65, 65, 65, 65, 65,
	65, 65, 65, 65, 65, 65, 66, 66, 67
};

/* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
	0, 2, 1, 1, 1, 1, 3, 1, 4, 3,
	4, 2, 2, 1, 2, 2, 2, 2, 2, 2,
	2, 2, 1, 4, 1, 3, 3, 3, 1, 3,
	3, 1, 3, 3, 1, 3, 3, 3, 3, 1,
	3, 3, 1, 3, 1, 3, 1, 3, 1, 3,
	1, 3, 1, 5, 1, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 1, 3, 2
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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
      {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
      }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print(FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
	FILE *yyo = yyoutput;
	YYUSE(yyo);
	if (!yyvaluep)
		return;
# ifdef YYPRINT
	if (yytype < YYNTOKENS)
		YYPRINT(yyoutput, yytoknum[yytype], *yyvaluep);
# endif
	YYUSE(yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print(FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
	YYFPRINTF(yyoutput, "%s %s (",
		yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

	yy_symbol_value_print(yyoutput, yytype, yyvaluep);
	YYFPRINTF(yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print(yytype_int16 *yybottom, yytype_int16 *yytop)
{
	YYFPRINTF(stderr, "Stack now");
	for (; yybottom <= yytop; yybottom++)
	{
		int yybot = *yybottom;
		YYFPRINTF(stderr, " %d", yybot);
	}
	YYFPRINTF(stderr, "\n");
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
yy_reduce_print(yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
	unsigned long int yylno = yyrline[yyrule];
	int yynrhs = yyr2[yyrule];
	int yyi;
	YYFPRINTF(stderr, "Reducing stack by rule %d (line %lu):\n",
		yyrule - 1, yylno);
	/* The symbols being reduced.  */
	for (yyi = 0; yyi < yynrhs; yyi++)
	{
		YYFPRINTF(stderr, "   $%d = ", yyi + 1);
		yy_symbol_print(stderr,
			yystos[yyssp[yyi + 1 - yynrhs]],
			&(yyvsp[(yyi + 1) - (yynrhs)])
			);
		YYFPRINTF(stderr, "\n");
	}
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
yystrlen(const char *yystr)
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
yystpcpy(char *yydest, const char *yysrc)
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
yytnamerr(char *yyres, const char *yystr)
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
	do_not_strip_quotes:;
	}

	if (!yyres)
		return yystrlen(yystr);

	return yystpcpy(yyres, yystr) - yyres;
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
yysyntax_error(YYSIZE_T *yymsg_alloc, char **yymsg,
yytype_int16 *yyssp, int yytoken)
{
	YYSIZE_T yysize0 = yytnamerr(YY_NULLPTR, yytname[yytoken]);
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
		if (!yypact_value_is_default(yyn))
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
					&& !yytable_value_is_error(yytable[yyx + yyn]))
				{
					if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
					{
						yycount = 1;
						yysize = yysize0;
						break;
					}
					yyarg[yycount++] = yytname[yyx];
					{
						YYSIZE_T yysize1 = yysize + yytnamerr(YY_NULLPTR, yytname[yyx]);
						if (!(yysize <= yysize1
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
	  YYSIZE_T yysize1 = yysize + yystrlen(yyformat);
	  if (!(yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
		  return 2;
	  yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
  {
	  *yymsg_alloc = 2 * yysize;
	  if (!(yysize <= *yymsg_alloc
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
			  yyp += yytnamerr(yyp, yyarg[yyi++]);
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
yydestruct(const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
	YYUSE(yyvaluep);
	if (!yymsg)
		yymsg = "Deleting";
	YY_SYMBOL_PRINT(yymsg, yytype, yyvaluep, yylocationp);

	YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
		YYUSE(yytype);
	YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse(void)
{
	int yystate;
	/* Number of tokens to shift before error messages enabled.  */
	int yyerrstatus;

	/* The stacks and their tools:
	'yyss': related to states.
	'yyvs': related to semantic values.

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

	YYDPRINTF((stderr, "Starting parse\n"));

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
			yyoverflow(YY_("memory exhausted"),
				&yyss1, yysize * sizeof(*yyssp),
				&yyvs1, yysize * sizeof(*yyvsp),
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
				(union yyalloc *) YYSTACK_ALLOC(YYSTACK_BYTES(yystacksize));
			if (!yyptr)
				goto yyexhaustedlab;
			YYSTACK_RELOCATE(yyss_alloc, yyss);
			YYSTACK_RELOCATE(yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
			if (yyss1 != yyssa)
				YYSTACK_FREE(yyss1);
		}
# endif
#endif /* no yyoverflow */

		yyssp = yyss + yysize - 1;
		yyvsp = yyvs + yysize - 1;

		YYDPRINTF((stderr, "Stack size increased to %lu\n",
			(unsigned long int) yystacksize));

		if (yyss + yystacksize - 1 <= yyssp)
			YYABORT;
	}

	YYDPRINTF((stderr, "Entering state %d\n", yystate));

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
	if (yypact_value_is_default(yyn))
		goto yydefault;

	/* Not known => get a lookahead token if don't already have one.  */

	/* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
	if (yychar == YYEMPTY)
	{
		YYDPRINTF((stderr, "Reading a token: "));
		yychar = yylex();
	}

	if (yychar <= YYEOF)
	{
		yychar = yytoken = YYEOF;
		YYDPRINTF((stderr, "Now at end of input.\n"));
	}
	else
	{
		yytoken = YYTRANSLATE(yychar);
		YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
	}

	/* If the proper action on seeing token YYTOKEN is to reduce or to
	detect an error, take that action.  */
	yyn += yytoken;
	if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
		goto yydefault;
	yyn = yytable[yyn];
	if (yyn <= 0)
	{
		if (yytable_value_is_error(yyn))
			goto yyerrlab;
		yyn = -yyn;
		goto yyreduce;
	}

	/* Count tokens shifted since error; after three, turn off error
	status.  */
	if (yyerrstatus)
		yyerrstatus--;

	/* Shift the lookahead token.  */
	YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);

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
	'$$ = $1'.

	Otherwise, the following line sets YYVAL to garbage.
	This behavior is undocumented and Bison
	users should not rely upon it.  Assigning to YYVAL
	unconditionally makes the parser a bit smaller, and it avoids a
	GCC warning that YYVAL may be used uninitialized.  */
	yyval = yyvsp[1 - yylen];


	YY_REDUCE_PRINT(yyn);
	switch (yyn)
	{
		case 6:
#line 18 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = (yyvsp[-1]); }
#line 1340 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 8:
#line 21 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::ARRAY_SUB, (yyvsp[-3]), (yyvsp[-1])); (yyval)->pos = (yyvsp[-2])->pos; (yyval)->lineN = (yyvsp[-2])->lineN; delete (yyvsp[-2]); delete (yyvsp[0]); }
#line 1346 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 9:
#line 22 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::call(dynamic_cast<stnode::id*>((yyvsp[-2]))); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; delete (yyvsp[-1]); delete (yyvsp[0]); }
#line 1352 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 10:
#line 23 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::call(dynamic_cast<stnode::id*>((yyvsp[-3])), (yyvsp[-1])); (yyval)->pos = (yyvsp[-2])->pos; (yyval)->lineN = (yyvsp[-2])->lineN; delete (yyvsp[-2]); delete (yyvsp[0]); }
#line 1358 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 11:
#line 24 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::INC_POST, (yyvsp[-1])); (yyval)->pos = (yyvsp[0])->pos; (yyval)->lineN = (yyvsp[0])->lineN; delete (yyvsp[0]); }
#line 1364 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 12:
#line 25 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::DEC_POST, (yyvsp[-1])); (yyval)->pos = (yyvsp[0])->pos; (yyval)->lineN = (yyvsp[0])->lineN; delete (yyvsp[0]); }
#line 1370 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 14:
#line 28 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::INC_PRE, (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; delete (yyvsp[-1]); }
#line 1376 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 15:
#line 29 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::DEC_PRE, (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; delete (yyvsp[-1]); }
#line 1382 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 16:
#line 30 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::REF, (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; delete (yyvsp[-1]); }
#line 1388 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 17:
#line 31 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::DEREF, (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; delete (yyvsp[-1]); }
#line 1394 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 18:
#line 32 "cpyexp.y" /* yacc.c:1646  */
		{ if ((yyvsp[0])->getType() == stnode::type::NUMBER) { (yyval) = (yyvsp[0]); }
		else { (yyval) = new stnode::op::op(stnode::op::ops::POSI, (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; } delete (yyvsp[-1]); }
#line 1400 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 19:
#line 33 "cpyexp.y" /* yacc.c:1646  */
		{ if ((yyvsp[0])->getType() == stnode::type::NUMBER) { (yyval) = new stnode::number(-dynamic_cast<stnode::number*>((yyvsp[0]))->val); }
		else { (yyval) = new stnode::op::op(stnode::op::ops::NEGA, (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; } delete (yyvsp[-1]); }
#line 1406 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 20:
#line 34 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[0]); (yyval) = op; }
#line 1412 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 21:
#line 35 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[0]); (yyval) = op; }
#line 1418 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 23:
#line 38 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::cast *cast = dynamic_cast<stnode::cast*>((yyvsp[-2])); cast->node = (yyvsp[0]); (yyval) = cast; delete (yyvsp[-3]); delete (yyvsp[-1]); }
#line 1424 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 25:
#line 41 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::MUL, (yyvsp[-2]), (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; delete (yyvsp[-1]); }
#line 1430 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 26:
#line 42 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1436 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 27:
#line 43 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1442 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 29:
#line 46 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::ADD, (yyvsp[-2]), (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; delete (yyvsp[-1]); }
#line 1448 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 30:
#line 47 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::SUB, (yyvsp[-2]), (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; delete (yyvsp[-1]); }
#line 1454 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 32:
#line 50 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1460 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 33:
#line 51 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1466 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 35:
#line 54 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1472 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 36:
#line 55 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1478 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 37:
#line 56 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1484 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 38:
#line 57 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1490 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 40:
#line 60 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1496 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 41:
#line 61 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1502 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 43:
#line 64 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::op::op(stnode::op::ops::AND, (yyvsp[-2]), (yyvsp[0])); (yyval)->pos = (yyvsp[-1])->pos; (yyval)->lineN = (yyvsp[-1])->lineN; delete (yyvsp[-1]); }
#line 1508 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 45:
#line 67 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1514 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 47:
#line 70 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1520 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 49:
#line 73 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1526 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 51:
#line 76 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1532 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 53:
#line 79 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::ifelse *cExp = new stnode::ifelse; cExp->pos = (yyvsp[-3])->pos; cExp->lineN = (yyvsp[-3])->lineN; cExp->exp = (yyvsp[-4]); cExp->blockTrue = new stTree; cExp->blockTrue->push_back((yyvsp[-2])); cExp->blockFalse = new stTree; cExp->blockFalse->push_back((yyvsp[0])); delete (yyvsp[-3]); delete (yyvsp[-1]); (yyval) = cExp; }
#line 1538 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 55:
#line 82 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1544 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 56:
#line 83 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1550 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 57:
#line 84 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1556 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 58:
#line 85 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1562 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 59:
#line 86 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1568 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 60:
#line 87 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1574 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 61:
#line 88 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1580 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 62:
#line 89 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1586 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 63:
#line 90 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1592 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 64:
#line 91 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1598 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 65:
#line 92 "cpyexp.y" /* yacc.c:1646  */
		{ stnode::op::op *op = dynamic_cast<stnode::op::op*>((yyvsp[-1])); op->arg[0] = (yyvsp[-2]); op->arg[1] = (yyvsp[0]); (yyval) = op; }
#line 1604 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 67:
#line 95 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = new stnode::expTree((yyvsp[0]), (yyvsp[-2])); (yyval)->pos = (yyvsp[-1])->pos; delete (yyvsp[-1]); }
#line 1610 "parser_yacc.cpp" /* yacc.c:1646  */
		break;

		case 68:
#line 97 "cpyexp.y" /* yacc.c:1646  */
		{ (yyval) = (yyvsp[-1]); yacc_result = (yyvsp[-1]); }
#line 1616 "parser_yacc.cpp" /* yacc.c:1646  */
		break;


#line 1620 "parser_yacc.cpp" /* yacc.c:1646  */
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
	YY_SYMBOL_PRINT("-> $$ =", yyr1[yyn], &yyval, &yyloc);

	YYPOPSTACK(yylen);
	yylen = 0;
	YY_STACK_PRINT(yyss, yyssp);

	*++yyvsp = yyval;

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
	yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE(yychar);

	/* If not already recovering from an error, report this error.  */
	if (!yyerrstatus)
	{
		++yynerrs;
#if ! YYERROR_VERBOSE
		yyerror(YY_("syntax error"));
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
					YYSTACK_FREE(yymsg);
				yymsg = (char *)YYSTACK_ALLOC(yymsg_alloc);
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
			yyerror(yymsgp);
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
			yydestruct("Error: discarding",
				yytoken, &yylval);
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

	/* Do not reclaim the symbols of the rule whose action triggered
	this YYERROR.  */
	YYPOPSTACK(yylen);
	yylen = 0;
	YY_STACK_PRINT(yyss, yyssp);
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
		if (!yypact_value_is_default(yyn))
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


		yydestruct("Error: popping",
			yystos[yystate], yyvsp);
		YYPOPSTACK(1);
		yystate = *yyssp;
		YY_STACK_PRINT(yyss, yyssp);
	}

	YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
		*++yyvsp = yylval;
	YY_IGNORE_MAYBE_UNINITIALIZED_END


		/* Shift the error token.  */
		YY_SYMBOL_PRINT("Shifting", yystos[yyn], yyvsp, yylsp);

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
yyexhaustedlab :
	yyerror(YY_("memory exhausted"));
	yyresult = 2;
	/* Fall through.  */
#endif

yyreturn:
	if (yychar != YYEMPTY)
	{
		/* Make sure we have latest lookahead translation.  See comments at
		user semantic actions for why this is necessary.  */
		yytoken = YYTRANSLATE(yychar);
		yydestruct("Cleanup: discarding lookahead",
			yytoken, &yylval);
	}
	/* Do not reclaim the symbols of the rule whose action triggered
	this YYABORT or YYACCEPT.  */
	YYPOPSTACK(yylen);
	YY_STACK_PRINT(yyss, yyssp);
	while (yyssp != yyss)
	{
		yydestruct("Cleanup: popping",
			yystos[*yyssp], yyvsp);
		YYPOPSTACK(1);
	}
#ifndef yyoverflow
	if (yyss != yyssa)
		YYSTACK_FREE(yyss);
#endif
#if YYERROR_VERBOSE
	if (yymsg != yymsgbuf)
		YYSTACK_FREE(yymsg);
#endif
	return yyresult;
}
#line 99 "cpyexp.y" /* yacc.c:1906  */


int yylex()
{
	if (yacc_p == yacc_pEnd)
		return -1;
	int ret = -1;
	switch ((*yacc_p)->getType())
	{
		case token::type::ID:
		{
			token::id* tk = static_cast<token::id*>(*yacc_p);
			yylval = new stnode::id(tk->str, dataType());
			ret = ID;
			break;
		}
		case token::type::CHARA:
		{
			token::chara* tk = static_cast<token::chara*>(*yacc_p);
			yylval = new stnode::chara(tk->ch);
			ret = CHARA;
			break;
		}
		case token::type::STR:
		{
			token::str* tk = static_cast<token::str*>(*yacc_p);
			yylval = new stnode::str(tk->strr);
			ret = STR;
			break;
		}
		case token::type::NUMBER:
		{
			token::number* tk = static_cast<token::number*>(*yacc_p);
			yylval = new stnode::number(tk->val);
			ret = NUMBER;
			break;
		}
		case token::type::OP:
		{
			token::op* tk = static_cast<token::op*>(*yacc_p);
			stnode::op::ops newOP;
			switch (tk->opType)
			{
				case token::ops::opType::COMMA:
					ret = COMMA;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::SUB_LEFT:
					ret = SUB_LEFT;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::SUB_RIGHT:
					ret = SUB_RIGHT;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::BRACKET_LEFT:
					ret = BRACKET_LEFT;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::BRACKET_RIGHT:
					ret = BRACKET_RIGHT;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::INC:
					ret = INC;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::DEC:
					ret = DEC;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::NOT:
					ret = NOT;
					newOP = stnode::op::ops::NOT;
					break;
				case token::ops::opType::LGNOT:
					ret = LGNOT;
					newOP = stnode::op::ops::LGNOT;
					break;
				case token::ops::opType::DIV:
					ret = DIV;
					newOP = stnode::op::ops::DIV;
					break;
				case token::ops::opType::MUL:
					ret = MUL;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::MOD:
					ret = MOD;
					newOP = stnode::op::ops::MOD;
					break;
				case token::ops::opType::ADD:
					ret = ADD;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::SUB:
					ret = SUB;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::SHL:
					ret = SHL;
					newOP = stnode::op::ops::SHL;
					break;
				case token::ops::opType::SHR:
					ret = SHR;
					newOP = stnode::op::ops::SHR;
					break;
				case token::ops::opType::BIG:
					ret = BIG;
					newOP = stnode::op::ops::BIG;
					break;
				case token::ops::opType::BIGEQU:
					ret = BIGEQU;
					newOP = stnode::op::ops::BIGEQU;
					break;
				case token::ops::opType::LES:
					ret = LES;
					newOP = stnode::op::ops::LES;
					break;
				case token::ops::opType::LESEQU:
					ret = LESEQU;
					newOP = stnode::op::ops::LESEQU;
					break;
				case token::ops::opType::EQU:
					ret = EQU;
					newOP = stnode::op::ops::EQU;
					break;
				case token::ops::opType::NEQU:
					ret = NEQU;
					newOP = stnode::op::ops::NEQU;
					break;
				case token::ops::opType::AND:
					ret = AND;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::XOR:
					ret = XOR;
					newOP = stnode::op::ops::XOR;
					break;
				case token::ops::opType::BOR:
					ret = BOR;
					newOP = stnode::op::ops::BOR;
					break;
				case token::ops::opType::LGAND:
					ret = LGAND;
					newOP = stnode::op::ops::LGAND;
					break;
				case token::ops::opType::LGOR:
					ret = LGOR;
					newOP = stnode::op::ops::LGOR;
					break;
				case token::ops::opType::ASSIGN:
					ret = ASSIGN;
					newOP = stnode::op::ops::ASSIGN;
					break;
				case token::ops::opType::MODASS:
					ret = MODASS;
					newOP = stnode::op::ops::MODASS;
					break;
				case token::ops::opType::DIVASS:
					ret = DIVASS;
					newOP = stnode::op::ops::DIVASS;
					break;
				case token::ops::opType::MULASS:
					ret = MULASS;
					newOP = stnode::op::ops::MULASS;
					break;
				case token::ops::opType::ADDASS:
					ret = ADDASS;
					newOP = stnode::op::ops::ADDASS;
					break;
				case token::ops::opType::SUBASS:
					ret = SUBASS;
					newOP = stnode::op::ops::SUBASS;
					break;
				case token::ops::opType::SHLASS:
					ret = SHLASS;
					newOP = stnode::op::ops::SHLASS;
					break;
				case token::ops::opType::SHRASS:
					ret = SHRASS;
					newOP = stnode::op::ops::SHRASS;
					break;
				case token::ops::opType::ANDASS:
					ret = ANDASS;
					newOP = stnode::op::ops::ANDASS;
					break;
				case token::ops::opType::XORASS:
					ret = XORASS;
					newOP = stnode::op::ops::XORASS;
					break;
				case token::ops::opType::BORASS:
					ret = BORASS;
					newOP = stnode::op::ops::BORASS;
					break;
				case token::ops::opType::QMARK:
					ret = QMARK;
					newOP = stnode::op::ops::ERROR;
					break;
				case token::ops::opType::COLON:
					ret = COLON;
					newOP = stnode::op::ops::ERROR;
					break;
				default:
					yyerror("Illegal Token(OP)");
					return -1;
			}
			stnode::op::op *opPtr;
			switch (newOP)
			{
				case stnode::op::ops::NOT:
				case stnode::op::ops::LGNOT:
					opPtr = new stnode::op::op(newOP, NULL);
					break;
				case stnode::op::ops::DIV:
				case stnode::op::ops::MOD:
				case stnode::op::ops::SHL:
				case stnode::op::ops::SHR:
				case stnode::op::ops::BIG:
				case stnode::op::ops::BIGEQU:
				case stnode::op::ops::LES:
				case stnode::op::ops::LESEQU:
				case stnode::op::ops::EQU:
				case stnode::op::ops::NEQU:
				case stnode::op::ops::XOR:
				case stnode::op::ops::BOR:
				case stnode::op::ops::LGAND:
				case stnode::op::ops::LGOR:
				case stnode::op::ops::ASSIGN:
				case stnode::op::ops::MODASS:
				case stnode::op::ops::DIVASS:
				case stnode::op::ops::MULASS:
				case stnode::op::ops::ADDASS:
				case stnode::op::ops::SUBASS:
				case stnode::op::ops::SHLASS:
				case stnode::op::ops::SHRASS:
				case stnode::op::ops::ANDASS:
				case stnode::op::ops::XORASS:
				case stnode::op::ops::BORASS:
					opPtr = new stnode::op::op(newOP, NULL, NULL);
					break;
				default:
					opPtr = new stnode::op::op(newOP);
			}
			yylval = opPtr;
			break;
		}
		case token::type::KEYWORD:
		{
			token::keyword* tk = static_cast<token::keyword*>(*yacc_p);
			int ptrLvl = 0;
			yacc_p++;
			while (yacc_p != yacc_pEnd && (*yacc_p)->getType() == token::type::OP && static_cast<token::op *>(*yacc_p)->opType == token::ops::opType::MUL)
			{
				ptrLvl++;
				yacc_p++;
			}
			dataType::type varType;
			switch (tk->word)
			{
				case token::keywords::SINT:
					varType = dataType::SINT;
					break;
				case token::keywords::S8:
					varType = dataType::S8;
					break;
				case token::keywords::S16:
					varType = dataType::S16;
					break;
				case token::keywords::S32:
					varType = dataType::S32;
					break;
				case token::keywords::S64:
					varType = dataType::S64;
					break;
				case token::keywords::UINT:
					varType = dataType::UINT;
					break;
				case token::keywords::U8:
					varType = dataType::U8;
					break;
				case token::keywords::U16:
					varType = dataType::U16;
					break;
				case token::keywords::U32:
					varType = dataType::U32;
					break;
				case token::keywords::U64:
					varType = dataType::U64;
					break;
				case token::keywords::VOID:
					varType = dataType::VOID;
					break;
				default:
					varType = dataType::ERROR;
					yyerror("Illegal Token(Keyword)");
					return -1;
			}
			yylval = new stnode::cast(NULL, dataType(varType, ptrLvl));
			ret = DTYPE;
			break;
		}
		case token::type::DELIM:
			yylval = new stnode::delim;
			ret = DELIM;
	}
	yylval->pos = (*yacc_p)->pos;
	yylval->lineN = yacc_lineN;
	yacc_p++;
	return ret;
}

void yyerror(const char *err)
{
	yacc_err = err;
}
