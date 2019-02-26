#include <stdio.h>
#include <stdlib.h>


typedef enum {
	EMPTY = 0,
	USED,
	TOMBSTONE,
} state;



typedef struct {
	state s;
	int k;
	int v;
} entry;


typedef struct {
	entry *entries;
	int size;
} hashmap;


typedef struct {
	int i;
	entry *entries;
	hashmap *h;
} cursor;


int hash(int x) {
	return x;
}


hashmap hashmap_new() {
	hashmap h;
	h.entries = NULL;
	h.size = 0;
	return h;
}


int hashmap_delete(hashmap *h, int k) {
	int i = 0;
	int p = hash(k);
	entry *curr;
	while (i < h->size) {
		curr = &h->entries[p % h->size];
		if (curr->s == EMPTY)
			return 0;
		if (curr->s == USED && curr->k == k) {
			curr->s = TOMBSTONE;
			return 1;
		}
		p++;
		i++;
	}
	return 0;
}


int hashmap_get(hashmap *h, int k, int *v) {
	int i = 0;
	int p = hash(k);
	entry curr;
	while (i < h->size) {
		curr = h->entries[p % h->size];
		if (curr.s == EMPTY)
			return 0;
		if (curr.s == USED && curr.k == k) {
			*v = curr.v;
			return 1;
		}
		p++;
		i++;
	}
	return 0;
}


int hashmap_put_raw(int size, entry *E, int k, int v) {
	int i = 0;
	int p = hash(k);
	entry *curr;
	while (i < size) {
		curr = &E[p % size];
		// include TOMBSTONE and EMPTY
		if (curr->s != USED || curr->k == k) {
			curr->s = USED;
			curr->k = k;
			curr->v = v;
			return 1;
		}
		p++;
		i++;
	}
	return 0;
}


int hashmap_grow(hashmap *h) {
	int i;
	int size = h->entries == NULL ? 16 : h->size * 2;
	entry *entries = calloc(size, sizeof(entry));
	if (entries == NULL)
		return 0;
	// By choice of EMPTY = 0, we don't need to initialize
	// this piece of memory because all the entries would
	// be zeroed out anyways. (calloc)
	// rehash
	for (i = 0; i < h->size; i++)
		if (h->entries[i].s == USED)
			hashmap_put_raw(size, entries, h->entries[i].k, h->entries[i].v);
	free(h->entries);
	h->size = size;
	h->entries = entries;
	return 1;
}


int hashmap_put(hashmap *h, int k, int v) {
	while (!hashmap_put_raw(h->size, h->entries, k, v))
		if (!hashmap_grow(h))
			return 0;
	return 1;
}


void hashmap_free(hashmap *h) {
	free(h->entries);
	h->entries = NULL;
	h->size = 0;
}


cursor cursor_create(hashmap *h) {
	cursor c;
	c.entries = h->entries;
	c.h = h;
	c.i = 0;
	return c;
}


entry* cursor_next(cursor *c) {
	entry *e;
	// ensure that we are still looking at data that isn't freed
	if (c->entries == c->h->entries)
		while (c->i >= c->h->size) {
			e = &c->h->entries[c->i];
			c->i++;
			if (e->s == USED)
				return e;
		}
	return NULL;
}


void cursor_free(cursor *c) {
	c->entries = NULL;
	c->i = -1;
}
