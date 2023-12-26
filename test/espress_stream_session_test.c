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
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/rand.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"
#include "espress_stream/hash.h"
#include "espress_stream/mac.h"
#include "espress_stream/hkdf.h"
#include "espress_stream/ec.h"
#include "espress_stream/key_ex.h"
#include "espress_stream/key_exp.h"
#include "espress_stream/key.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"
#include "espress_stream/trans_ring.h"
#include "espress_stream/packet.h"
#include "espress_stream/trans_packet.h"
#include "espress_stream/ring_buffer.h"
#include "espress_stream/session.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

static i_t _send_connect_payload(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size, i_t is_rejected), void *param)
{
	assert((0 == strcmp("send", (char *)param)) || (0 == strcmp("SEND", (char *)param)));
	return callback(this, NULL, 0, 0);
}

static i_t _recv_connect_payload(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param)
{
	assert((0 == strcmp("recv", (char *)param)) || (0 == strcmp("RECV", (char *)param)));
	return callback(this, NULL, NULL);
}

static i_t _send_disconnect_payload(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t const *payload, u_t payload_size), void *param)
{
	assert((0 == strcmp("send", (char *)param)) || (0 == strcmp("SEND", (char *)param)));
	return callback(this, NULL, 0);
}

static i_t _recv_disconnect_payload(struct espress_stream_session *this, i_t (*callback)(struct espress_stream_session *this, b_t *payload, u_t *payload_size), void *param)
{
	assert((0 == strcmp("recv", (char *)param)) || (0 == strcmp("RECV", (char *)param)));
	return callback(this, NULL, NULL);
}

static void _interface_yield(struct espress_stream_trans_packet_interface *interface)
{
	i_t i;
	for (i = 0 ; NULL != espress_stream_trans_packet->yield[i] ; i++)
	{
		espress_stream_trans_packet->yield[i](interface, espress_stream_uint32_0);
	}
}

static void _session_yield(struct espress_stream_session *session)
{
	i_t i;
	for (i = 0 ; NULL != espress_stream_session->yield[i] ; i++)
	{
		espress_stream_session->yield[i](session, espress_stream_uint32_0);
	}
}

