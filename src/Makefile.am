## Process this file with automake to produce Makefile.in ##

lib_LTLIBRARIES = libcommoner.la
libcommoner_la_SOURCES = commoner.c int.c os.c str.c concat.c msg.c
libcommoner_la_LDFLAGS = -version-info '0:1:0'
include_HEADERS = commoner.h
noinst_HEADERS = os.h

AM_CFLAGS = -g -ggdb
AM_CPPFLAGS =
AM_LDFLAGS =

clean-local: tidy-local

anew: clean all

lint:
	for f in $(libcommoner_la_SOURCES); do cppcheck --enable=all $$f; echo; done

valgrind: clean $(bin_PROGRAMS)
	for f in $(bin_PROGRAMS); do \
		$@ --leak-check=full --show-leak-kinds=all --track-origins=yes ./"$$f"; \
		echo; \
		echo; \
	done

vvalgrind: clean $(bin_PROGRAMS)
	for f in $(bin_PROGRAMS); do \
		valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./"$$f"; \
		echo; \
		echo; \
	done

mtrace: $(bin_PROGRAMS)
	for f in $^; do MALLOC_TRACE=./mtrace.txt ./$$f; mtrace ./$$f ./mtrace.txt; echo; cat mtrace.txt; echo; echo; done

expand:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -E $(libcommoner_la_SOURCES)

check-local: $(bin_PROGRAMS) refresh-check
	cd test; \
	./configure

refresh-check:
	cd test; \
	autoconf

asm: $(libcommoner_la_SOURCES:.c=.s) $(bin_PROGRAMS)
	for f in $(libcommoner_la_SOURCES:.c=.o); do objdump --file-start-context -slS "$$f" > "$${f//.*/.od}"; done

.c.s:
	$(CC) $(AM_CFLAGS) $(AM_CPPFLAGS) -S $<

.pyx.c:
	$(CC) -E $(libcommoner_la_CPPFLAGS) -o $(@:.c=.co) $< $(top_srcdir)/config.h
	$(CYTHON) --embed -p -2 -Wextra -o $@ $(@:.c=.co)

tidy-local:
	$(RM) *.o || true
	$(RM) *.od || true
	$(RM) *.py || true
	$(RM) *.pyc || true
	$(RM) no_common.h || true
	$(RM) *~
	$(RM) *-stamp*

anew: clean all
ready: clean all tidy-local
null: clean all clean
void: null

prod:
	cd "$(top_srcdir)"; \
	autoconf && \
	./configure --disable-debug neric && \
	$(MAKE)

no-debug:
	cd "$(top_srcdir)"; \
	autoconf && \
	./configure --disable-debug && \
	$(MAKE)

check-configure: refresh-check
	cd test && \
	cat configure | less

check-all: $(bin_PROGRAMS) gzbatt refresh-check
	cd test && \
	./configure --with-gtk2

debug:
	cd "$(top_srcdir)"; \
	autoconf && \
	./configure --enable-debug && \
	$(MAKE)
