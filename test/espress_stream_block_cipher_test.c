#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/block_cipher.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_block_cipher_test()
{
	struct espress_stream_block_cipher cipher[1];
	b_t const *key;
	b_t block[ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE];
	b_t const *encrypted;

	key = (b_t const *)
		"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
		"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f";
	memcpy(block, "\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff", ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE);
	encrypted = (b_t const *)"\x8e\xa2\xb7\xca\x51\x67\x45\xbf\xea\xfc\x49\x90\x4b\x49\x60\x89";
	printf("block_cipher: key = "); _dump(key, ESPRESS_STREAM_BLOCK_CIPHER_KEY_SIZE); printf(",\n");
	printf("\tplain = "); _dump(block, ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE); printf(" ->\n");
	espress_stream_block_cipher->init(cipher, key);
	espress_stream_block_cipher->encrypt(cipher, block);
	printf("\tcipher = "); _dump(block, ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE); printf("\n");
	assert(0 == memcmp(encrypted, block, ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE));
	espress_stream_block_cipher->final(cipher);

	key = (b_t const *)
		"\x60\x3D\xEB\x10\x15\xCA\x71\xBE\x2B\x73\xAE\xF0\x85\x7D\x77\x81"
		"\x1F\x35\x2C\x07\x3B\x61\x08\xD7\x2D\x98\x10\xA3\x09\x14\xDF\xF4";
	memcpy(block, "\x6B\xC1\xBE\xE2\x2E\x40\x9F\x96\xE9\x3D\x7E\x11\x73\x93\x17\x2A", ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE);
	encrypted = (b_t const *)"\xF3\xEE\xD1\xBD\xB5\xD2\xA0\x3C\x06\x4B\x5A\x7E\x3D\xB1\x81\xF8";
	printf("block_cipher: key = "); _dump(key, ESPRESS_STREAM_BLOCK_CIPHER_KEY_SIZE); printf(",\n");
	printf("\tplain = "); _dump(block, ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE); printf(" ->\n");
	espress_stream_block_cipher->init(cipher, key);
	espress_stream_block_cipher->encrypt(cipher, block);
	printf("\tcipher = "); _dump(block, ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE); printf("\n");
	assert(0 == memcmp(encrypted, block, ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE));
	espress_stream_block_cipher->final(cipher);
}
