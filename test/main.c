#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/rand.h"

extern void espress_stream_macro_repeat_test(void);
extern void espress_stream_uint_test(void);
extern void espress_stream_ec_test(void);
extern void espress_stream_key_ex_test(void);
extern void espress_stream_hash_test(void);
extern void espress_stream_mac_test(void);
extern void espress_stream_hkdf_test(void);
extern void espress_stream_key_exp_test(void);
extern void espress_stream_block_cipher_test(void);
extern void espress_stream_ctr_cipher_test(void);
extern void espress_stream_ring_test(void);
extern void espress_stream_event_test(void);
extern void espress_stream_tick_test(void);
extern void espress_stream_timer_test(void);
extern void espress_stream_rand_test(void);
extern void espress_stream_dgram_test(void);
extern void espress_stream_trans_ring_test(void);
extern void espress_stream_key_test(void);
extern void espress_stream_packet_test(void);
extern void espress_stream_trans_packet_test(void);
extern void espress_stream_ring_buffer_test(void);
extern void espress_stream_session_test(void);

int main(int argc, char **argv)
{
	int i;

	i = espress_stream_tick->initialize();
	assert(0 == i);
	i = espress_stream_rand->initialize();
	assert(0 == i);

	if (argc < 2)
	{
		espress_stream_macro_repeat_test();
		espress_stream_uint_test();
		espress_stream_ec_test();
		espress_stream_key_ex_test();
		espress_stream_hash_test();
		espress_stream_mac_test();
		espress_stream_hkdf_test();
		espress_stream_key_exp_test();
		espress_stream_block_cipher_test();
		espress_stream_ctr_cipher_test();
		espress_stream_ring_test();
		espress_stream_event_test();
		espress_stream_tick_test();
		espress_stream_timer_test();
		espress_stream_rand_test();
		espress_stream_dgram_test();
		espress_stream_trans_ring_test();
		espress_stream_key_test();
		espress_stream_packet_test();
		espress_stream_trans_packet_test();
		espress_stream_ring_buffer_test();
		espress_stream_session_test();
	}
	else
	{
		for (i = 1 ; i < argc ; i++)
		{
			if (0 == strcmp("macro_repeat", argv[i]))
			{
				espress_stream_macro_repeat_test();
			}
			else if (0 == strcmp("uint", argv[i]))
			{
				espress_stream_uint_test();
			}
			else if (0 == strcmp("ec", argv[i]))
			{
				espress_stream_ec_test();
			}
			else if (0 == strcmp("key_ex", argv[i]))
			{
				espress_stream_key_ex_test();
			}
			else if (0 == strcmp("hash", argv[i]))
			{
				espress_stream_hash_test();
			}
			else if (0 == strcmp("mac", argv[i]))
			{
				espress_stream_mac_test();
			}
			else if (0 == strcmp("hkdf", argv[i]))
			{
				espress_stream_hkdf_test();
			}
			else if (0 == strcmp("key_exp", argv[i]))
			{
				espress_stream_key_exp_test();
			}
			else if (0 == strcmp("block_cipher", argv[i]))
			{
				espress_stream_block_cipher_test();
			}
			else if (0 == strcmp("ctr_cipher", argv[i]))
			{
				espress_stream_ctr_cipher_test();
			}
			else if (0 == strcmp("ring", argv[i]))
			{
				espress_stream_ring_test();
			}
			else if (0 == strcmp("event", argv[i]))
			{
				espress_stream_event_test();
			}
			else if (0 == strcmp("tick", argv[i]))
			{
				espress_stream_tick_test();
			}
			else if (0 == strcmp("timer", argv[i]))
			{
				espress_stream_timer_test();
			}
			else if (0 == strcmp("rand", argv[i]))
			{
				espress_stream_rand_test();
			}
			else if (0 == strcmp("dgram", argv[i]))
			{
				espress_stream_dgram_test();
			}
			else if (0 == strcmp("trans_ring", argv[i]))
			{
				espress_stream_trans_ring_test();
			}
			else if (0 == strcmp("key", argv[i]))
			{
				espress_stream_key_test();
			}
			else if (0 == strcmp("packet", argv[i]))
			{
				espress_stream_packet_test();
			}
			else if (0 == strcmp("trans_packet", argv[i]))
			{
				espress_stream_trans_packet_test();
			}
			else if (0 == strcmp("ring_buffer", argv[i]))
			{
				espress_stream_ring_buffer_test();
			}
			else if (0 == strcmp("session", argv[i]))
			{
				espress_stream_session_test();
			}
		}
	}

	espress_stream_rand->finalize();
	espress_stream_tick->finalize();

	return 0;
}
