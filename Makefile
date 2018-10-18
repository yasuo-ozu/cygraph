SRCS=cg_*.cc
HEADERS=cg_*.h

.PHONY:	clean log
cygraph: ${SRCS} ${HEADERS} Makefile
	g++ -o $@ -g3 ${SRCS}
clean:
	rm -f cygraph
log:
	git log --pretty=format:" - %s %n   http://github.com/yasuo-ozu/cygraph/commit/%H/" --since=10hour
