#include <assert.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
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

static void _yield(struct espress_stream_trans_packet_interface *interface)
{
	i_t i;
	for (i = 0 ; NULL != espress_stream_trans_packet->yield[i] ; i++)
	{
		espress_stream_trans_packet->yield[i](interface, espress_stream_uint32_0);
	}
}

static void _espress_stream_trans_packet_test(
	struct espress_stream_event *client_event,
	struct espress_stream_event *server_event,
	struct espress_stream_event_flags *client_flags,
	struct espress_stream_event_flags *server_flags,
	struct espress_stream_dgram_address *client_client_address,
	struct espress_stream_dgram_address *server_server_address,
	struct espress_stream_trans_packet_interface *client_interface,
	struct espress_stream_trans_packet_interface *server_interface
)
{
	i_t i;
	u_t u;
	b_t b[4];
	struct espress_stream_event_flags f[1];
	struct espress_stream_dgram_address client_server_address[1];
	struct espress_stream_dgram_address server_client_address[1];
	struct espress_stream_trans_packet client_trans_packet[1];
	struct espress_stream_trans_packet server_trans_packet[1];
	struct espress_stream_uint32 client_connection_param[1];
	struct espress_stream_uint32 server_connection_param[1];
	struct espress_stream_uint32 client_payload_address[1];
	//struct espress_stream_uint32 server_payload_address[1];
	b_t client_payload[80];
	b_t server_payload[80];
	u_t client_payload_size;
	u_t client_add_payload_size;
	u_t server_payload_size;
	//u_t server_add_payload_size;
	struct espress_stream_uint32 client_receive_window_address[1];
	struct espress_stream_uint32 client_receive_window_size[1];
	i_t client_is_ack_true;
	struct espress_stream_uint32 client_send_data_address[1];
	struct espress_stream_uint32 server_receive_window_address[1];
	i_t server_is_ack_true;
	struct espress_stream_uint32 server_receive_window_size[1];
	struct espress_stream_uint32 server_send_data_address[1];

	i = espress_stream_trans_packet->open(client_trans_packet, client_interface, client_event, 0);
	assert(0 == i);
	i = espress_stream_trans_packet->open(server_trans_packet, server_interface, server_event, 1);
	assert(0 == i);

	((struct sockaddr_in *)(client_server_address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(client_server_address->storage))->sin_port = ((struct sockaddr_in *)(server_server_address->storage))->sin_port;
	((struct sockaddr_in *)(client_server_address->storage))->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	for ( ; ; )
	{
		espress_stream_uint32_assign(client_connection_param, espress_stream_uint32_0);
		i = espress_stream_trans_packet->send_connect(client_trans_packet, client_server_address, client_connection_param, 4096, (b_t *)"abcdefghijklmnopqrstuvwxyz", 26);
		if (0 <= i)
		{
			break;
		}
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(client_flags, f);
			if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND))
			{
				espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
				break;
			}
		}
	}
	assert(0 == i);
	printf("trans_packet: client(%s:%d) ", inet_ntoa(((struct sockaddr_in *)(client_client_address->storage))->sin_addr), ntohs(((struct sockaddr_in *)(client_client_address->storage))->sin_port));
	printf("- connect -> server(%s:%d)\n", inet_ntoa(((struct sockaddr_in *)(client_server_address->storage))->sin_addr), ntohs(((struct sockaddr_in *)(client_server_address->storage))->sin_port));

	for ( ; ; )
	{
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(server_flags, f);
			if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV))
			{
				espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
				break;
			}
		}
		espress_stream_uint32_assign(client_connection_param, espress_stream_uint32_max);
		memset(client_payload, 0, sizeof(client_payload));
		client_payload_size = sizeof(client_payload);
		i = espress_stream_trans_packet->recv_connect(server_trans_packet, server_client_address, client_connection_param, &u, client_payload, &client_payload_size);
		if (0 <= i)
		{
			espress_stream_trans_packet->purge(server_trans_packet);
			break;
		}
	}
	assert(0 == i);
	printf("trans_packet: server(%s:%d) ", inet_ntoa(((struct sockaddr_in *)(server_server_address->storage))->sin_addr), ntohs(((struct sockaddr_in *)(server_server_address->storage))->sin_port));
	printf("<- connect(initial_window_wize = %u) ", u);
	printf("- client(%s:%d)\n", inet_ntoa(((struct sockaddr_in *)(server_client_address->storage))->sin_addr), ntohs(((struct sockaddr_in *)(server_client_address->storage))->sin_port));
	assert(espress_stream_uint32_eq(client_connection_param, espress_stream_uint32_0));
	assert(4096 == u);
	assert((26 == client_payload_size) && (0 == memcmp((b_t *)"abcdefghijklmnopqrstuvwxyz", client_payload, client_payload_size)));


	for ( ; ; )
	{
		espress_stream_uint32_assign(server_connection_param, espress_stream_uint32_0);
		i = espress_stream_trans_packet->send_connect(server_trans_packet, server_client_address, server_connection_param, 8192, (b_t *)"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
		if (0 <= i)
		{
			break;
		}
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(server_flags, f);
			if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND))
			{
				espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
				break;
			}
		}
	}
	assert(0 == i);
	printf("trans_packet: server(%s:%d) ", inet_ntoa(((struct sockaddr_in *)(server_server_address->storage))->sin_addr), ntohs(((struct sockaddr_in *)(server_server_address->storage))->sin_port));
	printf("- connect -> client(%s:%d)\n", inet_ntoa(((struct sockaddr_in *)(server_client_address->storage))->sin_addr), ntohs(((struct sockaddr_in *)(server_client_address->storage))->sin_port));

	for ( ; ; )
	{
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(client_flags, f);
			if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV))
			{
				espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
				break;
			}
		}
		espress_stream_uint32_assign(server_connection_param, espress_stream_uint32_max);
		memset(server_payload, 0, sizeof(server_payload));
		server_payload_size = sizeof(server_payload);
		i = espress_stream_trans_packet->recv_connect(client_trans_packet, client_server_address, server_connection_param, &u, server_payload, &server_payload_size);
		if (0 <= i)
		{
			espress_stream_trans_packet->purge(client_trans_packet);
			break;
		}
	}
	assert(0 == i);
	printf("trans_packet: client(%s:%d) <- ", inet_ntoa(((struct sockaddr_in *)(client_client_address->storage))->sin_addr), ntohs(((struct sockaddr_in *)(client_client_address->storage))->sin_port));
	printf("connect(initial_window_wize = %u) ", u);
	printf("- server(%s:%d)\n", inet_ntoa(((struct sockaddr_in *)(client_server_address->storage))->sin_addr), ntohs(((struct sockaddr_in *)(client_server_address->storage))->sin_port));
	assert(espress_stream_uint32_eq(server_connection_param, espress_stream_uint32_0));
	assert(8192 == u);
	assert((26 == server_payload_size) && (0 == memcmp((b_t *)"ABCDEFGHIJKLMNOPQRSTUVWXYZ", server_payload, server_payload_size)));

	for ( ; ; )
	{
		espress_stream_uint32_set(client_receive_window_address, 0x00, 0x01, 0x02, 0x03);
		espress_stream_uint32_set(client_receive_window_size, 0x04, 0x05, 0x06, 0x07);
		espress_stream_uint32_set(client_send_data_address, 0x08, 0x09, 0x0a, 0x0b);
		i = espress_stream_trans_packet->send_envelope(client_trans_packet,
			client_receive_window_address,
			client_receive_window_size,
			client_send_data_address,
			(b_t *)"!@#$%^&*", 8,
			(b_t *)"()-_=+\\|`~[{]};:'\",<.>/?", 24);
		if (0 <= i)
		{
			break;
		}
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(client_flags, f);
			if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND))
			{
				espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
				break;
			}
		}
	}
	assert(0 == i);
	printf("trans_packet: client ");
	printf("- envelope -> server\n");

	for ( ; ; )
	{
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(server_flags, f);
			if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV))
			{
				espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
				break;
			}
		}
		espress_stream_uint32_assign(client_receive_window_address, espress_stream_uint32_0);
		espress_stream_uint32_assign(client_receive_window_size, espress_stream_uint32_0);
		espress_stream_uint32_set(client_payload_address, 0x08, 0x09, 0x0a, 0x0b);
		memset(client_payload, 0, sizeof(client_payload));
		client_payload_size = 24;
		client_add_payload_size = sizeof(client_payload) - 24;
		i = espress_stream_trans_packet->recv_envelope(server_trans_packet,
			client_receive_window_address,
			client_receive_window_size,
			&client_is_ack_true,
			client_send_data_address,
			client_payload_address, 
			client_payload, &client_payload_size,
			client_payload + 24, &client_add_payload_size);
		client_payload_size += client_add_payload_size;
		if (0 <= i)
		{
			espress_stream_trans_packet->purge(server_trans_packet);
			break;
		}
	}
	assert(0 == i);
	printf("trans_packet: server ");
	printf("<- envelope ");
	printf("- client\n");
	espress_stream_uint32_get(client_receive_window_address, b, b + 1, b + 2, b + 3);
	assert((0x00 == b[0]) && (0x01 == b[1]) && (0x02 == b[2]) && (0x03 == b[3]));
	espress_stream_uint32_get(client_receive_window_size, b, b + 1, b + 2, b + 3);
	assert((0x04 == b[0]) && (0x05 == b[1]) && (0x06 == b[2]) && (0x07 == b[3]));
	espress_stream_uint32_get(client_send_data_address, b, b + 1, b + 2, b + 3);
	assert((0x08 == b[0]) && (0x09 == b[1]) && (0x0a == b[2]) && (0x0b == b[3]));
	assert((32 == client_payload_size) && (0 == memcmp((b_t *)"!@#$%^&*()-_=+\\|`~[{]};:'\",<.>/?", client_payload, client_payload_size)));

	for ( ; ; )
	{
		espress_stream_uint32_set(client_receive_window_address, 0x00, 0x10, 0x20, 0x30);
		espress_stream_uint32_set(client_receive_window_size, 0x40, 0x50, 0x60, 0x70);
		i = espress_stream_trans_packet->send_ack(server_trans_packet,
			client_receive_window_address,
			client_receive_window_size);
		if (0 <= i)
		{
			break;
		}
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(server_flags, f);
			if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND))
			{
				espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
				break;
			}
		}
	}
	assert(0 == i);
	printf("trans_packet: server ");
	printf("- ack -> client\n");

	for ( ; ; )
	{
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(client_flags, f);
			if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV))
			{
				espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
				break;
			}
		}
		espress_stream_uint32_assign(server_receive_window_address, espress_stream_uint32_0);
		espress_stream_uint32_assign(server_receive_window_size, espress_stream_uint32_0);
		espress_stream_uint32_assign(server_send_data_address, espress_stream_uint32_0);
		i = espress_stream_trans_packet->recv_ack(client_trans_packet,
			server_receive_window_address,
			server_receive_window_size,
			&server_is_ack_true);
		if (0 <= i)
		{
			espress_stream_trans_packet->purge(client_trans_packet);
			break;
		}
	}
	assert(0 == i);
	printf("trans_packet: client ");
	printf("<- ack ");
	printf("- server\n");
	espress_stream_uint32_get(client_receive_window_address, b, b + 1, b + 2, b + 3);
	assert((0x00 == b[0]) && (0x10 == b[1]) && (0x20 == b[2]) && (0x30 == b[3]));
	espress_stream_uint32_get(client_receive_window_size, b, b + 1, b + 2, b + 3);
	assert((0x40 == b[0]) && (0x50 == b[1]) && (0x60 == b[2]) && (0x70 == b[3]));

	for ( ; ; )
	{
		i = espress_stream_trans_packet->send_disconnect(client_trans_packet, (b_t *)"0123456789", 10);
		if (0 <= i)
		{
			break;
		}
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(client_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(client_flags, f);
			if (espress_stream_event_flags->is_set(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND))
			{
				espress_stream_event_flags->reset(client_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_SEND);
				break;
			}
		}
	}
	assert(0 == i);
	printf("trans_packet: client ");
	printf("- disconnect -> server\n");

	for ( ; ; )
	{
		for ( ; ; )
		{
			_yield(client_interface);
			_yield(server_interface);
			espress_stream_event->wait(server_event, f, espress_stream_uint32_0);
			espress_stream_event_flags->compo(server_flags, f);
			if (espress_stream_event_flags->is_set(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV))
			{
				espress_stream_event_flags->reset(server_flags, ESPRESS_STREAM_EVENT_TRANS_PACKET_RECV);
				break;
			}
		}
		memset(client_payload, 0, sizeof(client_payload));
		client_payload_size = sizeof(client_payload);
		i = espress_stream_trans_packet->recv_disconnect(server_trans_packet, client_payload, &client_payload_size);
		if (0 <= i)
		{
			espress_stream_trans_packet->purge(server_trans_packet);
			break;
		}
	}
	assert(0 == i);
	printf("trans_packet: server ");
	printf("<- disconnect ");
	printf("- client\n");
	assert((10 == client_payload_size) && (0 == memcmp((b_t *)"0123456789", client_payload, client_payload_size)));

	espress_stream_trans_packet->close(server_trans_packet);
	espress_stream_trans_packet->close(client_trans_packet);
}

