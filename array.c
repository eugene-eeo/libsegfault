#include <stdlib.h>


typedef struct {
	unsigned long size;
	int *N;
} array;


/*
 * Returns a new array
 */
array array_new() {
	array x = {0, NULL};
	return x;
}

/*
 * Free array.
 */
void array_free(array *a) {
	free(a->N);
	a->size = 0;
	a->N = NULL;
}

/*
 * Resizes array to size new_size.
 */
int array_resize(array *a, unsigned long new_size) {
	if (a->N == NULL) {
		a->N = calloc(new_size, sizeof(int));
		if (a->N == NULL)
			return 0;
	} else {
		// realloc and fill memory with zeroes
		int *ptr = realloc(a->N, new_size * sizeof(int));
		if (ptr == NULL)
			return 0;
		a->N = ptr;
		if (new_size > a->size)
			for (unsigned long i = 0; i < new_size - a->size; i++)
				ptr[a->size + i] = 0;
	}
	a->size = new_size;
	return 1;
}

/*
 * Sets a[idx] = val and return 1 if possible, resizing if
 * neccessary. Return 0 => resize failed.
 */
int array_set(array *a, int idx, int val) {
	while ((unsigned long) idx >= a->size)
		if (!array_resize(a, a->size*2))
			return 0;
	a->N[idx] = val;
	return 1;
}

/*
 * Sets *dst = a[idx] and return 1 if possible, otherwise do
 * nothing to dst and return 0.
 */
int array_get(array *a, int idx, int *dst) {
	if ((unsigned long) idx >= a->size)
		return 0;
	*dst = a->N[idx];
	return 1;
}
