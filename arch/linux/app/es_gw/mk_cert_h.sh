#!/bin/bash

cat << "EOT" > mk_cert_h.c
#include <stdio.h>
#include <string.h>
#include "cipher_api.h"

int main()
{
	int i;
	unsigned char secret_key[CIPHER_ED25519_SECRET_KEY_SIZE];
	unsigned char public_key[CIPHER_ED25519_PUBLIC_KEY_SIZE];

	cipher_ed25519_get_key(secret_key, public_key);

	printf("#define SECRET_KEY {\\\n");
	for (i = 0 ; i < CIPHER_ED25519_SECRET_KEY_SIZE ; i++) {
		printf("%s0x%02x,%s", (0 == (i % 8)) ? "\t" : "", secret_key[i], (7 == (i % 8)) ? "\\\n" : " ");
	}
	printf("}\n");

	printf("#define PUBLIC_KEY {\\\n");
	for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
		printf("%s0x%02x,%s", (0 == (i % 8)) ? "\t" : "", public_key[i], (7 == (i % 8)) ? "\\\n" : " ");
	}
	printf("}\n");

	return 0;
}
EOT
gcc -O2 -Icipher/include -Wall mk_cert_h.c cipher/src/*.c
./a.out > src/cert.h
rm -f src/mk_cert_h.o cipher/src/*.o a.out mk_cert_h.c
