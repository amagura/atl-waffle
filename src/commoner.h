/****
  COMMON; commoner.h

  Copyright (C) 2015, 2016, 2017, 2018 Alexej G. Magura

  This file is a part of Commoner

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 ****/
#ifndef COMMONER_COMMONER_H_GUARD
# define COMMONER_COMMONER_H_GUARD 1
/* BEGIN_C_DECLS should be used at the beginning of your declarations,
   so that C++ compilers don't mangle their names.  Use END_C_DECLS at
   the end of C declarations. */
# undef BEGIN_C_DECLS
# undef END_C_DECLS
# ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS }
# else
#  define BEGIN_C_DECLS /* empty */
#  define END_C_DECLS /* empty */
# endif

/* PARAMS is a macro used to wrap function prototypes, so that
   compilers that don't understand ANSI C prototypes still work,
   and ANSI C compilers can issue warnings about type mismatches. */
# undef PARAMS
# if defined (__STDC__) || defined (_AIX) \
  || (defined (__mips) && defined (_SYSTYPE_SVR4)) \
  || defined(WIN32) || defined(__cplusplus)
#  define PARAMS(protos) protos
# else
#  define PARAMS(protos) ()
# endif

BEGIN_C_DECLS

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
/////////////////////////////////////////
// PRIVATE Macros
/////////////////////////////////////////
# if defined(COINT_MCHECK) \
  && defined(HAVE_MCHECK_H)
#  include <mcheck.h>
#  define coint_mtrace mtrace()
#  define coint_muntrace muntrace()
# else
#  define coint_mtrace
#  define coint_muntrace
# endif

# if COINT_INTERNAL_DEBUG
#  if !defined(COINT_INTERNAL_DLVL)
#   define COINT_INTERNAL_DLVL (COINT_INTERNAL_DEBUG + 1) // XXX change this to increase/decrease debug verbosity
#  endif
# endif

