#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/ec.h"
#include "espress_stream/key_ex.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_key_ex_test()
{
	b_t const *alices_private_key;
	b_t const *alices_public_key;
	b_t const *bobs_private_key;
	b_t const *bobs_public_key;
	b_t const *shared_key;
	b_t b[ESPRESS_STREAM_KEY_EX_KEY_SIZE];

	alices_private_key = (b_t const *)
		"\x77\x07\x6d\x0a\x73\x18\xa5\x7d\x3c\x16\xc1\x72\x51\xb2\x66\x45"
		"\xdf\x4c\x2f\x87\xeb\xc0\x99\x2a\xb1\x77\xfb\xa5\x1d\xb9\x2c\x2a";
	alices_public_key = (b_t const *)
		"\x85\x20\xf0\x09\x89\x30\xa7\x54\x74\x8b\x7d\xdc\xb4\x3e\xf7\x5a"
		"\x0d\xbf\x3a\x0d\x26\x38\x1a\xf4\xeb\xa4\xa9\x8e\xaa\x9b\x4e\x6a";
	bobs_private_key = (b_t const *)
		"\x5d\xab\x08\x7e\x62\x4a\x8a\x4b\x79\xe1\x7f\x8b\x83\x80\x0e\xe6"
		"\x6f\x3b\xb1\x29\x26\x18\xb6\xfd\x1c\x2f\x8b\x27\xff\x88\xe0\xeb";
	bobs_public_key = (b_t const *)
		"\xde\x9e\xdb\x7d\x7b\x7d\xc1\xb4\xd3\x5b\x61\xc2\xec\xe4\x35\x37"
		"\x3f\x83\x43\xc8\x5b\x78\x67\x4d\xad\xfc\x7e\x14\x6f\x88\x2b\x4f";
	shared_key = (b_t const *)
		"\x4a\x5d\x9d\x5b\xa4\xce\x2d\xe1\x72\x8e\x3b\xf4\x80\x35\x0f\x25"
		"\xe0\x7e\x21\xc9\x47\xd1\x9e\x33\x76\xf0\x9b\x3c\x1e\x16\x17\x42";
	printf("key_ex: alices_private_key = "); _dump(alices_private_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE); printf("\n");
	espress_stream_key_ex->get_public_key(b, alices_private_key);
	printf("key_ex: alices_public_key = "); _dump(b, ESPRESS_STREAM_KEY_EX_KEY_SIZE); printf("\n");
	assert(0 == memcmp(alices_public_key, b, ESPRESS_STREAM_KEY_EX_KEY_SIZE));
	printf("key_ex: bobs_private_key = "); _dump(bobs_private_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE); printf("\n");
	espress_stream_key_ex->get_public_key(b, bobs_private_key);
	printf("key_ex: bobs_public_key = "); _dump(b, ESPRESS_STREAM_KEY_EX_KEY_SIZE); printf("\n");
	assert(0 == memcmp(bobs_public_key, b, ESPRESS_STREAM_KEY_EX_KEY_SIZE));
	espress_stream_key_ex->get_shared_key(b, alices_private_key, bobs_public_key);
	printf("key_ex: alices_shared_key = "); _dump(b, ESPRESS_STREAM_KEY_EX_KEY_SIZE); printf("\n");
	assert(0 == memcmp(shared_key, b, ESPRESS_STREAM_KEY_EX_KEY_SIZE));
	espress_stream_key_ex->get_shared_key(b, bobs_private_key, alices_public_key);
	printf("key_ex: bobs_shared_key = "); _dump(b, ESPRESS_STREAM_KEY_EX_KEY_SIZE); printf("\n");
	assert(0 == memcmp(shared_key, b, ESPRESS_STREAM_KEY_EX_KEY_SIZE));
}
