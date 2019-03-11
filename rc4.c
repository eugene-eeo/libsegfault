#include <string.h>

void rc4_swap(unsigned char *a, unsigned char *b) {
	unsigned char t = *a;
	*a = *b;
	*b = t;
}

void rc4_initialize(unsigned char *S, char *key) {
	// initialize
	int i = 0;
	for (i = 0; i < 256; i++) {
		S[i] = i;
	}
	// shuffle
	int j = 0;
	int n = strlen(key);
	for (i = 0; i < 256; i++) {
		j += S[i];
		j += key[i % n];
		j %= 256;
		rc4_swap(&S[i], &S[j]);
	}
}

void rc4_encrypt(char *key, char *message, char *dst) {
	unsigned char S[256];
	rc4_initialize(S, key);
	int i = 0;
	int j = 0;
	int p = 0;
	int n = strlen(message);
	int k;
	for (p = 0; p < n; p++) {
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;

		rc4_swap(&S[i], &S[j]);
		k = S[(S[i] + S[j]) % 256];

		dst[p] = message[p] ^ k;
	}
	// add null character at the end
	dst[n] = 0;
}

void rc4_decrypt(char *key, char *cipher, char *dst) {
	return rc4_encrypt(key, cipher, dst);
}
