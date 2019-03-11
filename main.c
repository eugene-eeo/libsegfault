#include <stdio.h>
#include <assert.h>
#include "array.c"
#include "hashmap.c"
#include "disjoint_set.c"
#include "rc4.c"
#include <string.h>

void test_rc4() {
	char *k = "Key";
	char *m = "Plaintext";
	char *c = malloc(strlen(m) + 1);
	char *b = malloc(strlen(m) + 1);

	assert(c != NULL);
	assert(b != NULL);

	rc4_encrypt(k, m, c);
	rc4_decrypt(k, c, b);

	assert(c[0] == '\xBB');
	assert(c[1] == '\xF3');
	assert(c[2] == '\x16');
	assert(c[3] == '\xE8');
	assert(c[4] == '\xD9');
	assert(c[5] == '\x40');
	assert(c[6] == '\xAF');
	assert(c[7] == '\x0A');
	assert(c[8] == '\xD3');

	assert(strlen(c) == strlen(m));
	assert(strcmp(b, m) == 0);
}

void test_dset() {
	dset *a = dset_new();
	dset *b = dset_new();
	dset *c = dset_new();
	dset *d = dset_new();

	assert(dset_find(a) == a);
	assert(dset_find(b) == b);
	assert(dset_find(c) == c);
	assert(dset_find(d) == d);

	dset_union(a, b);
	dset_union(c, d);

	assert(dset_find(a) == dset_find(b));
	assert(dset_find(c) == dset_find(d));
	assert(dset_find(a) != dset_find(c));

	dset_union(a, c);
	assert(dset_find(a) == dset_find(b));
	assert(dset_find(a) == dset_find(c));
	assert(dset_find(a) == dset_find(d));

	free(a);
	free(b);
	free(c);
	free(d);
}

void test_hashmap() {
	hashmap h = hashmap_new();
	for (int i = 0; i < 50; i++) {
		assert(hashmap_put(&h, i, i));
	}

	for (int i = 0; i < 50; i++) {
		int v;
		assert(hashmap_get(&h, i, &v));
		assert(v == i);
	}

	for (int i = 0; i < 25; i++) {
		assert(hashmap_delete(&h, 2*i));
	}

	for (int i = 0; i < 25; i++) {
		int v;
		// check that we cannot get a deleted item
		assert(!hashmap_get(&h, 2*i, &v));
		assert(hashmap_get(&h, 2*i+1, &v));
		assert(v == 2*i+1);
	}

	cursor c = cursor_create(&h);
	entry *e;
	while ((e = cursor_next(&c)) != NULL)
		assert(e->k % 2 == 1);

	hashmap_free(&h);
	assert(cursor_next(&c) == NULL);
}

void test_array() {
	array a;
	a = array_new();
	assert(array_resize(&a, 10));

	int v;

	for (int i = 0; i < 10; i++) {
		assert(array_get(&a, i, &v));
		assert(v == 0);
	}

	assert(array_resize(&a, 5));
	assert(!array_get(&a, 6, &v));

	assert(array_resize(&a, 10));
	assert(array_get(&a, 6, &v));
	assert(v == 0);

	for (int i = 0; i < 10; i++) {
		assert(array_set(&a, i, i));
		assert(array_get(&a, i, &v));
		assert(v == i);
	}

	assert(array_set(&a, 100, 69));
	assert(array_get(&a, 100, &v));
	assert(v == 69);

	array_free(&a);
}

int main() {
	test_array();
	test_hashmap();
	test_dset();
	test_rc4();
}
