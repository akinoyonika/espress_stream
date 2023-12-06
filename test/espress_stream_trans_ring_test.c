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
#include "espress_stream/trans_ring.h"

void espress_stream_trans_ring_test()
{
	i_t i;
	i_t j;
	u_t u;
	struct espress_stream_event event[1];
	struct espress_stream_event_flags flags[1];
	struct
	{
		espress_stream_dgram_buffer_header;
		b_t data[ESPRESS_STREAM_DGRAM_MTU];
	} send_buffer[4], recv_buffer[4];
	struct espress_stream_dgram_buffer *send_buffers[] =
	{
		(struct espress_stream_dgram_buffer *)(send_buffer + 0),
		(struct espress_stream_dgram_buffer *)(send_buffer + 1),
		(struct espress_stream_dgram_buffer *)(send_buffer + 2),
		(struct espress_stream_dgram_buffer *)(send_buffer + 3)
	};
	struct espress_stream_dgram_buffer *recv_buffers[] =
	{
		(struct espress_stream_dgram_buffer *)(recv_buffer + 0),
		(struct espress_stream_dgram_buffer *)(recv_buffer + 1),
		(struct espress_stream_dgram_buffer *)(recv_buffer + 2),
		(struct espress_stream_dgram_buffer *)(recv_buffer + 3)
	};
	struct espress_stream_dgram_address address[1];
	struct espress_stream_dgram dgram[1];
	struct espress_stream_trans_ring send_ring[1];
	struct espress_stream_trans_ring recv_ring[1];

	espress_stream_event->init(event);
	((struct sockaddr_in *)(address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(address->storage))->sin_addr.s_addr = htonl(INADDR_ANY);
	i = espress_stream_dgram->init(dgram, address, event, event);
	assert(0 == i);
	espress_stream_trans_ring->init(send_ring, send_buffers, 4, dgram);
	espress_stream_trans_ring->init(recv_ring, recv_buffers, 4, dgram);

	for ( ; NULL != espress_stream_trans_ring->head(recv_ring, 0) ; )
	{
		espress_stream_trans_ring->head(recv_ring, 0)[0]->data_size = espress_stream_dgram->mtu(dgram);
		espress_stream_trans_ring->en(recv_ring, 1);
	}
	espress_stream_trans_ring->recv_request(recv_ring);

	for (i = 0 ; NULL != espress_stream_trans_ring->head(send_ring, 0) ; i++)
	{
		((struct sockaddr_in *)(send_buffers[i]->address->storage))->sin_family = AF_INET;
		((struct sockaddr_in *)(send_buffers[i]->address->storage))->sin_port = htons(1234);
		((struct sockaddr_in *)(send_buffers[i]->address->storage))->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		for (j = 0, u = i ; j < (0x100 + i) ; j++, u++)
		{
			espress_stream_trans_ring->head(send_ring, 0)[0]->data[j] = (b_t)u;
		}
		espress_stream_trans_ring->head(send_ring, 0)[0]->data_size = j;
		espress_stream_trans_ring->en(send_ring, 1);
	}
	espress_stream_trans_ring->send_request(send_ring);

	for (j = 0 ; j < 4 ; )
	{
		espress_stream_dgram->send_yield(dgram, espress_stream_uint32_0);
		espress_stream_dgram->recv_yield(dgram, espress_stream_uint32_0);
		espress_stream_event->wait(event, flags, espress_stream_uint32_0);
		if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_DGRAM_SEND_RESULT))
		{
			i = espress_stream_trans_ring->send_result(send_ring, &u);
			printf("trans_ring: send_result -> %u\n", u);
			assert(0 == i);
			if (0 < espress_stream_trans_ring->num_valid(send_ring))
			{
				espress_stream_trans_ring->send_request(send_ring);
			}
		}
		if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_DGRAM_RECV_RESULT))
		{
			i = espress_stream_trans_ring->recv_result(recv_ring, &u);
			printf("trans_ring: recv_result -> %u\n", u);
			assert(0 == i);
			while (NULL != espress_stream_trans_ring->head(recv_ring, j))
			{
				assert((0x100 + j) == espress_stream_trans_ring->head(recv_ring, j)[0]->data_size);
				for (i = 0, u = j ; i < (0x100 + j) ; i++, u++)
				{
					assert((b_t)u == espress_stream_trans_ring->head(recv_ring, j)[0]->data[i]);
				}
				j++;
			}
			if (0 < espress_stream_trans_ring->num_valid(recv_ring))
			{
				espress_stream_trans_ring->recv_request(recv_ring);
			}
		}
	}

	espress_stream_trans_ring->final(send_ring);
	espress_stream_trans_ring->final(recv_ring);
	espress_stream_dgram->final(dgram);
	espress_stream_event->final(event);
}
