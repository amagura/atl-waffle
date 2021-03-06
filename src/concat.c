/****
  COMMONER; concat.c, (should be merged with similar files)

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
#ifndef COMMONER_CONCAT_C_GUARD
# define COMMONER_CONCAT_C_GUARD 1
# ifdef HAVE_CONFIG_H
#  include <config.h>
# endif

# if defined(HAVE_BZERO) || defined(HAVE_BCOPY)
#  include <strings.h>
# endif

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <stdbool.h>
# include <string.h>
# include <limits.h>
# include "commoner.h"

// XXX OBSOLETE
# if 0
char *concat(const char *s1, ...) __attribute__((sentinel))
     __attribute__((warn_unused_result));

// XXX returned value needs free
char *concat(const char *s1, ...)
{
	va_list args;
	const char *s;
	char *p, *result;
	unsigned long l, m, n;
	m = n = strlen(s1);
	va_start(args, s1);
	while ((s = va_arg(args, char *))) {
		l = strlen(s);
		if ((m += l) < l) break;
	}
	va_end(args);
	if (s || m >= INT_MAX) return NULL;
	comnr_mtrace;

# if defined(__cplusplus)
	result = (char *)malloc(m + 1);
# else
	result = malloc(m + 1);
# endif
	if (!result) return NULL;

	memcpy(p = result, s1, n);
	p += n;
	va_start(args, s1);
	while ((s = va_arg(args, char *))) {
		l = strlen(s);
		if ((n += l) < l || n > m) break;
		memcpy(p, s, l);
		p += l;
	}
	va_end(args);
	if (s || m != n || p != result + n) {
		free(result);
		return NULL;
	}
	*p = '\0';
	comnr_muntrace;
	return result;
}
# endif

/* unlike `concat', which returns a
 * new pointer that must then be copied
 * or acted upon in some meaningfully meaningless
 * manner, `catl' returns the number of bytes belonging
 * to `dst', which could _NOT_ be filled xor could _NOT_ be copied,
 * always copying no more than `dsize` of data into `dst'
 *
 * If the return value, which we'll call `y', is negative, 
 * then the resulting catenation has been truncated by `!y'
 * many bytes.  Similarlly, if a positive value is returned:
 * `y' many bytes is how much of `dst', which was _NOT_ used.
 *
 * XXX A failure is indicated by a return value _equal to
 * the destination buffers size_, which may make errors somewhat
 * harder to spot! */

size_t concatl(char *dst, size_t dsize, const char *src0, ...) __attribute__((sentinel));

size_t concatl(char *dst, size_t dsize, const char *src0, ...)
{
     va_list args;
     const char *src_x = NULL;
     char *p, *tmp;
     unsigned long ldx, mdx, ndx;
     size_t used = 0;

     mdx = ndx = strlen(src0);
     va_start(args, src0);
     while ((src_x = va_arg(args, char *))) {
	  ldx = strlen(src_x);
	  if ((mdx += ldx) < ldx) break;
     }
     va_end(args);
     if (src_x || mdx >= INT_MAX) return dsize;
     coint_mtrace;
# if defined(__cplusplus)
     tmp = (char *)malloc(mdx + 1);
# else
     tmp = malloc(mdx + 1);
# endif
     if (!tmp) return dsize;
     bzero(tmp, mdx + 1);
     bzero(dst, mdx + 1);

     p = tmp;
     p = mempcpy(p, (char *)src0, ndx);

     used += ndx;
     COINT_DBG("p: `%s`\n", p);
     COINT_DBG("used: %lu\n", used - 0);

     va_start(args, src0);
     while ((src_x = va_arg(args, char *))) {
	  ldx = strlen(src_x);
	  if ((ndx += ldx) < ldx || ndx > mdx) break;
	  p = mempcpy(p, (char *)src_x, ldx);
	  used += ldx;
     }
     va_end(args);
     if (src_x || mdx != ndx || p != tmp + ndx) {
	  free(tmp);
	  return dsize;
     }

     COINT_DBG("tmp: `%s'\n", tmp);
     p = mempcpy(dst, tmp, (used > dsize ? dsize : used));
     free(tmp);
     *p = '\0';
     ++used;

     COINT_DBG("dst: `%s'\n", dst);
     COINT_DBG("*p: `%c'\n", *p);
     COINT_DBG("*--p: `%c'\n", cpeek(p, dst));
     COINT_DBG("strlen(dst): %lu\n", strlen(dst));
     COINT_DBG("used#2: %lu\n", used - 0);
     coint_muntrace;
     return dsize - used;
}

/* concatm is a little different:
 * unlike `concatl' or `concat', concatm _moves_ memory: that is, the destination
 * pointer can be passed as an argument.
 * In every other regard, concatm behaves exactly like concatl. */
size_t concatm(char *dst, size_t dsize, const char *src0, ...) __attribute__((sentinel));

size_t concatm(char *dst, size_t dsize, const char *src0, ...)
{
     va_list args;
     const char *src_x = NULL;
     char *p, *tmp;
     unsigned long ldx, mdx, ndx;
     size_t used = 0;

     mdx = ndx = strlen(src0);
     va_start(args, src0);
     while ((src_x = va_arg(args, char *))) {
	  ldx = strlen(src_x);
	  if ((mdx += ldx) < ldx) break;
     }
     va_end(args);
     if (src_x || mdx >= INT_MAX) return dsize;

     coint_mtrace;

# if defined(__cplusplus)
     tmp = (char *)malloc(mdx + 1);
# else
     tmp = malloc(mdx + 1);
# endif
     if (!tmp) return dsize;
     bzero(tmp, mdx + 1);

     p = tmp;
     p = mempcpy(p, (char *)src0, ndx);

     used += ndx;
     COINT_DBG("p: `%s`\n", p);
     COINT_DBG("used: %lu\n", used - 0);

     va_start(args, src0);
     while ((src_x = va_arg(args, char *))) {
	  ldx = strlen(src_x);
	  if ((ndx += ldx) < ldx || ndx > mdx) break;
	  p = mempcpy(p, (char *)src_x, ldx);
	  used += ldx;
     }
     va_end(args);
     if (src_x || mdx != ndx || p != tmp + ndx) {
	  free(tmp);
	  return dsize;
     }
     COINT_DBG("tmp: `%s'\n", tmp);
     p = mempmove(dst, tmp, (used > dsize ? dsize : used));
     free(tmp);
     *p = '\0';
     ++used;

     COINT_DBG("dst: `%s'\n", dst);
     COINT_DBG("*p: `%c'\n", *p);
     COINT_DBG("*--p: `%c'\n", cpeek(p, dst));
     COINT_DBG("strlen(dst): %lu\n", strlen(dst));
     COINT_DBG("used#2: %lu\n", used - 0);
     coint_muntrace;

     return dsize - used;
}

#if 0
void *shrnkcat(size_t src_size, size_t max, const char *src0, ...) __attribute__((sentinel));

void *shrnkcat(size_t src_size, size_t max, const char *src0, ...)
{
     va_list args;
     const char *src_x = NULL;
     char *p, *tmp;
     p = tmp = NULL;
     unsigned long ldx, mdx, ndx;
     size_t used = 0;

     mdx = ndx = strlen(src0);
     va_start(args, src0);

     char *buf = malloc(max);
}
#endif
#endif
