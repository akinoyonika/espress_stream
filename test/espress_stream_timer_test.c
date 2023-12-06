#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/timer.h"

static void _callback(struct espress_stream_timer *this, void *param)
{
	struct espress_stream_event *event = (struct espress_stream_event *)param;
	espress_stream_event->set(event, 0);
}

void espress_stream_timer_test()
{
	i_t i;
	struct espress_stream_uint32 u[1];
	b_t b0;
	b_t b1;
	b_t b2;
	b_t b3;
	struct espress_stream_uint32 t[1];
	struct espress_stream_event event[1];
	struct espress_stream_event_flags flags[1];
	struct espress_stream_timer timer[1];

	espress_stream_event->init(event);

	espress_stream_timer->init(timer);

	espress_stream_uint32_assign(t, espress_stream_uint32_0);
	for (i = 0 ; i < 4 ; i++)
	{
		espress_stream_timer->start(timer, _callback, (void *)event, t);
		for ( ; ; )
		{
			espress_stream_timer->yield(timer, espress_stream_uint32_0);
			espress_stream_event->wait(event, flags, espress_stream_uint32_0);
			if (espress_stream_event_flags->is_set(flags, 0))
			{
				break;
			}
		}
		espress_stream_tick->get_us(u);
		espress_stream_uint32_get(u, &b0, &b1, &b2, &b3);
		printf("timer: %d -> %02x%02x%02x%02x\n", i, b0, b1, b2, b3);
		assert(espress_stream_event_flags->is_set(flags, 0));
		espress_stream_uint32_add(t, espress_stream_tick_us_1_sec);
	}

	espress_stream_timer->final(timer);

	espress_stream_event->final(event);
}