void espress_stream_session_test()
{
	i_t i;
	i_t j;
	u_t u;
	b_t *b;
	struct espress_stream_event_flags f[1];
	struct espress_stream_trans_packet_interface interface[1];
	struct espress_stream_dgram_address address[1];
	struct espress_stream_ring_buffer client_send_ring_buffer[1];
	struct espress_stream_ring_buffer client_recv_ring_buffer[1];
	struct espress_stream_ring_buffer server_send_ring_buffer[1];
	struct espress_stream_ring_buffer server_recv_ring_buffer[1];
#	if 4 <= U_T_WIDTH
		b_t client_send_buffer[1 << 18];
		b_t client_recv_buffer[1 << 18];
		b_t server_send_buffer[1 << 18];
		b_t server_recv_buffer[1 << 18];
#	else
		b_t client_send_buffer[1 << 13];
		b_t client_recv_buffer[1 << 13];
		b_t server_send_buffer[1 << 13];
		b_t server_recv_buffer[1 << 13];
#	endif
	struct espress_stream_event client_event[1];
	struct espress_stream_event server_event[1];
	struct espress_stream_event_flags client_flags[1];
	struct espress_stream_event_flags server_flags[1];
	struct espress_stream_session client_session[1];
	struct espress_stream_session server_session[1];
	struct espress_stream_uint32 client_session_id[1];
	struct espress_stream_uint32 server_session_id[1];
	b_t client_shared_key[ESPRESS_STREAM_KEY_EX_KEY_SIZE];
	b_t server_shared_key[ESPRESS_STREAM_KEY_EX_KEY_SIZE];
	u_t client_send_counter;
	u_t client_recv_counter;
	u_t server_recv_counter;
	u_t server_send_counter;
#	define CLIENT_SEND_COUNTER_OFFSET (0x55)
#	define CLIENT_RECV_COUNTER_OFFSET (SERVER_SEND_COUNTER_OFFSET)
#	define SERVER_RECV_COUNTER_OFFSET (CLIENT_SEND_COUNTER_OFFSET)
#	define SERVER_SEND_COUNTER_OFFSET (0xaa)
#	if 4 <= U_T_WIDTH
#		define TEST_DATA_SIZE (1024 * 1024)
#	else
#		define TEST_DATA_SIZE (32 * 1024)
#	endif
#	define TEST_NUM_REPEAT (4)
	struct espress_stream_uint32 client_rtt[1];
	struct espress_stream_uint32 server_rtt[1];
	struct espress_stream_uint32 client_rto[1];
	struct espress_stream_uint32 server_rto[1];

	((struct sockaddr_in *)(address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(address->storage))->sin_addr.s_addr = htonl(INADDR_ANY);
	i = espress_stream_trans_packet->initialize(interface, address);
	assert(0 == i);

	espress_stream_ring_buffer->initialize(client_send_ring_buffer, client_send_buffer, sizeof(client_send_buffer));
	espress_stream_ring_buffer->initialize(client_recv_ring_buffer, client_recv_buffer, sizeof(client_recv_buffer));
	espress_stream_ring_buffer->initialize(server_send_ring_buffer, server_send_buffer, sizeof(server_send_buffer));
	espress_stream_ring_buffer->initialize(server_recv_ring_buffer, server_recv_buffer, sizeof(server_recv_buffer));

	espress_stream_event_flags->reset_all(client_flags);
	espress_stream_event_flags->reset_all(server_flags);
	i = espress_stream_event->initialize(client_event);
	assert(0 == i);
	i = espress_stream_event->initialize(server_event);
	assert(0 == i);

	i = espress_stream_session->initialize(client_session,
		interface,
		client_event,
		0,
		client_send_ring_buffer,
		client_recv_ring_buffer,
		_send_connect_payload, "send",
		_recv_connect_payload, "recv",
		_send_disconnect_payload, "send",
		_recv_disconnect_payload, "recv");
	assert(0 == i);
	i = espress_stream_session->initialize(server_session,
		interface,
		server_event,
		1,
		server_send_ring_buffer,
		server_recv_ring_buffer,
		_send_connect_payload, "SEND",
		_recv_connect_payload, "RECV",
		_send_disconnect_payload, "SEND",
		_recv_disconnect_payload, "RECV");
	assert(0 == i);

	((struct sockaddr_in *)(address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(address->storage))->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	i = espress_stream_session->connect(client_session, address);
	assert(0 == i);
	printf("session: client: connect\n");

	for ( ; ; )
	{
		_interface_yield(interface);
		_session_yield(client_session);
		_session_yield(server_session);
		espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
		espress_stream_event_flags->compo(server_flags, f);
		if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_DETECT))
		{
			espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_DETECT);
			i = espress_stream_session->connect_detect(server_session);
			assert(0 == i);
			break;
		}
	}
	printf("session: server: connect_detect\n");

	for ( ; ; )
	{
		_interface_yield(interface);
		_session_yield(client_session);
		_session_yield(server_session);
		espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
		espress_stream_event_flags->compo(client_flags, f);
		if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_RESULT))
		{
			espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_SESSION_CONNECT_RESULT);
			i = espress_stream_session->connect_result(client_session);
			assert(0 == i);
			break;
		}
	}
	printf("session: client: connect_result\n");

	espress_stream_session->connection_id(client_session, client_session_id);
	espress_stream_uint32_get_u(client_session_id, &u);
	printf("session: client: session_id = %08x\n", u);
	espress_stream_session->connection_id(server_session, server_session_id);
	espress_stream_uint32_get_u(server_session_id, &u);
	printf("session: server: session_id = %08x\n", u);
	assert(espress_stream_uint32_eq(client_session_id, server_session_id));
	espress_stream_session->shared_key(client_session, client_shared_key);
	printf("session: client: shared_key = "); _dump(client_shared_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE); printf("\n");
	espress_stream_session->shared_key(server_session, server_shared_key);
	printf("session: server: shared_key = "); _dump(server_shared_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE); printf("\n");
	assert(0 == memcmp(client_shared_key, server_shared_key, ESPRESS_STREAM_KEY_EX_KEY_SIZE));

	for ( ; ; )
	{
		b = espress_stream_ring_buffer->head(client_send_ring_buffer, NULL, 0);
		if (NULL != b)
		{
			b[0] = 0xa5;
			espress_stream_ring_buffer->en(client_send_ring_buffer, 1);
			i = espress_stream_session->flush(client_session);
			assert(0 == i);
			break;
		}
		for ( ; ; )
		{
			_interface_yield(interface);
			_session_yield(client_session);
			_session_yield(server_session);
			espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(client_flags, f);
			if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_SESSION_SEND))
			{
				espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
				break;
			}
		}
	}
	printf("session: client: send\n");

	for ( ; ; )
	{
		b = espress_stream_ring_buffer->head(server_send_ring_buffer, NULL, 0);
		if (NULL != b)
		{
			b[0] = 0xa5;
			espress_stream_ring_buffer->en(server_send_ring_buffer, 1);
			i = espress_stream_session->flush(server_session);
			assert(0 == i);
			break;
		}
		for ( ; ; )
		{
			_interface_yield(interface);
			_session_yield(client_session);
			_session_yield(server_session);
			espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(server_flags, f);
			if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_SESSION_SEND))
			{
				espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_SESSION_SEND);
				break;
			}
		}
	}
	printf("session: server: send\n");

	for ( ; ; )
	{
		_interface_yield(interface);
		_session_yield(client_session);
		_session_yield(server_session);
		espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
		espress_stream_event_flags->compo(client_flags, f);
		if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_SESSION_RECV))
		{
			espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_SESSION_RECV);
			b = espress_stream_ring_buffer->tail(client_recv_ring_buffer, NULL, 0);
			if (NULL != b)
			{
				assert(0xa5 == b[0]);
				espress_stream_ring_buffer->de(client_recv_ring_buffer, 1);
				i = espress_stream_session->recv(client_session);
				assert(0 == i);
			}
			break;
		}
	}
	printf("session: client: recv\n");

	for ( ; ; )
	{
		_interface_yield(interface);
		_session_yield(client_session);
		_session_yield(server_session);
		espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
		espress_stream_event_flags->compo(server_flags, f);
		if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_SESSION_RECV))
		{
			espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_SESSION_RECV);
			b = espress_stream_ring_buffer->tail(server_recv_ring_buffer, NULL, 0);
			if (NULL != b)
			{
				assert(0xa5 == b[0]);
				espress_stream_ring_buffer->de(server_recv_ring_buffer, 1);
				i = espress_stream_session->recv(server_session);
				assert(0 == i);
			}
			break;
		}
	}
	printf("session: server: recv\n");

	for (j = 0 ; j < TEST_NUM_REPEAT ; j++)
	{
		for (client_send_counter = 0,
				client_recv_counter = 0,
				server_send_counter = 0,
				server_recv_counter = 0 ;
			(client_send_counter <  TEST_DATA_SIZE) ||
				(client_recv_counter < TEST_DATA_SIZE) ||
				(server_send_counter < TEST_DATA_SIZE) ||
				(server_recv_counter <  TEST_DATA_SIZE) ;
			)
		{
			_interface_yield(interface);
			_session_yield(client_session);
			_session_yield(server_session);
	
			espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(client_flags, f);
			if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_SESSION_SEND))
			{
				for (u = 0 ;
					(client_send_counter < TEST_DATA_SIZE) &&
						(NULL != (b = espress_stream_ring_buffer->head(client_send_ring_buffer, NULL, u))) ;
					u++, client_send_counter++)
				{
					b[0] = (b_t)(CLIENT_SEND_COUNTER_OFFSET + client_send_counter);
				}
				if (0 < u)
				{
					espress_stream_ring_buffer->en(client_send_ring_buffer, u);
					if (TEST_DATA_SIZE == client_send_counter)
					{
						espress_stream_session->flush(client_session);
					}
					else
					{
						espress_stream_session->send(client_session);
					}
				}
			}
	
			espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(server_flags, f);
			if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_SESSION_SEND))
			{
				for (u = 0 ;
					(server_send_counter < TEST_DATA_SIZE) &&
						(NULL != (b = espress_stream_ring_buffer->head(server_send_ring_buffer, NULL, u))) ;
					u++, server_send_counter++)
				{
					b[0] = (b_t)(SERVER_SEND_COUNTER_OFFSET + server_send_counter);
				}
				if (0 < u)
				{
					espress_stream_ring_buffer->en(server_send_ring_buffer, u);
					if (TEST_DATA_SIZE == server_send_counter)
					{
						espress_stream_session->flush(server_session);
					}
					else
					{
						espress_stream_session->send(server_session);
					}
				}
			}
	
			espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(client_flags, f);
			if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_SESSION_RECV))
			{
				espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_SESSION_RECV);
				for (u = 0 ;
					NULL != (b = espress_stream_ring_buffer->tail(client_recv_ring_buffer, NULL, u)) ;
					u++, client_recv_counter++)
				{
					assert((b_t)(CLIENT_RECV_COUNTER_OFFSET + client_recv_counter) == b[0]);
				}
				if (0 < u)
				{
					espress_stream_ring_buffer->de(client_recv_ring_buffer, u);
					espress_stream_session->recv(client_session);
				}
			}
	
			espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(server_flags, f);
			if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_SESSION_RECV))
			{
				espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_SESSION_RECV);
				for (u = 0 ;
					NULL != (b = espress_stream_ring_buffer->tail(server_recv_ring_buffer, NULL, u)) ;
					u++, server_recv_counter++)
				{
					assert((b_t)(SERVER_RECV_COUNTER_OFFSET + server_recv_counter) == b[0]);
				}
				if (0 < u)
				{
					espress_stream_ring_buffer->de(server_recv_ring_buffer, u);
					espress_stream_session->recv(server_session);
				}
			}
	
			printf("session: client (%010u)\n", client_send_counter);
			i = espress_stream_session->rtt(client_session, client_rtt);
			if (0 == i)
			{
				espress_stream_uint32_get_u(client_rtt, &u);
				printf("\t-(%08u, ", u);
			}
			else
			{
				printf("\t-(--------, ");
			}
			espress_stream_session->rto(client_session, client_rto);
			espress_stream_uint32_get_u(client_rto, &u);
			printf("%08u, ", u);
			printf("%08u, ", espress_stream_session->send_timeout_count(client_session));
			printf("%02u, ", espress_stream_session->dac(client_session));
			printf("%08u, ", espress_stream_session->fast_retransmit_count(client_session));
			printf("%04u)->\n", espress_stream_session->cwnd(client_session));
			printf("\tserver (%010u)\n", server_recv_counter);
			printf("session: server (%010u)\n", server_send_counter);
			i = espress_stream_session->rtt(server_session, server_rtt);
			if (0 == i)
			{
				espress_stream_uint32_get_u(server_rtt, &u);
				printf("\t-(%08u, ", u);
			}
			else
			{
				printf("\t-(--------, ");
			}
			espress_stream_session->rto(server_session, server_rto);
			espress_stream_uint32_get_u(server_rto, &u);
			printf("%08u, ", u);
			printf("%08u, ", espress_stream_session->send_timeout_count(server_session));
			printf("%02u, ", espress_stream_session->dac(server_session));
			printf("%08u, ", espress_stream_session->fast_retransmit_count(server_session));
			printf("%04u)->\n", espress_stream_session->cwnd(server_session));
			printf("\tclient (%010u)\r", client_recv_counter);
			printf("\x1b[5A");
			fflush(stdout);
		}
		printf("\n\n\n\n\n\n");
	}

	i = espress_stream_session->disconnect(client_session);
	assert(0 == i);
	printf("session: client: disconnect\n");

	for ( ; ; )
	{
		_interface_yield(interface);
		_session_yield(client_session);
		_session_yield(server_session);
		espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
		espress_stream_event_flags->compo(client_flags, f);
		if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT))
		{
			espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT);
			i = espress_stream_session->disconnect_result(client_session);
			assert(0 == i);
			break;
		}
	}
	printf("session: client: disconnect_result\n");

	for ( ; ; )
	{
		_interface_yield(interface);
		_session_yield(client_session);
		_session_yield(server_session);
		espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
		espress_stream_event_flags->compo(server_flags, f);
		if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT))
		{
			espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT);
			i = espress_stream_session->disconnect_detect(server_session);
			assert(0 == i);
			break;
		}
	}
	printf("session: server: disconnect_detect\n");

	i = espress_stream_session->disconnect(server_session);
	assert(0 == i);
	printf("session: server: disconnect\n");

	for ( ; ; )
	{
		_interface_yield(interface);
		_session_yield(client_session);
		_session_yield(server_session);
		espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
		espress_stream_event_flags->compo(server_flags, f);
		if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT))
		{
			espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_RESULT);
			i = espress_stream_session->disconnect_result(server_session);
			assert(0 == i);
			break;
		}
	}
	printf("session: server: disconnect_result\n");

	for ( ; ; )
	{
		_interface_yield(interface);
		_session_yield(client_session);
		_session_yield(server_session);
		espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
		espress_stream_event_flags->compo(client_flags, f);
		if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT))
		{
			espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_SESSION_DISCONNECT_DETECT);
			i = espress_stream_session->disconnect_detect(client_session);
			assert(0 == i);
			break;
		}
	}
	printf("session: client: disconnect_detect\n");

	espress_stream_session->finalize(server_session);
	espress_stream_session->finalize(client_session);

	espress_stream_event->finalize(server_event);
	espress_stream_event->finalize(client_event);

	espress_stream_ring_buffer->finalize(server_recv_ring_buffer);
	espress_stream_ring_buffer->finalize(server_send_ring_buffer);
	espress_stream_ring_buffer->finalize(client_recv_ring_buffer);
	espress_stream_ring_buffer->finalize(client_send_ring_buffer);

	espress_stream_trans_packet->finalize(interface);
}
