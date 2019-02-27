
xml2: xml2.c xml2.sm
	cc $(CFLAGS) -o xml2 xml2.c


test: xml2
	test "$$(echo '<a><b f="1"></b></a>' |./xml2 |head -n1)" = "/a/b/f	1"

xml2.pp.c: xml2.c
	cat xml2.c |grep -v "include <" |cpp >xml2.pp.c

