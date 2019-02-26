#include <stdlib.h>
#include <stdio.h>


typedef enum {
	EMPTY = 0,
	USED,
	TOMBSTONE,
} state;


typedef struct {
	int state;
	int k;
	int v;
} entry;


typedef struct {
	entry *entries;
	int size;
} hashmap;


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
	entry curr;
	while (i < h->size) {
		curr = h->entries[p % h->size];
		if (curr.state == EMPTY)
			return 0;
		if (curr.state == USED && curr.k == k) {
			curr.state = TOMBSTONE;
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
		if (curr.state == EMPTY)
			return 0;
		if (curr.k == k) {
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
		if (curr->state != USED || curr->k == k) {
			curr->state = USED;
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
	int size = h->size * 2;
	if (h->entries == NULL) {
		size = 16;
	}
	entry *entries = calloc(size, sizeof(entry));
	if (entries == NULL)
		return 0;
	// rehash
	for (i = 0; i < h->size; i++)
		if (h->entries[i].state == USED)
			hashmap_put_raw(size, entries, h->entries[i].k, h->entries[i].v);
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
