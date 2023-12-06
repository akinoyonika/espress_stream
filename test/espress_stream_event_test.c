#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"

void espress_stream_event_test()
{
	i_t i;
	i_t j;
	i_t k;
	struct espress_stream_event_flags flags[1];
	struct espress_stream_event event[1];

	espress_stream_event->init(event);
	for (i = 0 ; i < 4 ; i++)
	{
		espress_stream_event->wait(event, flags, espress_stream_uint32_0);
		for (j = 0 ; j < 4 ; j++)
		{
			k = espress_stream_event_flags->is_set(flags, (u_t)i);
			assert(0 == k);
		}
		espress_stream_event->set(event, (u_t)i);
		espress_stream_event->wait(event, flags, espress_stream_uint32_0);
		printf("event: ");
		for (j = 0 ; j < 4 ; j++)
		{
			k = espress_stream_event_flags->is_set(flags, (u_t)j);
			printf("%d ", k);
			assert((i == j) ? (0 != k) : (0 == k));
		}
		printf("\n");
	}
	espress_stream_event->final(event);

	espress_stream_event->init(event);
	for (i = 0 ; i < 4 ; i++)
	{
		espress_stream_event->wait(event, flags, espress_stream_uint32_0);
		for (j = 0 ; j < 4 ; j++)
		{
			k = espress_stream_event_flags->is_set(flags, (u_t)i);
			assert(0 == k);
		}
		for (j = 0 ; j <= i ; j++)
		{
			espress_stream_event->set(event, (u_t)j);
		}
		espress_stream_event->wait(event, flags, espress_stream_uint32_0);
		printf("event: ");
		for (j = 0 ; j < 4 ; j++)
		{
			k = espress_stream_event_flags->is_set(flags, (u_t)j);
			printf("%d ", k);
			assert((j <= i) ? (0 != k) : (0 == k));
		}
		printf("\n");
	}
	espress_stream_event->final(event);
}
