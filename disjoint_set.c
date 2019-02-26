#include <stdlib.h>

typedef struct dset {
	struct dset *parent;
	int rank;
} dset;


dset* dset_new() {
	dset* s = malloc(sizeof(dset));
	if (s != NULL) {
		s->parent = s;
		s->rank = 0;
	}
	return s;
}


dset* dset_find(dset *x) {
	dset *next;
	while (x->parent != x) {
		next = x->parent;
		x->parent = next->parent;
		x = next;
	}
	return x;
}


void dset_union(dset *x, dset *y) {
	dset *x_root = dset_find(x);
	dset *y_root = dset_find(y);
	dset *tmp;
	if (x_root == y_root)
		return;
	// make sure that x_root->rank >= y_root->rank
	if (x_root->rank < y_root->rank) {
		tmp = x_root;
		x_root = y_root;
		y_root = tmp;
	}
	y_root->parent = x_root;
	if (x_root->rank == y_root->rank)
		x_root->rank++;
}
