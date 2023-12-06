#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/ec.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_ec_test()
{
	b_t u[ESPRESS_STREAM_EC_SIZE];
	b_t k[ESPRESS_STREAM_EC_SIZE];
	b_t const *r;

	memcpy(u,
		"\x09\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
		ESPRESS_STREAM_EC_SIZE);
	memcpy(k,
		"\x67\xc6\x69\x73\x51\xff\x4a\xec\x29\xcd\xba\xab\xf2\xfb\xe3\x46"
		"\x7c\xc2\x54\xf8\x1b\xe8\xe7\x8d\x76\x5a\x2e\x63\x33\x9f\xc9\x9a",
		ESPRESS_STREAM_EC_SIZE);
	r = (b_t const *)
		"\x63\xa8\xfd\x14\xa5\x7b\x6e\xcc\xea\x4e\xcd\x7a\xaf\xd3\x1d\xe6"
		"\x1d\x79\x6a\x69\x6e\x8f\x16\x27\x8a\xbe\x1c\x9f\xea\x23\x7e\x05";
	printf("ec: u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	printf("ec: k = "); _dump(k, ESPRESS_STREAM_EC_SIZE); printf("\n");
	espress_stream_ec->prepare_key(k);
	espress_stream_ec->smul(u, k);
	printf("ec: k * u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	assert(0 == memcmp(r, u, ESPRESS_STREAM_EC_SIZE));

	memcpy(k,
		"\x66\x32\x0d\xb7\x31\x58\xa3\x5a\x25\x5d\x05\x17\x58\xe9\x5e\xd4"
		"\xab\xb2\xcd\xc6\x9b\xb4\x54\x11\x0e\x82\x74\x41\x21\x3d\xdc\x87",
		ESPRESS_STREAM_EC_SIZE);
	r = (b_t const *)
		"\xab\x94\x34\xd5\x84\x11\x51\xb8\xd6\xb8\x68\x94\xf0\x99\x8b\x73"
		"\xef\x0a\x9f\x7a\x89\x6a\x77\x58\x6b\x01\x36\x8e\xd9\x0c\x45\x61";
	printf("ec: u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	printf("ec: k = "); _dump(k, ESPRESS_STREAM_EC_SIZE); printf("\n");
	espress_stream_ec->prepare_key(k);
	espress_stream_ec->smul(u, k);
	printf("ec: k * u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	assert(0 == memcmp(r, u, ESPRESS_STREAM_EC_SIZE));

	memcpy(u,
		"\x09\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
		ESPRESS_STREAM_EC_SIZE);
	memcpy(k,
		"\xe2\xa0\x7f\xf8\xe3\x47\x69\xe3\x11\xb6\x98\xb9\x41\x9f\x18\x22"
		"\xa8\x4b\xc8\xfd\xa2\x04\x1a\x90\xf4\x49\xfe\x15\x4b\x48\x96\x2d",
		ESPRESS_STREAM_EC_SIZE);
	r = (b_t const *)
		"\xfd\x15\xd9\xf5\x34\xac\xe7\x91\x01\xac\x9f\xef\x3d\x20\x49\xef"
		"\xa3\x2d\xb8\xda\x16\x89\xbf\xd7\x68\xd3\x37\xd4\x51\x30\x9c\x6a";
	printf("ec: u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	printf("ec: k = "); _dump(k, ESPRESS_STREAM_EC_SIZE); printf("\n");
	espress_stream_ec->prepare_key(k);
	espress_stream_ec->smul(u, k);
	printf("ec: k * u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	assert(0 == memcmp(r, u, ESPRESS_STREAM_EC_SIZE));

	memcpy(k,
		"\xe8\x15\x25\xcb\x5c\x8f\xae\x6d\x45\x46\x27\x86\xe5\x3f\xa9\x8d"
		"\x8a\x71\x8a\x2c\x75\xa4\xbc\x6a\xee\xba\x7f\x39\x02\x15\x67\xea",
		ESPRESS_STREAM_EC_SIZE);
	r = (b_t const *)
		"\xe5\xcb\x46\x91\x2c\x5f\x9c\xc4\x0d\xe1\xa7\xd0\xba\x6d\xd9\xa2"
		"\x57\x0f\xee\x27\xcd\x1c\x44\x24\x3e\xef\xd4\xba\x34\xb4\x1f\x3a";
	printf("ec: u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	printf("ec: k = "); _dump(k, ESPRESS_STREAM_EC_SIZE); printf("\n");
	espress_stream_ec->prepare_key(k);
	espress_stream_ec->smul(u, k);
	printf("ec: k * u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	assert(0 == memcmp(r, u, ESPRESS_STREAM_EC_SIZE));

	memcpy(u,
		"\xe6\xdb\x68\x67\x58\x30\x30\xdb\x35\x94\xc1\xa4\x24\xb1\x5f\x7c"
		"\x72\x66\x24\xec\x26\xb3\x35\x3b\x10\xa9\x03\xa6\xd0\xab\x1c\x4c",
		ESPRESS_STREAM_EC_SIZE);
	memcpy(k,
		"\xa5\x46\xe3\x6b\xf0\x52\x7c\x9d\x3b\x16\x15\x4b\x82\x46\x5e\xdd"
		"\x62\x14\x4c\x0a\xc1\xfc\x5a\x18\x50\x6a\x22\x44\xba\x44\x9a\xc4",
		ESPRESS_STREAM_EC_SIZE);
	r = (b_t const *)
		"\xc3\xda\x55\x37\x9d\xe9\xc6\x90\x8e\x94\xea\x4d\xf2\x8d\x08\x4f"
		"\x32\xec\xcf\x03\x49\x1c\x71\xf7\x54\xb4\x07\x55\x77\xa2\x85\x52";
	printf("ec: u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	printf("ec: k = "); _dump(k, ESPRESS_STREAM_EC_SIZE); printf("\n");
	espress_stream_ec->prepare_key(k);
	espress_stream_ec->smul(u, k);
	printf("ec: k * u = "); _dump(u, ESPRESS_STREAM_EC_SIZE); printf("\n");
	assert(0 == memcmp(r, u, ESPRESS_STREAM_EC_SIZE));
}
