#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/hash.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_hash_test()
{
#	if 4 <= I_T_WIDTH
		i_t i;
#	endif
	struct espress_stream_hash h[1];
	b_t const *message;
	u_t message_size;
	b_t const *digest;
	b_t d[ESPRESS_STREAM_HASH_DIGEST_SIZE];

	message = (b_t const *)"";
	message_size = 0;
	digest = (b_t const *)
		"\xe3\xb0\xc4\x42\x98\xfc\x1c\x14"
		"\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24"
		"\x27\xae\x41\xe4\x64\x9b\x93\x4c"
		"\xa4\x95\x99\x1b\x78\x52\xb8\x55";
	espress_stream_hash->initialize(h);
	espress_stream_hash->update(h, message, message_size);
	espress_stream_hash->finalize(h, d);
	printf("hash: (null) -> "); _dump(d, ESPRESS_STREAM_HASH_DIGEST_SIZE); printf("\n");
	assert(0 == memcmp(digest, d, ESPRESS_STREAM_HASH_DIGEST_SIZE));

	message = (b_t const *)"abc";
	message_size = 3;
	digest = (b_t const *)
		"\xba\x78\x16\xbf\x8f\x01\xcf\xea"
		"\x41\x41\x40\xde\x5d\xae\x22\x23"
		"\xb0\x03\x61\xa3\x96\x17\x7a\x9c"
		"\xb4\x10\xff\x61\xf2\x00\x15\xad";
	espress_stream_hash->initialize(h);
	espress_stream_hash->update(h, message, message_size);
	espress_stream_hash->finalize(h, d);
	printf("hash: \"%s\" -> ", message); _dump(d, ESPRESS_STREAM_HASH_DIGEST_SIZE); printf("\n");
	assert(0 == memcmp(digest, d, ESPRESS_STREAM_HASH_DIGEST_SIZE));

	message = (b_t const *)"The quick brown fox jumps over the lazy dog";
	message_size = 43;
	digest = (b_t const *)
		"\xd7\xa8\xfb\xb3\x07\xd7\x80\x94"
		"\x69\xca\x9a\xbc\xb0\x08\x2e\x4f"
		"\x8d\x56\x51\xe4\x6d\x3c\xdb\x76"
		"\x2d\x02\xd0\xbf\x37\xc9\xe5\x92";
	espress_stream_hash->initialize(h);
	espress_stream_hash->update(h, message, message_size);
	espress_stream_hash->finalize(h, d);
	printf("hash: \"%s\" -> ", message); _dump(d, ESPRESS_STREAM_HASH_DIGEST_SIZE); printf("\n");
	assert(0 == memcmp(digest, d, ESPRESS_STREAM_HASH_DIGEST_SIZE));

#	if 4 <= I_T_WIDTH
		message = (b_t const *)"a";
		message_size = 1;
		digest = (b_t const *)
			"\xcd\xc7\x6e\x5c\x99\x14\xfb\x92"
			"\x81\xa1\xc7\xe2\x84\xd7\x3e\x67"
			"\xf1\x80\x9a\x48\xa4\x97\x20\x0e"
			"\x04\x6d\x39\xcc\xc7\x11\x2c\xd0";
		espress_stream_hash->initialize(h);
		for (i = 0 ; i < 1000000 ; i++)
		{
			espress_stream_hash->update(h, message, message_size);
		}
		espress_stream_hash->finalize(h, d);
		printf("hash: (\"%s\" repeated 1,000,000 times) -> ", message); _dump(d, ESPRESS_STREAM_HASH_DIGEST_SIZE); printf("\n");
		assert(0 == memcmp(digest, d, ESPRESS_STREAM_HASH_DIGEST_SIZE));
#	endif
}
