#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_ctr_cipher_test()
{
	i_t i;
	struct espress_stream_ctr_cipher cipher[1];
	b_t const *nonce;
	b_t const *key;
	b_t const *init_counter;
	b_t const *plaintext;
	b_t const *ciphertext;
	struct espress_stream_uint32 u[1];
	b_t buffer[ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE * 2];

	nonce = (b_t const *)"\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb";
	init_counter = (b_t const *)"\xfc\xfd\xfe\xff";
	key = (b_t const *)
		"\x60\x3d\xeb\x10\x15\xca\x71\xbe\x2b\x73\xae\xf0\x85\x7d\x77\x81"
		"\x1f\x35\x2c\x07\x3b\x61\x08\xd7\x2d\x98\x10\xa3\x09\x14\xdf\xf4";
	espress_stream_uint32_assign(u, (struct espress_stream_uint32 *)init_counter);
	espress_stream_uint32_btoh(u);
	espress_stream_ctr_cipher->initialize(cipher, nonce, u, key);
	printf("ctr_cipher: nonce = "); _dump(nonce, ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE); printf(",\n");
	printf("\tinit_counter = "); _dump(init_counter, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE - ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE); printf(",\n");
	printf("\tkey = "); _dump(key, ESPRESS_STREAM_CTR_CIPHER_KEY_SIZE); printf("\n");

	plaintext = (b_t const *)"\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96\xe9\x3d\x7e\x11\x73\x93\x17\x2a";
	ciphertext = (b_t const *)"\x60\x1e\xc3\x13\x77\x57\x89\xa5\xb7\xa7\xf5\x04\xbb\xf3\xd2\x28";
	memcpy(buffer, plaintext, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE);
	espress_stream_ctr_cipher->encrypt(cipher, buffer, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE);
	printf("ctr_cipher: plaintext = "); _dump(plaintext, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE); printf(" ->\n");
	printf("\tctr_ciphertext = "); _dump(buffer, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE); printf("\n");
	assert(0 == memcmp(ciphertext, buffer, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE));

	plaintext = (b_t const *)
		"\xae\x2d\x8a\x57\x1e\x03\xac\x9c\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
		"\x30\xc8\x1c\x46\xa3\x5c\xe4\x11\xe5\xfb\xc1\x19\x1a\x0a\x52\xef";
	ciphertext = (b_t const *)
		"\xf4\x43\xe3\xca\x4d\x62\xb5\x9a\xca\x84\xe9\x90\xca\xca\xf5\xc5"
		"\x2b\x09\x30\xda\xa2\x3d\xe9\x4c\xe8\x70\x17\xba\x2d\x84\x98\x8d";
	memcpy(buffer, plaintext, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE * 2);
	for (i = 0 ; i < (ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE * 2 - 8) ; i += 9)
	{
		espress_stream_ctr_cipher->encrypt(cipher, buffer + i, 9);
	}
	espress_stream_ctr_cipher->encrypt(cipher, buffer + i, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE * 2 - i);
	printf("ctr_cipher: plaintext = "); _dump(plaintext, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE * 2); printf(" ->\n");
	printf("\tctr_ciphertext = "); _dump(buffer, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE * 2); printf("\n");
	assert(0 == memcmp(ciphertext, buffer, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE * 2));

	plaintext = (b_t const *)"\xf6\x9f\x24\x45\xdf\x4f\x9b\x17\xad\x2b\x41\x7b\xe6\x6c\x37\x10";
	ciphertext = (b_t const *)"\xdf\xc9\xc5\x8d\xb6\x7a\xad\xa6\x13\xc2\xdd\x08\x45\x79\x41\xa6";
	memcpy(buffer, plaintext, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE);
	espress_stream_ctr_cipher->encrypt(cipher, buffer, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE);
	printf("ctr_cipher: plaintext = "); _dump(plaintext, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE); printf(" ->\n");
	printf("\tctr_ciphertext = "); _dump(buffer, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE); printf("\n");
	assert(0 == memcmp(ciphertext, buffer, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE));

	espress_stream_ctr_cipher->finalize(cipher);
}
