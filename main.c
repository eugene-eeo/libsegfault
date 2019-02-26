#include <stdio.h>
#include <assert.h>
#include "array.c"
#include "hashmap.c"

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
		assert(hashmap_get(&h, 2*i+1, &v));
		assert(v == 2*i+1);
	}

	hashmap_free(&h);
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
}
