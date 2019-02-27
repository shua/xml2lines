#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FF, TT } bool;

enum {
#define S(x, ...) x,
#include "xml2.sm"
#undef S
} s;

typedef struct vec {
	unsigned int l, L;
	char* v;
} V;

void clear(V*);
void flush(V*, V*);
V* makeV(char*);
void push(V*, char);
void pop_until(V*, char);

bool etagexpected(V*, V*);

V*
makeV(char* c) {
	V* v = malloc(sizeof(V));

	v->l = strnlen(c, 1024);
	v->L = 1;
	while(v->L <= v->l)
		v->L *= 2;

	v->v = malloc(v->L);
	strncpy(v->v, c, v->l);

	return v;
}

void
clear(V* v) {
	v->v[v->l = 0] = 0;
}

void
flush(V* name, V* val) {
	printf("%s\t%s\n", name->v, val->v);
	val->v[val->l = 0] = 0;
}

void
pop_until(V* v, char c) {
	while(v->l > 0 && v->v[v->l] != c)
		v->l--;
	v->v[v->l] = 0;
}

void
push(V* v, char c) {
	v->v[v->l] = c;
	v->l++;
	if (v->l == v->L) {
		v->L *= 2;
		v->v = realloc(v->v, v->L);
	}
	v->v[v->l] = 0;
}

bool
etagexpected(V* hier, V* etag) {
	int i = hier->l - etag->l;

	if (i != 0 && hier->v[i - 1] != '/')
		return FF;

	return strncmp(hier->v + i, etag->v, etag->l) == 0;
}

int
main(int argc, char* argv[]) {
	V* name = makeV(""),
	 * val  = makeV(""),
	 * etag = makeV("");

	for(
		int c=getc(stdin), line=0, col=0;
		c != EOF;
		line=(c=='\n' ? (col=0, line + 1) : line), c=getc(stdin), col++)
	switch(s) {

#ifdef DEBUG
#	define dbgS(s, c) printf("DBG: S(%s) '%c'\n", #s, c)
#else
#	define dbgS(...)
#endif

#define err(msg) fprintf(stderr, "ERR:%d:%d %s\n", line, col, msg)

#define S(x, ...) case x: dbgS(x, c); switch(c) { __VA_ARGS__ } break;
#define C(c, ...) case c: { __VA_ARGS__ } break;
#define D(...) default: { __VA_ARGS__ } break;

#include "xml2.sm"

#undef err
#undef dbgS
#undef S
#undef C
#undef D

	}
}