void espress_stream_trans_packet_test()
{
	i_t i;
	struct espress_stream_trans_packet_interface client_interface[1];
	struct espress_stream_trans_packet_interface server_interface[1];
	struct espress_stream_dgram_address client_client_address[1];
	struct espress_stream_dgram_address server_server_address[1];
	struct espress_stream_event client_event[1];
	struct espress_stream_event server_event[1];
	struct espress_stream_event_flags client_flags[1];
	struct espress_stream_event_flags server_flags[1];

	espress_stream_event_flags->reset_all(client_flags);
	espress_stream_event_flags->reset_all(server_flags);

	i = espress_stream_event->initialize(client_event);
	assert(0 == i);
	i = espress_stream_event->initialize(server_event);
	assert(0 == i);

	((struct sockaddr_in *)(client_client_address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(client_client_address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(client_client_address->storage))->sin_addr.s_addr = htonl(INADDR_ANY);
	i = espress_stream_trans_packet->initialize(client_interface, client_client_address);
	assert(0 == i);
	((struct sockaddr_in *)(server_server_address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(server_server_address->storage))->sin_port = htons(4321);
	((struct sockaddr_in *)(server_server_address->storage))->sin_addr.s_addr = htonl(INADDR_ANY);
	i = espress_stream_trans_packet->initialize(server_interface, server_server_address);
	assert(0 == i);

	_espress_stream_trans_packet_test(
		client_event,
		server_event,
		client_flags,
		server_flags,
		client_client_address,
		server_server_address,
		client_interface,
		server_interface
	);

	espress_stream_trans_packet->finalize(server_interface);
	espress_stream_trans_packet->finalize(client_interface);

	espress_stream_event->finalize(server_event);
	espress_stream_event->finalize(client_event);

	/****************************************************************/

	espress_stream_event_flags->reset_all(client_flags);

	i = espress_stream_event->initialize(client_event);
	assert(0 == i);

	((struct sockaddr_in *)(client_client_address->storage))->sin_family = AF_INET;
	((struct sockaddr_in *)(client_client_address->storage))->sin_port = htons(1234);
	((struct sockaddr_in *)(client_client_address->storage))->sin_addr.s_addr = htonl(INADDR_ANY);
	i = espress_stream_trans_packet->initialize(client_interface, client_client_address);
	assert(0 == i);

	_espress_stream_trans_packet_test(
		client_event,
		client_event,
		client_flags,
		client_flags,
		client_client_address,
		client_client_address,
		client_interface,
		client_interface
	);

	espress_stream_trans_packet->finalize(client_interface);

	espress_stream_event->finalize(client_event);

}
