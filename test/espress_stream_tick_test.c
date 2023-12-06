#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"

void espress_stream_tick_test()
{
	i_t i;
	b_t b0;
	b_t b1;
	b_t b2;
	b_t b3;
	struct espress_stream_uint32 t[1];
	struct espress_stream_uint32 t0[1];

	espress_stream_tick->get_us(t0);
	espress_stream_uint32_get(t0, &b0, &b1, &b2, &b3);
	for (i = 0, printf("tick: %d -> %02x%02x%02x%02x\n", i, b0, b1, b2, b3) ;
		i < 4 ;
		i++, printf("tick: %d -> %02x%02x%02x%02x\n", i, b0, b1, b2, b3))
	{
		for ( ; ; )
		{
			espress_stream_tick->get_us(t);
			espress_stream_uint32_get(t, &b0, &b1, &b2, &b3);
			espress_stream_uint32_sub(t, t0);
			if (espress_stream_uint32_le(espress_stream_tick_us_1_sec, t))
			{
				espress_stream_uint32_add(t0, espress_stream_tick_us_1_sec);
				break;
			}
		}
	}
}
