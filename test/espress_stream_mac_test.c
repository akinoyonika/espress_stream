#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_mac_test()
{
	struct espress_stream_mac mac[1];
	b_t const *key;
	u_t key_size;
	b_t const *message;
	u_t message_size;
	b_t const *mac_body;
	b_t buffer[ESPRESS_STREAM_MAC_MAC_SIZE];

	key = (b_t const *)"";
	key_size = 0;
	mac_body = (b_t const *)
		"\xb6\x13\x67\x9a\x08\x14\xd9\xec\x77\x2f\x95\xd7\x78\xc3\x5f\xc5"
		"\xff\x16\x97\xc4\x93\x71\x56\x53\xc6\xc7\x12\x14\x42\x92\xc5\xad";
	espress_stream_mac->initialize(mac, key, key_size);
	espress_stream_mac->finalize(mac, buffer);
	printf("mac: key = (null), message = (null) ->\n");
	printf("\t"); _dump(buffer, ESPRESS_STREAM_MAC_MAC_SIZE); printf("\n");
	assert(0 == memcmp(mac_body, buffer, ESPRESS_STREAM_MAC_MAC_SIZE));

	key = (b_t const *)
		"\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
		"\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b";
	key_size = 32;
	message = (b_t const *)"\x48\x69\x20\x54\x68\x65\x72\x65";
	message_size = 8;
	mac_body = (b_t const *)"\x19\x8a\x60\x7e\xb4\x4b\xfb\xc6\x99\x03\xa0\xf1\xcf\x2b\xbd\xc5";
	espress_stream_mac->initialize(mac, key, key_size);
	espress_stream_mac->update(mac, message, message_size);
	espress_stream_mac->finalize(mac, buffer);
	printf("mac: key = "); _dump(key, key_size); printf(",\n");
	printf("\tmessage = "); _dump(message, message_size); printf(" ->\n");
	printf("\tmac = "); _dump(buffer, ESPRESS_STREAM_MAC_MAC_SIZE); printf("\n");
	assert(0 == memcmp(mac_body, buffer, ESPRESS_STREAM_MAC_MAC_SIZE / 2));

	key = (b_t const *)"abc";
	key_size = 3;
	message = (b_t const *)"The quick brown fox jumps over the lazy dog";
	message_size = 43;
	mac_body = (b_t const *)
		"\xd8\x21\x59\xdf\x72\x1f\x31\x19\x40\x34\x92\xe6\xf0\x41\xdf\x30"
		"\x82\xac\x04\x8d\xc5\xee\x0f\xa0\xca\xe5\x1d\xf9\xbc\xf7\xf1\x0e";
	espress_stream_mac->initialize(mac, key, key_size);
	espress_stream_mac->update(mac, message, message_size);
	espress_stream_mac->finalize(mac, buffer);
	printf("mac: key = \"%s\", message = \"%s\" ->\n", key, message);
	printf("\t"); _dump(buffer, ESPRESS_STREAM_MAC_MAC_SIZE); printf("\n");
	assert(0 == memcmp(mac_body, buffer, ESPRESS_STREAM_MAC_MAC_SIZE));

	key = (b_t const *)"The quick brown fox jumps over the lazy dog";
	key_size = 43;
	message = (b_t const *)"The quick brown fox jumps over the lazy dog";
	message_size = 43;
	mac_body = (b_t const *)
		"\x05\xef\x8d\x26\x32\xd4\x14\x0d\xb7\x30\x87\x8f\xfb\x03\xa0\xbd"
		"\x9b\x32\xde\x06\xfb\x74\xdf\x04\x71\xbd\xe7\x77\xcb\xa1\xef\xf7";
	espress_stream_mac->initialize(mac, key, key_size);
	espress_stream_mac->update(mac, message, message_size);
	espress_stream_mac->finalize(mac, buffer);
	printf("mac: key = \"%s\", message = \"%s\" ->\n", key, message);
	printf("\t"); _dump(buffer, ESPRESS_STREAM_MAC_MAC_SIZE); printf("\n");
	assert(0 == memcmp(mac_body, buffer, ESPRESS_STREAM_MAC_MAC_SIZE));
}
