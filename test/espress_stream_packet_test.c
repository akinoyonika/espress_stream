#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"
#include "espress_stream/key_exp.h"
#include "espress_stream/key.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"
#include "espress_stream/trans_ring.h"
#include "espress_stream/packet.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_packet_test()
{
	i_t i;
	i_t j;
	u_t u;
	b_t b[4];
	struct espress_stream_packet *p;
	struct espress_stream_event event[1];
	struct espress_stream_event_flags flags[1];
	struct espress_stream_key send_key[1];
	struct espress_stream_key recv_key[1];
	struct espress_stream_packet send_packet[4];
	struct espress_stream_packet recv_packet[4];
	struct espress_stream_dgram_buffer *send_buffers[] =
	{
		(struct espress_stream_dgram_buffer *)(send_packet + 0),
		(struct espress_stream_dgram_buffer *)(send_packet + 1),
		(struct espress_stream_dgram_buffer *)(send_packet + 2),
		(struct espress_stream_dgram_buffer *)(send_packet + 3)
	};
	struct espress_stream_dgram_buffer *recv_buffers[] =
	{
		(struct espress_stream_dgram_buffer *)(recv_packet + 0),
		(struct espress_stream_dgram_buffer *)(recv_packet + 1),
		(struct espress_stream_dgram_buffer *)(recv_packet + 2),
		(struct espress_stream_dgram_buffer *)(recv_packet + 3)
	};
	struct espress_stream_dgram_address address[1];
	struct espress_stream_dgram dgram[1];
	struct espress_stream_trans_ring send_ring[1];
	struct espress_stream_trans_ring recv_ring[1];

	espress_stream_event->initialize(event);
	((struct sockaddr_in *)(address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(address->storage))->sin_addr.s_addr = htonl(INADDR_ANY);
	i = espress_stream_dgram->initialize(dgram, address, event, event);
	assert(0 == i);
	espress_stream_trans_ring->initialize(send_ring, send_buffers, 4, dgram);
	espress_stream_trans_ring->initialize(recv_ring, recv_buffers, 4, dgram);
	espress_stream_key->initialize(send_key, (b_t const *)"0123456789ABCDEFGHIJKLMNOPQRSTU", 32, (b_t const *)"0123456789abcdefghijklmnopqrstu", 32);
	espress_stream_key->initialize(recv_key, (b_t const *)"0123456789ABCDEFGHIJKLMNOPQRSTU", 32, (b_t const *)"0123456789abcdefghijklmnopqrstu", 32);

	for ( ; NULL != espress_stream_trans_ring->head(recv_ring, 0) ; )
	{
		p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(recv_ring, 0)[0]);
		espress_stream_packet->initialize(p, event);
		p->data_size = espress_stream_dgram->mtu(dgram);
		espress_stream_trans_ring->en(recv_ring, 1);
	}
	espress_stream_trans_ring->recv_request(recv_ring);

	for (i = 0 ; NULL != espress_stream_trans_ring->head(send_ring, i) ; i++)
	{
		p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(send_ring, i)[0]);
		espress_stream_packet->initialize(p, event);
	}

	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(send_ring, 0)[0]);
	assert(NULL != p);
	p->data_size = 0;
	((struct sockaddr_in *)(p->address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(p->address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(p->address->storage))->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	espress_stream_uint16_assign(espress_stream_packet->packet_type(p), espress_stream_packet_type_client_connect);
	espress_stream_uint16_set_u(espress_stream_packet->packet_size(p), ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE);
	espress_stream_uint32_set(espress_stream_packet->connection_id(p), 0x00, 0x00, 0x00, 0x00);
	espress_stream_uint32_set(espress_stream_packet->connection_param(p), 0x00, 0x00, 0x00, 0x01);
	espress_stream_uint32_set(espress_stream_packet->initial_window_size(p), 0x00, 0x01, 0x00, 0x00);
	for (i = 0 ; i < espress_stream_packet->key_change_size(p) ; i++)
	{
		espress_stream_packet->key_change(p)[i] = i;
	}
	for (i = 0 ; i < espress_stream_packet->random_size(p) ; i++)
	{
		espress_stream_packet->random(p)[i] = 0xff - i;
	}
	printf("packet: "); _dump(p->data, ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE); printf(" -> encode\n");
	i = espress_stream_trans_ring->en(send_ring, 1);
	assert(1 == i);
	espress_stream_packet->encode(p, 0, send_key);

	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(send_ring, 0)[0]);
	assert(NULL != p);
	p->data_size = 0;
	((struct sockaddr_in *)(p->address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(p->address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(p->address->storage))->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	espress_stream_uint16_assign(espress_stream_packet->packet_type(p), espress_stream_packet_type_client_envelope);
	espress_stream_uint16_set_u(espress_stream_packet->packet_size(p), ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE + 11);
	espress_stream_uint32_set(espress_stream_packet->connection_id(p), 0x00, 0x01, 0x02, 0x03);
	espress_stream_uint32_set(espress_stream_packet->counter_number(p), 0x04, 0x05, 0x06, 0x07);
	espress_stream_uint32_set(espress_stream_packet->receive_counter_number(p), 0x08, 0x09, 0x0a, 0x0b);
	espress_stream_uint32_set(espress_stream_packet->receive_window_address(p), 0x0c, 0x0d, 0x0e, 0x0f);
	espress_stream_uint32_set(espress_stream_packet->receive_window_size(p), 0x10, 0x11, 0x12, 0x13);
	espress_stream_uint32_set(espress_stream_packet->send_data_address(p), 0x14, 0x15, 0x16, 0x17);
	for (i = 0 ; i < 11 ; i++)
	{
		espress_stream_packet->envelope_payload(p)[i] = i;
	}
	printf("packet: "); _dump(p->data, ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE + 11); printf(" -> encode\n");
	i = espress_stream_trans_ring->en(send_ring, 1);
	assert(1 == i);
	espress_stream_packet->encode(p, 0, send_key);

	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(send_ring, 0)[0]);
	assert(NULL != p);
	p->data_size = 0;
	((struct sockaddr_in *)(p->address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(p->address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(p->address->storage))->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	espress_stream_uint16_assign(espress_stream_packet->packet_type(p), espress_stream_packet_type_client_envelope);
	espress_stream_uint16_set_u(espress_stream_packet->packet_size(p), ESPRESS_STREAM_PACKET_ENVELOPE_ACK_SIZE);
	espress_stream_uint32_set(espress_stream_packet->connection_id(p), 0x03, 0x02, 0x01, 0x00);
	espress_stream_uint32_set(espress_stream_packet->counter_number(p), 0x07, 0x06, 0x05, 0x04);
	espress_stream_uint32_set(espress_stream_packet->receive_counter_number(p), 0x0b, 0x0a, 0x09, 0x08);
	espress_stream_uint32_set(espress_stream_packet->receive_window_address(p), 0x0f, 0x0e, 0x0d, 0x0c);
	espress_stream_uint32_set(espress_stream_packet->receive_window_size(p), 0x13, 0x12, 0x11, 0x10);
	printf("packet: "); _dump(p->data, ESPRESS_STREAM_PACKET_ENVELOPE_ACK_SIZE); printf(" -> encode\n");
	i = espress_stream_trans_ring->en(send_ring, 1);
	assert(1 == i);
	espress_stream_packet->encode(p, 0, send_key);

	p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(send_ring, 0)[0]);
	assert(NULL != p);
	p->data_size = 0;
	((struct sockaddr_in *)(p->address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(p->address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(p->address->storage))->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	espress_stream_uint16_assign(espress_stream_packet->packet_type(p), espress_stream_packet_type_client_disconnect);
	espress_stream_uint16_set_u(espress_stream_packet->packet_size(p), ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE);
	espress_stream_uint32_set(espress_stream_packet->connection_id(p), 0x00, 0x11, 0x22, 0x33);
	espress_stream_uint32_set(espress_stream_packet->counter_number(p), 0x44, 0x55, 0x66, 0x77);
	printf("packet: "); _dump(p->data, ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE); printf(" -> encode\n");
	i = espress_stream_trans_ring->en(send_ring, 1);
	assert(1 == i);
	espress_stream_packet->encode(p, 0, send_key);

	for (i = 0 ; i < 4 ; )
	{
		for (j = 0 ; j < 4 ; j++)
		{
			espress_stream_packet->yield(send_packet + j, espress_stream_uint32_0);
			espress_stream_packet->yield(recv_packet + j, espress_stream_uint32_0);
		}
		espress_stream_dgram->send_yield(dgram, espress_stream_uint32_0);
		espress_stream_dgram->recv_yield(dgram, espress_stream_uint32_0);
		espress_stream_event->wait(event, flags, espress_stream_uint32_0);
		if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_PACKET_ENCODE_RESULT))
		{
			for (j = 0 ; espress_stream_trans_ring->tail(send_ring, j) ; j++)
			{
				if (0 == espress_stream_trans_ring->tail(send_ring, j)[0]->data_size)
				{
					break;
				}
				if (ESPRESS_STREAM_TRANS_RING_INVALID_DATA_SIZE == espress_stream_trans_ring->tail(send_ring, j)[0]->data_size)
				{
					continue;
				}
				printf("packet: "); _dump(espress_stream_trans_ring->tail(send_ring, j)[0]->data, espress_stream_trans_ring->tail(send_ring, j)[0]->data_size); printf(" -> send\n");
			}
			espress_stream_trans_ring->send_request(send_ring);
		}
		if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_PACKET_DECODE_RESULT))
		{
			while (NULL != espress_stream_trans_ring->head(recv_ring, 0))
			{
				p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(recv_ring, 0)[0]);
				if (0 == p->data_size)
				{
					switch (i)
					{
					case 0:
						assert(espress_stream_uint16_eq(espress_stream_packet->packet_type(p), espress_stream_packet_type_client_connect));
						espress_stream_uint16_get_u(espress_stream_packet->packet_size(p), &u);
						assert(ESPRESS_STREAM_PACKET_CONNECT_MIN_SIZE == u);
						espress_stream_uint32_get(espress_stream_packet->connection_id(p), b, b + 1, b + 2, b + 3);
						assert((0x00 == b[0]) && (0x00 == b[1]) && (0x00 == b[2]) && (0x00 == b[3]));
						espress_stream_uint32_get(espress_stream_packet->connection_param(p), b, b + 1, b + 2, b + 3);
						assert((0x00 == b[0]) && (0x00 == b[1]) && (0x00 == b[2]) && (0x01 == b[3]));
						espress_stream_uint32_get(espress_stream_packet->initial_window_size(p), b, b + 1, b + 2, b + 3);
						assert((0x00 == b[0]) && (0x01 == b[1]) && (0x00 == b[2]) && (0x00 == b[3]));
						for (j = 0 ; j < espress_stream_packet->key_change_size(p) ; j++)
						{
							assert(j == espress_stream_packet->key_change(p)[j]);
						}
						for (j = 0 ; j < espress_stream_packet->random_size(p) ; j++)
						{
							assert(0xff - j == espress_stream_packet->random(p)[j]);
						}
						break;
					case 1:
						assert(espress_stream_uint16_eq(espress_stream_packet->packet_type(p), espress_stream_packet_type_client_envelope));
						espress_stream_uint16_get_u(espress_stream_packet->packet_size(p), &u);
						assert((ESPRESS_STREAM_PACKET_ENVELOPE_MIN_SIZE + 11) == u);
						espress_stream_uint32_get(espress_stream_packet->connection_id(p), b, b + 1, b + 2, b + 3);
						assert((0x00 == b[0]) && (0x01 == b[1]) && (0x02 == b[2]) && (0x03 == b[3]));
						espress_stream_uint32_get(espress_stream_packet->counter_number(p), b, b + 1, b + 2, b + 3);
						assert((0x04 == b[0]) && (0x05 == b[1]) && (0x06 == b[2]) && (0x07 == b[3]));
						espress_stream_uint32_get(espress_stream_packet->receive_counter_number(p), b, b + 1, b + 2, b + 3);
						assert((0x08 == b[0]) && (0x09 == b[1]) && (0x0a == b[2]) && (0x0b == b[3]));
						espress_stream_uint32_get(espress_stream_packet->receive_window_address(p), b, b + 1, b + 2, b + 3);
						assert((0x0c == b[0]) && (0x0d == b[1]) && (0x0e == b[2]) && (0x0f == b[3]));
						espress_stream_uint32_get(espress_stream_packet->receive_window_size(p), b, b + 1, b + 2, b + 3);
						assert((0x10 == b[0]) && (0x11 == b[1]) && (0x12 == b[2]) && (0x13 == b[3]));
						espress_stream_uint32_get(espress_stream_packet->send_data_address(p), b, b + 1, b + 2, b + 3);
						assert((0x14 == b[0]) && (0x15 == b[1]) && (0x16 == b[2]) && (0x17 == b[3]));
						for (j = 0 ; j < 11 ; j++)
						{
							assert(j == espress_stream_packet->envelope_payload(p)[j]);
						}
						break;
					case 2:
						assert(espress_stream_uint16_eq(espress_stream_packet->packet_type(p), espress_stream_packet_type_client_envelope));
						espress_stream_uint16_get_u(espress_stream_packet->packet_size(p), &u);
						assert(ESPRESS_STREAM_PACKET_ENVELOPE_ACK_SIZE == u);
						espress_stream_uint32_get(espress_stream_packet->connection_id(p), b, b + 1, b + 2, b + 3);
						assert((0x03 == b[0]) && (0x02 == b[1]) && (0x01 == b[2]) && (0x00 == b[3]));
						espress_stream_uint32_get(espress_stream_packet->counter_number(p), b, b + 1, b + 2, b + 3);
						assert((0x07 == b[0]) && (0x06 == b[1]) && (0x05 == b[2]) && (0x04 == b[3]));
						espress_stream_uint32_get(espress_stream_packet->receive_counter_number(p), b, b + 1, b + 2, b + 3);
						assert((0x0b == b[0]) && (0x0a == b[1]) && (0x09 == b[2]) && (0x08 == b[3]));
						espress_stream_uint32_get(espress_stream_packet->receive_window_address(p), b, b + 1, b + 2, b + 3);
						assert((0x0f == b[0]) && (0x0e == b[1]) && (0x0d == b[2]) && (0x0c == b[3]));
						espress_stream_uint32_get(espress_stream_packet->receive_window_size(p), b, b + 1, b + 2, b + 3);
						assert((0x13 == b[0]) && (0x12 == b[1]) && (0x11 == b[2]) && (0x10 == b[3]));
						break;
					case 3:
						assert(espress_stream_uint16_eq(espress_stream_packet->packet_type(p), espress_stream_packet_type_client_disconnect));
						espress_stream_uint16_get_u(espress_stream_packet->packet_size(p), &u);
						assert(ESPRESS_STREAM_PACKET_DISCONNECT_MIN_SIZE == u);
						espress_stream_uint32_get(espress_stream_packet->connection_id(p), b, b + 1, b + 2, b + 3);
						assert((0x00 == b[0]) && (0x11 == b[1]) && (0x22 == b[2]) && (0x33 == b[3]));
						espress_stream_uint32_get(espress_stream_packet->counter_number(p), b, b + 1, b + 2, b + 3);
						assert((0x44 == b[0]) && (0x55 == b[1]) && (0x66 == b[2]) && (0x77 == b[3]));
						break;
					default:
						break;
					}
					espress_stream_uint16_get_u(espress_stream_packet->packet_size(p), &u);
					printf("packet: decode -> "); _dump(p->data, u); printf("\n");
					p->data_size = espress_stream_dgram->mtu(dgram);
					espress_stream_trans_ring->en(recv_ring, 1);
					espress_stream_trans_ring->recv_request(recv_ring);
					i++;
				}
				else
				{
					break;
				}
			}
		}
		if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_DGRAM_SEND_RESULT))
		{
			espress_stream_trans_ring->send_result(send_ring, &u);
			if (!espress_stream_trans_ring->is_empty(send_ring))
			{
				espress_stream_trans_ring->send_request(send_ring);
			}
		}
		if (espress_stream_event_flags->is_set(flags, ESPRESS_STREAM_EVENT_DGRAM_RECV_RESULT))
		{
			espress_stream_trans_ring->recv_result(recv_ring, &u);
			if (!espress_stream_trans_ring->is_empty(recv_ring))
			{
				espress_stream_trans_ring->recv_request(recv_ring);
			}
			for (j = espress_stream_trans_ring->num_invalid(recv_ring) - u ;
				NULL != espress_stream_trans_ring->head(recv_ring, j) ;
				j++)
			{
				p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(recv_ring, j)[0]);
				printf("packet: recv -> "); _dump(espress_stream_trans_ring->head(recv_ring, j)[0]->data, espress_stream_trans_ring->head(recv_ring, j)[0]->data_size); printf("\n");
				espress_stream_packet->decode(p, 0, recv_key);
			}
		}
	}

	espress_stream_trans_ring->de(send_ring, espress_stream_trans_ring->num_valid(send_ring));
	for (i = 0 ; NULL != espress_stream_trans_ring->head(send_ring, i) ; i++)
	{
		p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(send_ring, i)[0]);
		espress_stream_packet->finalize(p);
	}
	espress_stream_trans_ring->de(recv_ring, espress_stream_trans_ring->num_valid(recv_ring));
	for (i = 0 ; NULL != espress_stream_trans_ring->head(recv_ring, i) ; i++)
	{
		p = (struct espress_stream_packet *)(espress_stream_trans_ring->head(recv_ring, i)[0]);
		espress_stream_packet->finalize(p);
	}
	espress_stream_trans_ring->finalize(send_ring);
	espress_stream_trans_ring->finalize(recv_ring);
	espress_stream_dgram->finalize(dgram);
	espress_stream_event->finalize(event);
}
