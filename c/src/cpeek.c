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
#include <string.h>
#include "cpeek.h"

char cpeek(const char *c, const char *s, const short fwd)
{
     if (fwd > 0) {
	  if (*c == '\0'
# if defined(_GNU_SOURCE)
	      || c == strchr(s, '\0') - 1
# else
	      || c == &s[strlen(s)]
# endif
	       )
	       return *c;
	  else
	       return *(c + 1);
     }
     return (c == s) ? *c : *(c - 1);
}
