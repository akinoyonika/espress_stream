#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_dgram_test()
{
	i_t i;
	u_t u;
	struct espress_stream_event event[1];
	struct espress_stream_event_flags flags[1];
	struct
	{
		espress_stream_dgram_buffer_header;
		b_t data[ESPRESS_STREAM_DGRAM_MTU];
	} send_buffer[1], recv_buffer[1];
	struct espress_stream_dgram_buffer *send_buffers[] = {(struct espress_stream_dgram_buffer *)send_buffer};
	struct espress_stream_dgram_buffer *recv_buffers[] = {(struct espress_stream_dgram_buffer *)recv_buffer};
	struct espress_stream_dgram_address address[1];
	struct espress_stream_dgram dgram[1];

	espress_stream_event->init(event);
	((struct sockaddr_in *)(address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(address->storage))->sin_addr.s_addr = htonl(INADDR_ANY);
	i = espress_stream_dgram->init(dgram, address, event, event);
	assert(0 == i);
	recv_buffer->data_size = espress_stream_dgram->mtu(dgram);
	espress_stream_dgram->recv_request(dgram, recv_buffers, 1, NULL, 0);
	printf("dgram: recv_request\n");
	((struct sockaddr_in *)(send_buffer->address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(send_buffer->address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(send_buffer->address->storage))->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	for (i = 0 ; i < 0x100 ; i++)
	{
		send_buffer->data[i] = i;
	}
	send_buffer->data_size = i;
	espress_stream_dgram->send_request(dgram, send_buffers, 1, NULL, 0);
	printf("dgram: send_request -> "); _dump(send_buffer->data, send_buffer->data_size); printf("\n");
	for ( ; ; )
	{
		espress_stream_dgram->send_yield(dgram, espress_stream_uint32_0);
		espress_stream_dgram->recv_yield(dgram, espress_stream_uint32_0);
		espress_stream_event->wait(event, flags, espress_stream_uint32_0);
		if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_DGRAM_SEND_RESULT))
		{
			i = espress_stream_dgram->send_result(dgram, &u);
			printf("dgram: send_result\n");
			assert((0 == i) && (1 == u));
		}
		if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_DGRAM_RECV_RESULT))
		{
			i = espress_stream_dgram->recv_result(dgram, &u);
			printf("dgram: recv_result -> "); _dump(recv_buffer->data, recv_buffer->data_size); printf("\n");
			assert((0 == i) &&
				(1 == u) &&
				(send_buffer->data_size == recv_buffer->data_size) &&
				(0 == memcmp(send_buffer->data, recv_buffer->data, recv_buffer->data_size)));
			break;
		}
	}
	espress_stream_dgram->final(dgram);
	espress_stream_event->final(event);
}