# if defined(COINT_INTERNAL_DEBUG)
#  if !defined(COINT_PROGNAME)
#   define COINT_PROGNAME "commoner"
#  endif
#  define COINT_DBG(COINT_format, ...)				\
  do {								\
    fprintf(stderr, "## (%s)(%s)%d\n",				\
        COINT_PROGNAME, __FILE__, __LINE__);			\
    fprintf(stderr, "#  `%s'\n", __FUNCTION__);			\
    fprintf(stderr, (COINT_format), ##__VA_ARGS__);		\
    fprintf(stderr, "\n");					\
  } while(0)
#  define COINT_SDBG(COINT_format, COINT_exp)			\
  do {								\
    fprintf(stderr, "## (%s)(%s)%d\n",				\
        COINT_PROGNAME, __FILE__, __LINE__);			\
    fprintf(stderr, "#  `%s`\n", __FUNCTION__);			\
    fprintf(stderr, (COINT_format), (COINT_exp));		\
    fprintf(stderr, "\n");					\
  } while(0)
#  define COINT_ONDBG(...) (__VA_ARGS__)
#  define COINT_XONDBG(COINT_X) (COINT_X)
#  define coint_ping COINT_DBG("\n^^^^ %s ^^^^\n", "MARCO!")
#  define coint_pong COINT_DBG("\n$$$$ %s $$$$\n", "POLO!")
#  define coint_neko(COINT_F, ...)			\
  do {                                                  \
    fprintf(stderr,					\
        "\n%s{neko-chan}%s(%s)(%s)(%d)\n",		\
        "\033[91m❤\033[0m",                          \
        "\033[91m❤\033[0m",			        \
        __FILE__, __FUNCTION__, __LINE__);		\
    fprintf(stderr, "%s%s%s, %s%s%s~\n",		\
        "\033[32mn",					\
        "\033[35my",					\
        "\033[31ma\033[0m",				\
        "\033[32mn",					\
        "\033[35my",					\
        "\033[31ma\033[33ma\033[0m");			\
    fprintf(stderr, (COINT_F), ##__VA_ARGS__);		\
    fprintf(stderr, "\n");				\
  } while(0)

# else
#  define COINT_DBG(COINT_format, ...)
#  define COINT_SDBG(COINT_format, COINT_exp)
#  define COINT_ONDBG(...)
#  define COINT_XONDBG(COINT_X)
#  define coint_ping
#  define coint_pong
#  define coint_neko(COINT_F, ...)
# endif

/////////////////////////////////////////
// PUBLIC Macros
/////////////////////////////////////////

/* XXX Do not use the cat? macros when debugging */
# define catl(...) (concatl(__VA_ARGS__, (void *)NULL))
# define catm(...) (concatm(__VA_ARGS__, (void *)NULL))

# if defined(COMNR_MCHECK) \
  && defined(HAVE_MCHECK_H)
#  include <mcheck.h>
#  define comnr_mtrace mtrace()
#  define comnr_muntrace muntrace()
# else
#  define comnr_mtrace
#  define comnr_muntrace
# endif

# if !defined(COMNR_PROGNAME)
#  define COMNR_PROGNAME "commoner"
# endif

# if !defined(PROGNAME)
#  define PROGNAME COMNR_PROGNAME
# endif

# if !defined(COMMONER_VERSION)
#  define COMMONER_VERSION 0x0110
# endif

# if COMNR_DEBUG
#  define COMNR_DBG(COMNR_format, ...)			\
  do {							\
    fprintf(stderr, "## (%s)(%s)%d\n",			\
        PROGNAME, __FILE__, __LINE__);		        \
    fprintf(stderr, "#  `%s'\n", __FUNCTION__);	        \
    fprintf(stderr, (COMNR_format), ##__VA_ARGS__);	\
    fprintf(stderr, "\n");				\
  } while(0)
#  define COMNR_SDBG(COMNR_format, COMNR_exp)           \
  do {			     				\
    fprintf(stderr, "## (%s)(%s)%d\n",                  \
        PROGNAME, __FILE__, __LINE__);	                \
    fprintf(stderr, "#  `%s`\n", __FUNCTION__);	        \
    fprintf(stderr, (COMNR_format), (COMNR_exp));	\
    fprintf(stderr, "\n");				\
  } while(0)
#  define COMNR_ONDBG(...) (__VA_ARGS__)
#  define COMNR_XONDBG(COMNR_X) (COMNR_X)
#  define comnr_ping COMNR_DBG("\n^^^^ %s ^^^^\n", "MARCO!")
#  define comnr_pong COMNR_DBG("\n$$$$ %s $$$$\n", "POLO!")
#  define comnr_neko(COMNR_F, ...)		\
  do {						\
    fprintf(stderr,				\
        "\n%s{neko-chan}%s(%s)(%s)(%d)\n",	\
        "\033[91m❤\033[0m",	                \
        "\033[91m❤\033[0m",			\
        __FILE__, __FUNCTION__, __LINE__);	\
    fprintf(stderr, "%s%s%s, %s%s%s~\n",	\
        "\033[32mn",				\
        "\033[35my",                            \
        "\033[31ma\033[0m",		      	\
        "\033[32mn",				\
        "\033[35my",				\
        "\033[31ma\033[33ma\033[0m");		\
    fprintf(stderr, (COMNR_F), ##__VA_ARGS__);  \
    fprintf(stderr, "\n");			\
  } while(0)
#  define comnr_log(...)            \
  do {                              \
    fprintf(stderr, ##__VA_ARGS__); \
  } while(0)
# else
#  define COMNR_DBG(COINT_format, ...)
#  define COMNR_SDBG(COINT_format, COINT_exp)
#  define COMNR_ONDBG(...)
#  define COMNR_XONDBG(COINT_X)
#  define comnr_ping
#  define comnr_pong
#  define comnr_neko(COINT_F, ...)
#  define comnr_log(...)
# endif
# define COMNR_ERROR(COMNR_format, ...)		      \
  do {                                                \
    fprintf(stderr, "%s:err: ", PROGNAME);      \
    fprintf(stderr, (COMNR_format), __VA_ARGS__);     \
    fprintf(stderr,				      \
        "\nin %s:{%d}:%s()\n",		              \
        __FILE__,				      \
        __LINE__,				      \
        __FUNCTION__);			              \
  } while(0)
# define COMNR_FATAL(...) \
  do {                    \
    prinft("%s: %s\n",	  \
        (PROGNAME),       \
        ##__VA_ARGS__);	  \
    exit(EXIT_FAILURE);	  \
  } while (0)
# define comnr_usage(COMNR_format) \
  (printf((COMNR_format), (PROGNAME)));
# define comnr_arg(COMNR_opt, COMNR_desc, COMNR_tabs) \
  (printf("  %s%s%s\n", (COMNR_opt),(COMNR_tabs),(COMNR_desc)));
# define comnr_arg_eol_tabs "\n\t\t\t\t"
# define comnr_help(COMNR_usage, COMNR_tabs)	    \
  do {					            \
    comnr_usage((COMNR_usage));			    \
    comnr_arg("-h, --help",		            \
        "print this message and exit",	            \
        (COMNR_tabs));			            \
    comnr_arg("-v, --version",			    \
        "print program version and exit",           \
        (COMNR_tabs));			            \
  } while(0)

# define rstrdup(RSTRDUP_DEST, RSTRDUP_MAX, ...)                                                              \
  do {                                                                                                        \
    char *COMMONER_RSTRDUP_XYZ_ABC_tmp = malloc((RSTRDUP_MAX));                                               \
    size_t COMMONER_RSTRDUP_XYZ_ABC_left;                                                                     \
    COMMONER_RSTRDUP_XYZ_ABC_left = concatl(COMMONER_RSTRDUP_XYZ_ABC_tmp, (RSTRDUP_MAX), ##__VA_ARGS__, NULL);\
    size_t COMMONER_RSTRDUP_XYZ_ABC_need = (RSTRDUP_MAX) - COMMONER_RSTRDUP_XYZ_ABC_left;                     \
    COMMONER_RSTRDUP_XYZ_ABC_tmp = realloc(COMMONER_RSTRDUP_XYZ_ABC_tmp, COMMONER_RSTRDUP_XYZ_ABC_need);      \
    memmove((RSTRDUP_DEST), COMMONER_RSTRDUP_XYZ_ABC_tmp, COMMONER_RSTRDUP_XYZ_ABC_need);                      \
    free(COMMONER_RSTRDUP_XYZ_ABC_tmp);                                                                       \
  } while (0)

/* provides legacy access to abs_path */
# define abs_path(COMNR_AP_CCHAR_PTH) \
     getdir((NULL), (COMNR_AP_CCHAR_PTH))

/////////////////////////////////////////
// PUBLIC Functions
/////////////////////////////////////////

/** functions hosted by commoner.c **/
extern size_t strlcat PARAMS((char *dst, const char *src, size_t siz));
extern size_t strlcpy PARAMS((char *dst, const char *src, size_t dsize));
extern void bzero PARAMS((void *src, size_t n));
extern void bcopy PARAMS((const void *src, void *dest, size_t n));
void *mempmove PARAMS((void *dest, const void *src, size_t n));
extern void *mempcpy PARAMS((void *dest, const void *src, size_t n));
int stoll PARAMS((long long *dst, const char *s0));
//int comnr_init PARAMS((void));

/** functions hosted by str.c **/
int chars PARAMS((const char *s, const char c));
int charsterm PARAMS((const char *s, const char c, const char head, const char end));
int charstermp PARAMS((const char *s, const char c, const char *endp));
void trim PARAMS((char *s));
char *ptrim PARAMS((const char *s));
int cmpstrs PARAMS((const char *base, size_t n, ...)) __attribute__((sentinel));
int cmpcase PARAMS((const char *base, size_t n, ...)) __attribute__((sentinel));
char *strend PARAMS((const char *s));
# if defined(COMNR_EXPOSE_OLD_CPEEK)
char old_cpeek PARAMS((const char *c, const char *s, const short fwd));
# endif
char cpeek PARAMS((const char *const sp0, const char *const head));

/* from defunct mem.c */
int memlen PARAMS((const char *s));
char *strterm PARAMS((char *s, size_t sz));
void *memdup PARAMS((const void* src, size_t n));

/* from defunct repeat.c */
int repeat PARAMS((char *dst, const char s, size_t n));
char *repeatp PARAMS((const char s, int n));
int strrep PARAMS((char *dst, const char *s, size_t n));
char *strprep PARAMS((const char *s, int n));

/* from defunct rev.c */
void rev PARAMS((char *s));
char *revp PARAMS((const char *s));
void revn PARAMS((char *s, size_t n));
char *revnp PARAMS((char *s, size_t n));

/** functions hosted by concat.c **/

/* it is worth noting that the original concat
 * is still available in concat.c
 * it is, however, commented out.
 */
size_t concatl PARAMS((char *dst, size_t dsize, const char *src0, ...)) __attribute__((sentinel));
size_t concatm PARAMS((char *dst, size_t dsize, const char *src0, ...)) __attribute__((sentinel));

/** functions hosted by int.c **/
uintmax_t uintm_len PARAMS((uintmax_t idx));
int intlen PARAMS((int idx));
int intlenc PARAMS((const int idx));
size_t intlenm PARAMS((int src));
long randm PARAMS((long max));
uint64_t getrandom PARAMS(());

/* from defunct itoa.c */
void itoa PARAMS((char *dst, int src));
char *itoap PARAMS((const int src));

/** functions hosted by os.c **/
char *subdir PARAMS((char **dirs, size_t ssz));
char *getdir PARAMS((int *err, const char *pth));
int rpath PARAMS((char *pth, size_t n));
bool direxists PARAMS((char *pth));
size_t flen PARAMS((FILE *fp));
int mkstmp PARAMS((char *tmpl));

/** functions hosted by msg.c **/
void die PARAMS((int *rc, const char *format, ...)) /*__attribute__((sentinel))*/;
void kys PARAMS((const char *format, ...)) /*__attribute__((sentinel))*/;
void croak PARAMS((int *rc, const char *msg, ...)) __attribute__((sentinel));
void ribbt PARAMS((const char *msg, ...)) __attribute__((sentinel));

END_C_DECLS

#endif /* COMMONER_COMMONER_H_GUARD */
