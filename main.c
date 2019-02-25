#include <stdio.h>
#include <assert.h>
#include "array.c"

int main() {
	array a;
	a = array_new();
	array_resize(&a, 10);

	int v;

	for (int i = 0; i < 10; i++) {
		assert(array_get(&a, i, &v));
		assert(v == 0);
	}

	array_resize(&a, 5);
	assert(!array_get(&a, 6, &v));

	array_resize(&a, 10);
	assert(array_get(&a, 6, &v));
	assert(v == 0);

	for (int i = 0; i < 10; i++) {
		array_set(&a, i, i);
		assert(array_get(&a, i, &v));
		assert(v == i);
	}

	array_free(&a);
}
