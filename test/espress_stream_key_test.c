#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"
#include "espress_stream/key_exp.h"
#include "espress_stream/key.h"

#if ESPRESS_STREAM_IS_HOST_BYTE_ORDER_LITTLE
#define _O(w, n) (n)
#else
#define _O(w, n) ((1 << (w)) - 1 - (n))
#endif

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_key_test()
{
	i_t i;
	struct espress_stream_uint32 u[1];
	struct espress_stream_key key[1];
	struct espress_stream_uint32 counter_num[1];
	b_t mac_key[ESPRESS_STREAM_KEY_MAC_KEY_SIZE];
	b_t cipher_key[ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE];
	b_t curr_mac_key[ESPRESS_STREAM_KEY_MAC_KEY_SIZE];
	b_t curr_cipher_key[ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE];
	b_t prev_mac_key[ESPRESS_STREAM_KEY_MAC_KEY_SIZE];
	b_t prev_cipher_key[ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE];

	espress_stream_key->init(key, (b_t const *)"", 0, (b_t const *)"", 0);

	espress_stream_uint32_assign(counter_num, espress_stream_uint32_0);
	espress_stream_uint32_set(u, 0x00, 0x00, 0x00, ESPRESS_STREAM_KEY_HISTORY_SIZE);
	while (espress_stream_uint32_lt(counter_num, u))
	{
		i = espress_stream_key->get(key, mac_key, cipher_key, counter_num);
		assert(i);
		i = espress_stream_key->update(key, counter_num);
		printf("key: update %02x%02x%02x%02x ->\n",
			((b_t *)counter_num)[_O(2, 3)],
			((b_t *)counter_num)[_O(2, 2)],
			((b_t *)counter_num)[_O(2, 1)],
			((b_t *)counter_num)[_O(2, 0)]);
		printf("\tmac_key = "); _dump(mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE); printf("\n");
		printf("\tcipher_key = "); _dump(cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE); printf("\n");
		assert(i);
		espress_stream_uint32_add(counter_num, espress_stream_uint32_1);
		espress_stream_uint32_add(counter_num, espress_stream_uint32_1);
	}

	espress_stream_uint32_assign(counter_num, espress_stream_uint32_0);
	espress_stream_uint32_set(u, 0x00, 0x00, 0x00, ESPRESS_STREAM_KEY_HISTORY_SIZE);
	while (espress_stream_uint32_lt(counter_num, u))
	{
		i = espress_stream_key->get(key, mac_key, cipher_key, counter_num);
		printf("key: get %02x%02x%02x%02x -> %s\n",
			((b_t *)counter_num)[_O(2, 3)],
			((b_t *)counter_num)[_O(2, 2)],
			((b_t *)counter_num)[_O(2, 1)],
			((b_t *)counter_num)[_O(2, 0)],
			i ? "OK" : "NG");
		assert(!i);
		espress_stream_uint32_add(counter_num, espress_stream_uint32_1);
		i = espress_stream_key->get(key, mac_key, cipher_key, counter_num);
		printf("key: get %02x%02x%02x%02x ->\n",
			((b_t *)counter_num)[_O(2, 3)],
			((b_t *)counter_num)[_O(2, 2)],
			((b_t *)counter_num)[_O(2, 1)],
			((b_t *)counter_num)[_O(2, 0)]);
		printf("\tmac_key = "); _dump(mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE); printf("\n");
		printf("\tcipher_key = "); _dump(cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE); printf("\n");
		assert(i);
		espress_stream_uint32_add(counter_num, espress_stream_uint32_1);
	}

	memcpy(prev_mac_key, mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE);
	memcpy(prev_cipher_key, cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE);
	espress_stream_uint32_set(key->current, 0xff, 0xff, 0xff, 0xfe);
	espress_stream_uint32_set(counter_num, 0x00, 0x00, 0x00, 0x00);
	i = espress_stream_key->get(key, mac_key, cipher_key, counter_num);
	assert(i &&
		(0 != memcmp(prev_mac_key, mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE)) &&
		(0 != memcmp(prev_cipher_key, cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE)));
	i = espress_stream_key->update(key, counter_num);
	printf("key: update %02x%02x%02x%02x ->\n",
		((b_t *)counter_num)[_O(2, 3)],
		((b_t *)counter_num)[_O(2, 2)],
		((b_t *)counter_num)[_O(2, 1)],
		((b_t *)counter_num)[_O(2, 0)]);
	printf("\tmac_key = "); _dump(mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE); printf("\n");
	printf("\tcipher_key = "); _dump(cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE); printf("\n");
	assert(i);
	memcpy(curr_mac_key, mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE);
	memcpy(curr_cipher_key, cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE);
	espress_stream_uint32_set(counter_num, 0x00, 0x00, 0x00, 0x01);
	espress_stream_uint32_set(u, 0x00, 0x00, 0x00, ESPRESS_STREAM_KEY_HISTORY_SIZE);
	espress_stream_uint32_sub(counter_num, u);
	while (!espress_stream_uint32_eq(counter_num, espress_stream_uint32_1))
	{
		i = espress_stream_key->get(key, mac_key, cipher_key, counter_num);
		assert(i &&
			(0 == memcmp(prev_mac_key, mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE)) &&
			(0 == memcmp(prev_cipher_key, cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE)));
		i = espress_stream_key->update(key, counter_num);
		printf("key: update %02x%02x%02x%02x ->\n",
			((b_t *)counter_num)[_O(2, 3)],
			((b_t *)counter_num)[_O(2, 2)],
			((b_t *)counter_num)[_O(2, 1)],
			((b_t *)counter_num)[_O(2, 0)]);
		printf("\tmac_key = "); _dump(mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE); printf("\n");
		printf("\tcipher_key = "); _dump(cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE); printf("\n");
		assert(i);
		espress_stream_uint32_add(counter_num, espress_stream_uint32_1);
		i = espress_stream_key->get(key, mac_key, cipher_key, counter_num);
		printf("key: get %02x%02x%02x%02x -> %s\n",
			((b_t *)counter_num)[_O(2, 3)],
			((b_t *)counter_num)[_O(2, 2)],
			((b_t *)counter_num)[_O(2, 1)],
			((b_t *)counter_num)[_O(2, 0)],
			i ? "OK" : "NG");
		assert(!i);
		espress_stream_uint32_add(counter_num, espress_stream_uint32_1);
	}

	espress_stream_uint32_sub(counter_num, u);
	while (!espress_stream_uint32_eq(counter_num, espress_stream_uint32_1))
	{
		i = espress_stream_key->get(key, mac_key, cipher_key, counter_num);
		printf("key: get %02x%02x%02x%02x -> %s\n",
			((b_t *)counter_num)[_O(2, 3)],
			((b_t *)counter_num)[_O(2, 2)],
			((b_t *)counter_num)[_O(2, 1)],
			((b_t *)counter_num)[_O(2, 0)],
			i ? "OK" : "NG");
		assert(!i);
		espress_stream_uint32_add(counter_num, espress_stream_uint32_1);
	}
	i = espress_stream_key->get(key, mac_key, cipher_key, counter_num);
	assert(i);
	i = espress_stream_key->update(key, counter_num);
	printf("key: update %02x%02x%02x%02x ->\n",
		((b_t *)counter_num)[_O(2, 3)],
		((b_t *)counter_num)[_O(2, 2)],
		((b_t *)counter_num)[_O(2, 1)],
		((b_t *)counter_num)[_O(2, 0)]);
	printf("\tmac_key = "); _dump(mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE); printf("\n");
	printf("\tcipher_key = "); _dump(cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE); printf("\n");
	assert(i &&
		(0 == memcmp(curr_mac_key, mac_key, ESPRESS_STREAM_KEY_MAC_KEY_SIZE)) &&
		(0 == memcmp(curr_cipher_key, cipher_key, ESPRESS_STREAM_KEY_CIPHER_KEY_SIZE)));

	espress_stream_key->final(key);
}
