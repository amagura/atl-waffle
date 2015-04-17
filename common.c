/****
Copyright 2015 Alexej Magura

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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include "common.h"

int intlen(int idx)
{
     int result = 0;
     while (idx) {
	  ++result;
	  idx /= 10;
     }
     return result;
}

int intclen(const int idx)
{
     int copy[2] = {idx, 0};
     while (*copy) {
	  ++copy[1];
	  copy[0] /= 10;
     }
     return *copy;
}

void intmlen(int dst, int src)
{
     dst = 0;
     while (src) {
	  ++dst;
	  src /= 10;
     }
     memset(&dst, src, sizeof(int));
}

char cpeek(char *c, char *s, short fwd)
{
     char tmp = 0;
     switch (fwd) {
     case 1:
	  if (c == &s[strlen(s) - 1] || c == &s[strlen(s)])
	       return *c;
	  else
	       tmp = *++c;
	  --c;
	  break;
     case 0:
	  if (c == s)
	       return *c;
	  else
	       tmp = *--c;
	  ++c;
	  break;
     }
     return tmp;
}

/* unlike `concat', which returns a
 * new pointer that must then be copied
 * or acted upon in some meaningfully meaningless
 * manner, `catl' returns the number of bytes belonging
 * to `buf', which could _NOT_ be filled, always copying
 * no more than `bufsiz` of data into `buf'
 *
 * If the return value is an integral value, which
 * we'll call `y', that is less than 0,
 * then the resulting catenation has been truncated by `!y'
 * many bytes.  Similarlly, if a positive value is returned:
 * `y' many bytes is how much of `buf', which was _NOT_ used.
 *
 * XXX A failure is indicated by a return value _equal to
 * the destination buffers size_, which may make errors somewhat
 * harder to spot! */
size_t catl(char *buf, size_t bufsiz, const char *s1, ...)
{
     va_list args;
     const char *s;
     char *p, *tmp;
     unsigned long ldx, mdx, ndx;
     size_t used = 0;

     mdx = ndx = strlen(s1);
     va_start(args, s1);
     while ((s = va_arg(args, const char *))) {
	  ldx = strlen(s);
	  if ((mdx += ldx) < ldx) break;
     }
     va_end(args);
     if (s || mdx >= INT_MAX) return bufsiz;

     tmp = malloc(mdx + 1);
     if (!tmp) return bufsiz;
     bzero(tmp, mdx + 1);

     p = mempcpy(p = tmp, s1, ndx);
     used += ndx;
     COM_DBG("p: `%s`\n", p);

     va_start(args, s1);
     while ((s = va_arg(args, const char *))) {
	  ldx = strlen(s);
	  if ((ndx += ldx) < ldx || ndx > mdx) break;
	  p = mempcpy(p, s, ldx);
	  used += ldx;
     }
     va_end(args);
     if (s || mdx != ndx || p != tmp + ndx) {
	  free(tmp);
	  return bufsiz;
     }

     COM_DBG("*p: `%c'\n", *p);
     COM_DBG("*p--: `%c'\n", cpeek(p, tmp, 0));
     *p = '\0';
     ++used;

#if COM_DLVL > 1
     COM_DBG("used > bufsiz: %d\n", used > bufsiz);
     COM_DBG("used == strlen(p): %d\n", used == strlen(p));
     COM_DBG("used: %lu\n", used - 0);
     COM_DBG("strlen(p): %lu\n", strlen(p));
     COM_DBG("strlen(tmp): %lu\n", strlen(tmp));
     COM_DBG("mdx + 1: %lu\n", mdx + 1);
     COM_DBG("p: `%s`\n", p);
     COM_DBG("p == &tmp[%lu + 1]: %d\n", mdx, p == &tmp[mdx + 1]);
     COM_DBG("bufsiz - used: %lu\n", bufsiz - used);
     COM_DBG("used: %lu\n", used);
     COM_DBG("p == &tmp[%lu]: %d\n", mdx - 0, p == &tmp[mdx]);
#endif

     memcpy(buf, tmp, (used > bufsiz ? bufsiz : used));
     free(tmp);
     return bufsiz - used;
}

void rev(char *s)
{
     int hdx[2];
     *hdx = strlen(s) - 1;

     for (int idx = 0; idx < *hdx; ++idx, --hdx[0]) {
	  hdx[1] = s[idx];
	  s[idx] = s[*hdx];
	  s[*hdx] = hdx[1];
     }
}

char *revp(const char *s)
{
     int hdx[2];
     *hdx = strlen(s) - 1;
     char *copy = strdup(s);
     char dst[hdx[0] + 1];
     char *wp = dst;

     for (int idx = 0; idx < *hdx; ++idx, --hdx[0]) {
	  hdx[1] = copy[idx];
	  copy[idx] = copy[*hdx];
	  copy[*hdx] = hdx[1];
     }
     memcpy(wp, copy, hdx[0] + 1);
     free(copy);
     return wp;
}

char *itoap(const int src)
{
     int len[2] = {intclen(src), 0};
     char buf[++*len];
     char *wp = buf;

     for (; *len != 0; ++wp, len[1] /= 10) {
	  if (*len >= 0)
	       *wp = '0' + (len[1] % 10);
	  else
	       *wp = '0' - (len[1] % 10);
#if COM_DLVL > 1
	  COM_DBG("*wp: `%c'\n", *wp);
#endif

     }
     COM_DBG("*wp: `%c'\n", *wp);
     *wp = '\0';

#if COM_DLEVEL > 1
	  COM_DBG("*len: %d\n", *len);
	  COM_DBG("strlen(buf): %lu\n", strlen(buf));
	  COM_DBG("sizeof(buf): %lu\n", sizeof(buf));
	  COM_DBG("*wp: `%c'\n", *wp);
	  COM_DBG("buf: `%s'\n", buf);
#endif
     wp = revp(buf);

     COM_DBG("strlen(buf)#2: %lu\n", strlen(buf));
     COM_DBG("sizeof(buf)#2: %lu\n", sizeof(buf));
     COM_DBG("buf: `%s'\n", buf);
     COM_DBG("*wp: `%c'\n", *wp);

     return wp;
}


void itoa(char *dst, int src)
{
     int len = intlen(src);
     char tmp[++len];
     char *wp = tmp;

     for (; src != 0; ++wp, src /= 10) {
	  if (src >= 0)
	       *wp = '0' + (src % 10);
	  else
	       *wp = '0' - (src % 10);
#if COM_DLVL > 1
	  COM_DBG("*wp: `%c`\n", *wp);
#endif
     }
     COM_DBG("wp: `%s'\n", wp);
     *wp = '\0';
#if COM_DLVL > 1
     COM_DBG("*wp: `%c'\n", *wp);
     COM_DBG("len: %d\n", len);
     COM_DBG("strlen(tmp): %lu\n", strlen(tmp));
     COM_DBG("sizeof(tmp): %lu\n", sizeof(tmp));
     COM_DBG("tmp: `%s'\n", tmp);
#endif
     rev(tmp);
#if COM_DLVL > 1
     COM_DBG("strlen(tmp)#2: %lu\n", strlen(tmp));
     COM_DBG("sizeof(tmp)#2: %lu\n", sizeof(tmp));
     COM_DBG("tmp#2: `%s'\n", tmp);
     COM_DBG("*wp#2: `%c'\n", *wp);
#endif
     COM_DBG("wp: `%s'\n", tmp);
     memcpy(dst, tmp, len);
}
