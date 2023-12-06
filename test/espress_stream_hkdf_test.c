#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_hkdf_test()
{
	b_t const *salt;
	u_t salt_size;
	b_t const *ikm;
	u_t ikm_size;
	b_t const *prk;
	b_t const *info;
	u_t info_size;
	b_t const *t1;
	b_t buffer[ESPRESS_STREAM_HKDF_HASH_SIZE];

	salt = (b_t const *)"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c";
	salt_size = 13;
	ikm = (b_t const *)"\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b";
	ikm_size = 22;
	prk = (b_t const *)"\x07\x77\x09\x36\x2c\x2e\x32\xdf\x0d\xdc\x3f\x0d\xc4\x7b\xba\x63\x90\xb6\xc7\x3b\xb5\x0f\x9c\x31\x22\xec\x84\x4a\xd7\xc2\xb3\xe5";
	info = (b_t const *)"\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9";
	info_size = 10;
	t1 = (b_t const *)"\x3c\xb2\x5f\x25\xfa\xac\xd5\x7a\x90\x43\x4f\x64\xd0\x36\x2f\x2a\x2d\x2d\x0a\x90\xcf\x1a\x5a\x4c\x5d\xb0\x2d\x56\xec\xc4\xc5\xbf";
	espress_stream_hkdf->extract(buffer, salt, salt_size, ikm, ikm_size);
	printf("hkdf: salt = "); _dump(salt, salt_size); printf(",\n");
	printf("\tikm = "); _dump(ikm, ikm_size); printf(",\n");
	printf("\tinfo = f0f1f2f3f4f5f6f7f8f9 ->\n");
	printf("\tprk = "); _dump(buffer, ESPRESS_STREAM_HKDF_HASH_SIZE); printf(",\n");
	assert(0 == memcmp(prk, buffer, ESPRESS_STREAM_HKDF_HASH_SIZE));
	memset(buffer, 0, ESPRESS_STREAM_HKDF_HASH_SIZE);
	espress_stream_hkdf->expand(buffer, 0, prk, info, info_size, 1);
	printf("\tt[1] = "); _dump(buffer, ESPRESS_STREAM_HKDF_HASH_SIZE); printf("\n");
	assert(0 == memcmp(t1, buffer, ESPRESS_STREAM_HKDF_HASH_SIZE));
}
