/****
  COMMON; common.c, (include to include all of common-c)

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
#if HAVE_CONFIG_H
# include <config.h>
#endif

#if HAVE_STRTONUM
# include <bsd/stdlib.h>
# include <limits.h>
#endif

#include <errno.h>
#include <string.h>
#include "commoner.h"

# if !defined(HAVE_STRLCAT) && !defined(strlcat)
/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
inline size_t strlcat(char *dst, const char *src, size_t siz)
{
     /*-
      * SPDX-License-Identifier: BSD-3-Clause
      *
      * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
      * All rights reserved.
      *
      * Redistribution and use in source and binary forms, with or without
      * modification, are permitted provided that the following conditions
      * are met:
      * 1. Redistributions of source code must retain the above copyright
      *    notice, this list of conditions and the following disclaimer.
      * 2. Redistributions in binary form must reproduce the above copyright
      *    notice, this list of conditions and the following disclaimer in the
      *    documentation and/or other materials provided with the distribution.
      * 3. The name of the author may not be used to endorse or promote products
      *    derived from this software without specific prior written permission.
      *
      * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
      * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
      * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
      * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
      * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
      * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
      * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
      * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
      * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
      * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
      */
     char *d = dst;
     const char *s = src;
     size_t n = siz;
     size_t dlen;

     /* Find the end of dst and adjust bytes left but don't go past end */
     while (n-- != 0 && *d != '\0')
          d++;
     dlen = d - dst;
     n = siz - dlen;

     if (n == 0)
          return(dlen + strlen(s));
     while (*s != '\0') {
          if (n != 1) {
               *d++ = *s;
               n--;
          }
          s++;
     }
     *d = '\0';

     return(dlen + (s - src));       /* count does not include NUL */
}
#endif

# if !defined(HAVE_STRLCPY) && !defined(strlcpy)
/*
 * Copy string src to buffer dst of size dsize.  At most dsize-1
 * chars will be copied.  Always NUL terminates (unless dsize == 0).
 * Returns strlen(src); if retval >= dsize, truncation occurred.
 */
inline size_t strlcpy(char * __restrict dst, const char * __restrict src, size_t dsize)
{
     /*
      * Copyright (c) 1998, 2015 Todd C. Miller <Todd.Miller@courtesan.com>
      *
      * Permission to use, copy, modify, and distribute this software for any
      * purpose with or without fee is hereby granted, provided that the above
      * copyright notice and this permission notice appear in all copies.
      *
      * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
      * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
      * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
      * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
      * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
      * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
      * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
      */

     const char *osrc = src;
     size_t nleft = dsize;

     /* Copy as many bytes as will fit. */
     if (nleft != 0) {
          while (--nleft != 0) {
               if ((*dst++ = *src++) == '\0')
                    break;
          }
     }

     /* Not enough room in dst, add NUL and traverse rest of src. */
     if (nleft == 0) {
          if (dsize != 0)
               *dst = '\0';            /* NUL-terminate dst */
          while (*src++);
     }

     return(src - osrc - 1); /* count does not include NUL */
}
#endif

# if !defined(HAVE_BZERO) && !defined(bzero)
inline void bzero(void *ptr, size_t sz)
{memset(ptr, '\0', sz);}
# endif

# if !defined(HAVE_BCOPY) && !defined(bcopy)
inline void bcopy(const void *src, void *dest, size_t n)
{memmove(dest, src, n);}
# endif

# if !defined(HAVE_MEMPCPY) && !defined(mempcpy)
inline void *mempcpy(void *dest, const void *src, size_t n)
{
     void *tmp = memcpy(dest, src, n);
     tmp += n;
     return tmp;
}
# endif

inline void *mempmove(void *dest, const void *src, size_t n)
{
     void *tmp = memmove(dest, src, n);
     tmp += n;
     return tmp;
}

/* if *dst is NULL, remember to free it later */
int stoll(long long *dst, const char *s0)
{
     if (dst == NULL)
	  dst = malloc(1 * sizeof(*dst));

# if HAVE_STRTONUM || defined(strtonum)
     *dst = strtonum(s0, INT_MIN, INT_MAX, NULL);
# else
     *dst = (long long)strtol(s0, NULL, 10);
# endif
     return (errno) ? errno : 0;
}

#if defined(COMMONER_NEEDS_A_MAIN)
int main(int argc, char **argv)
{
     return EXIT_SUCCESS;
}
#endif
