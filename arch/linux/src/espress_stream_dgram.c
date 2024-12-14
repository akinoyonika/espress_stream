#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"

#define _PF_INET_MAX_TRANS_SIZE (1280)
#define _PF_INET6_MAX_TRANS_SIZE (1280)
#define _PF_PACKET_MAX_TRANS_SIZE (1500)
#define _PF_PACKET_MIN_TRANS_SIZE (46)

#if (ESPRESS_STREAM_DGRAM_MTU < _PF_INET_MAX_TRANS_SIZE) || \
	(ESPRESS_STREAM_DGRAM_MTU < _PF_INET6_MAX_TRANS_SIZE) || \
	(ESPRESS_STREAM_DGRAM_MTU < _PF_PACKET_MAX_TRANS_SIZE)
#	error
#endif

static u8_t * const _pad0 = NULL;

#define _N(count) 1,
#define ESPRESS_STREAM_MACRO_REPEAT (1)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad1[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 2,
#define ESPRESS_STREAM_MACRO_REPEAT (2)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad2[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 3,
#define ESPRESS_STREAM_MACRO_REPEAT (3)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad3[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 4,
#define ESPRESS_STREAM_MACRO_REPEAT (4)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad4[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 5,
#define ESPRESS_STREAM_MACRO_REPEAT (5)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad5[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 6,
#define ESPRESS_STREAM_MACRO_REPEAT (6)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad6[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 7,
#define ESPRESS_STREAM_MACRO_REPEAT (7)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad7[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 8,
#define ESPRESS_STREAM_MACRO_REPEAT (8)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad8[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 9,
#define ESPRESS_STREAM_MACRO_REPEAT (9)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad9[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 10,
#define ESPRESS_STREAM_MACRO_REPEAT (10)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad10[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 11,
#define ESPRESS_STREAM_MACRO_REPEAT (11)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad11[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 12,
#define ESPRESS_STREAM_MACRO_REPEAT (12)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad12[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 13,
#define ESPRESS_STREAM_MACRO_REPEAT (13)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad13[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 14,
#define ESPRESS_STREAM_MACRO_REPEAT (14)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad14[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 15,
#define ESPRESS_STREAM_MACRO_REPEAT (15)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad15[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 16,
#define ESPRESS_STREAM_MACRO_REPEAT (16)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad16[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 17,
#define ESPRESS_STREAM_MACRO_REPEAT (17)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad17[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 18,
#define ESPRESS_STREAM_MACRO_REPEAT (18)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad18[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 19,
#define ESPRESS_STREAM_MACRO_REPEAT (19)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad19[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 20,
#define ESPRESS_STREAM_MACRO_REPEAT (20)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad20[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 21,
#define ESPRESS_STREAM_MACRO_REPEAT (21)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad21[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 22,
#define ESPRESS_STREAM_MACRO_REPEAT (22)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad22[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 23,
#define ESPRESS_STREAM_MACRO_REPEAT (23)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad23[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 24,
#define ESPRESS_STREAM_MACRO_REPEAT (24)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad24[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 25,
#define ESPRESS_STREAM_MACRO_REPEAT (25)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad25[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 26,
#define ESPRESS_STREAM_MACRO_REPEAT (26)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad26[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 27,
#define ESPRESS_STREAM_MACRO_REPEAT (27)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad27[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 28,
#define ESPRESS_STREAM_MACRO_REPEAT (28)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad28[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 29,
#define ESPRESS_STREAM_MACRO_REPEAT (29)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad29[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 30,
#define ESPRESS_STREAM_MACRO_REPEAT (30)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad30[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 31,
#define ESPRESS_STREAM_MACRO_REPEAT (31)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad31[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 32,
#define ESPRESS_STREAM_MACRO_REPEAT (32)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad32[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 33,
#define ESPRESS_STREAM_MACRO_REPEAT (33)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad33[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 34,
#define ESPRESS_STREAM_MACRO_REPEAT (34)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad34[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 35,
#define ESPRESS_STREAM_MACRO_REPEAT (35)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad35[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 36,
#define ESPRESS_STREAM_MACRO_REPEAT (36)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad36[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 37,
#define ESPRESS_STREAM_MACRO_REPEAT (37)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad37[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 38,
#define ESPRESS_STREAM_MACRO_REPEAT (38)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad38[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 39,
#define ESPRESS_STREAM_MACRO_REPEAT (39)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad39[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 40,
#define ESPRESS_STREAM_MACRO_REPEAT (40)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad40[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 41,
#define ESPRESS_STREAM_MACRO_REPEAT (41)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad41[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 42,
#define ESPRESS_STREAM_MACRO_REPEAT (42)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad42[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 43,
#define ESPRESS_STREAM_MACRO_REPEAT (43)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad43[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 44,
#define ESPRESS_STREAM_MACRO_REPEAT (44)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad44[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 45,
#define ESPRESS_STREAM_MACRO_REPEAT (45)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad45[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) 46,
#define ESPRESS_STREAM_MACRO_REPEAT (46)
#include "espress_stream/macro_repeat.h"
static u8_t const _pad46[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

#define _N(count) _pad ## count,
#define ESPRESS_STREAM_MACRO_REPEAT (_PF_PACKET_MIN_TRANS_SIZE + 1)
#include "espress_stream/macro_repeat.h"
static u8_t const * const _pad[] = {ESPRESS_STREAM_MACRO_REPEAT(_N)};
#undef ESPRESS_STREAM_MACRO_REPEAT
#undef _N

static i_t _initialize(struct espress_stream_dgram *this, struct espress_stream_dgram_address const *bind_address, struct espress_stream_event *send_event, struct espress_stream_event *recv_event);
static void _finalize(struct espress_stream_dgram *this);
static i_t _send_request(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num);
static i_t _send_result(struct espress_stream_dgram *this, u_t *num);
static i_t _recv_request(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num);
static i_t _recv_result(struct espress_stream_dgram *this, u_t *num);
static u_t _mtu(struct espress_stream_dgram *this);
static void _send_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout);
static void _recv_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout);
static void _send_yield_break(struct espress_stream_dgram *this);
static void _recv_yield_break(struct espress_stream_dgram *this);

struct espress_stream_dgram_ops const espress_stream_dgram[] =
{{
	_initialize,
	_finalize,
	_send_request,
	_send_result,
	_recv_request,
	_recv_result,
	_mtu,
	_send_yield,
	_recv_yield,
	_send_yield_break,
	_recv_yield_break
}};

static i_t _initialize(struct espress_stream_dgram *this, struct espress_stream_dgram_address const *bind_address, struct espress_stream_event *send_event, struct espress_stream_event *recv_event)
{
	i_t ret = 0;
	i_t i;

	this->sock = -1;
	this->send_eventfd = -1;
	this->recv_eventfd = -1;

	this->send_outer_event = send_event;
	this->recv_outer_event = recv_event;

	i = espress_stream_event->initialize(this->send_event);
	if (0 != i)
	{
		ret = 1;
		goto SEND_EVENT_INIT_FAIL_EXIT;
	}
	i = espress_stream_event->initialize(this->recv_event);
	if (0 != i)
	{
		ret = 1;
		goto RECV_EVENT_INIT_FAIL_EXIT;
	}
	i = espress_stream_mutex->initialize(this->send_mutex);
	if (0 != i)
	{
		ret = 1;
		goto SEND_MUTEX_INIT_FAIL_EXIT;
	}
	i = espress_stream_mutex->initialize(this->recv_mutex);
	if (0 != i)
	{
		ret = 1;
		goto RECV_MUTEX_INIT_FAIL_EXIT;
	}

	this->sock = socket(bind_address->storage->ss_family, SOCK_DGRAM | SOCK_NONBLOCK, 0);
	if (-1 == this->sock)
	{
		ret = errno;
		goto SOCKET_INIT_FAIL_EXIT;
	}
	if (PF_INET == bind_address->storage->ss_family)
	{
		i = 1;
		i = setsockopt(this->sock, SOL_SOCKET, SO_BROADCAST, &i, sizeof(i));
		if (-1 == i)
		{
			ret = errno;
			goto SETSOCKOPT_FAIL_EXIT;
		}
	}
	i = bind(this->sock, (struct sockaddr const *)(bind_address->storage), sizeof(*(bind_address->storage)));
	if (-1 == i)
	{
		ret = errno;
		goto BIND_FAIL_EXIT;
	}
	this->protocol_family = bind_address->storage->ss_family;

	this->send_eventfd = eventfd(0, 0);
	if (-1 == this->send_eventfd)
	{
		ret = errno;
		goto SEND_EVENTFD_INIT_FAIL_EXIT;
	}
	this->num_send_buffer = 0;
	this->num_sent_buffer = 0;
	this->recv_eventfd = eventfd(0, 0);
	if (-1 == this->recv_eventfd)
	{
		ret = errno;
		goto RECV_EVENTFD_INIT_FAIL_EXIT;
	}
	this->num_recv_buffer = 0;
	this->num_rcvd_buffer = 0;

RECV_EVENTFD_INIT_FAIL_EXIT:
	if (0 != ret) close(this->send_eventfd);
SEND_EVENTFD_INIT_FAIL_EXIT:
BIND_FAIL_EXIT:
SETSOCKOPT_FAIL_EXIT:
	if (0 != ret) close(this->sock);
SOCKET_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_mutex->finalize(this->recv_mutex);
RECV_MUTEX_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_mutex->finalize(this->send_mutex);
SEND_MUTEX_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_event->finalize(this->recv_event);
RECV_EVENT_INIT_FAIL_EXIT:
	if (0 != ret) espress_stream_event->finalize(this->send_event);
SEND_EVENT_INIT_FAIL_EXIT:

	return ret;
}

static void _finalize(struct espress_stream_dgram *this)
{
	close(this->recv_eventfd);
	close(this->send_eventfd);
	close(this->sock);
	espress_stream_mutex->finalize(this->recv_mutex);
	espress_stream_mutex->finalize(this->send_mutex);
	espress_stream_event->finalize(this->recv_event);
	espress_stream_event->finalize(this->send_event);
	memset(this, 0, sizeof(*this));
}

static i_t _send_request(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num)
{
	i_t ret = 0;
	espress_stream_mutex->lock(this->send_mutex);
	if ((0 == this->num_send_buffer) && (0 == this->num_sent_buffer))
	{
		this->send_buffer = buffer;
		this->num_send_buffer = num;
		this->send_ex_buffer = ex_buffer;
		if (NULL == ex_buffer)
		{
			this->num_send_ex_buffer = 0;
		}
		else
		{
			this->num_send_ex_buffer = ex_num;
		}
		espress_stream_event->set(this->send_event, 1);
	}
	else
	{
		ret = -1;
	}
	espress_stream_mutex->unlock(this->send_mutex);
	return ret;
}

static i_t _send_result(struct espress_stream_dgram *this, u_t *num)
{
	i_t ret;
	espress_stream_mutex->lock(this->send_mutex);
	if (0 == this->num_send_buffer)
	{
		*num = this->num_sent_buffer;
		ret = this->send_errno;
		this->num_sent_buffer = 0;
		this->send_errno = 0;
	}
	else
	{
		*num = 0;
		ret = -1;
	}
	espress_stream_mutex->unlock(this->send_mutex);
	return ret;
}

static i_t _recv_request(struct espress_stream_dgram *this, struct espress_stream_dgram_buffer * const *buffer, u_t num, struct espress_stream_dgram_buffer * const *ex_buffer, u_t ex_num)
{
	i_t ret = 0;
	espress_stream_mutex->lock(this->recv_mutex);
	if ((0 == this->num_recv_buffer) && (0 == this->num_rcvd_buffer))
	{
		this->recv_buffer = buffer;
		this->num_recv_buffer = num;
		this->recv_ex_buffer = ex_buffer;
		if (NULL == ex_buffer)
		{
			this->num_recv_ex_buffer = 0;
		}
		else
		{
			this->num_recv_ex_buffer = ex_num;
		}
		espress_stream_event->set(this->recv_event, 1);
	}
	else
	{
		ret = -1;
	}
	espress_stream_mutex->unlock(this->recv_mutex);
	return ret;
}

static i_t _recv_result(struct espress_stream_dgram *this, u_t *num)
{
	i_t ret;
	espress_stream_mutex->lock(this->recv_mutex);
	if (0 == this->num_recv_buffer)
	{
		*num = this->num_rcvd_buffer;
		ret = this->recv_errno;
		this->num_rcvd_buffer = 0;
		this->recv_errno = 0;
	}
	else
	{
		*num = 0;
		ret = -1;
	}
	espress_stream_mutex->unlock(this->recv_mutex);
	return ret;
}

static u_t _mtu(struct espress_stream_dgram *this)
{
	u_t ret;
	switch (this->protocol_family)
	{
	case PF_PACKET:
		ret = _PF_PACKET_MAX_TRANS_SIZE;
		break;
	case PF_INET6:
		ret = _PF_INET6_MAX_TRANS_SIZE;
		break;
	case PF_INET:
	default:
		ret = _PF_INET_MAX_TRANS_SIZE;
		break;
	}
	return ret;
}

static int _sendmmsg(struct espress_stream_dgram *this)
{
	u_t u;
	u_t v;
	struct espress_stream_dgram_buffer *b;
	struct mmsghdr mmsghdr[ESPRESS_STREAM_SEND_QUEUE_SIZE];
	struct iovec iovec[ESPRESS_STREAM_SEND_QUEUE_SIZE][2];

	v = this->num_send_buffer + this->num_send_ex_buffer;
	if (ESPRESS_STREAM_SEND_QUEUE_SIZE < v)
	{
		v = ESPRESS_STREAM_SEND_QUEUE_SIZE;
	}
	for (u = 0 ; u < v ; u++)
	{
		if (u < this->num_send_buffer)
		{
			b = this->send_buffer[u];
		}
		else
		{
			b = this->send_ex_buffer[u - this->num_send_buffer];
		}
		iovec[u][0].iov_base = b->data;
		iovec[u][0].iov_len = b->data_size;
		mmsghdr[u].msg_hdr.msg_name = (struct sockaddr *)(b->address->storage);
		mmsghdr[u].msg_hdr.msg_namelen = sizeof(*(this->send_buffer[0]->address->storage));
		mmsghdr[u].msg_hdr.msg_iov = iovec[u];
		mmsghdr[u].msg_hdr.msg_iovlen = 1;
		mmsghdr[u].msg_hdr.msg_control = NULL;
		mmsghdr[u].msg_hdr.msg_controllen = 0;
		mmsghdr[u].msg_hdr.msg_flags = 0;
		mmsghdr[u].msg_len = b->data_size;
		if ((PF_PACKET == ((struct sockaddr *)(mmsghdr[u].msg_hdr.msg_name))->sa_family) && (mmsghdr[u].msg_len < _PF_PACKET_MIN_TRANS_SIZE))
		{
			iovec[u][1].iov_len = _PF_PACKET_MIN_TRANS_SIZE - mmsghdr[u].msg_len;
			iovec[u][1].iov_base = (b_t *)_pad[iovec[u][1].iov_len];
			mmsghdr[u].msg_hdr.msg_iovlen = 2;
			mmsghdr[u].msg_len = _PF_PACKET_MIN_TRANS_SIZE;
		}
	}

	return sendmmsg(this->sock, mmsghdr, this->num_send_buffer, 0);
}

static void _send_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout)
{
	int i;
	struct espress_stream_event_flags f[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	u_t t;
	ssize_t s;
	uint64_t w;
	struct pollfd fds[2] = {{this->send_eventfd, POLLIN, 0}, {this->sock, POLLOUT, 0}};

	espress_stream_tick->get_us(u);
	espress_stream_event->wait(this->send_event, f, timeout);
	if (!espress_stream_event_flags->is_set_any(f))
	{
		goto EXIT;
	}

	espress_stream_tick->get_us(v);
	espress_stream_uint32_sub(v, u);
	if (espress_stream_uint32_lt(v, timeout))
	{
		espress_stream_uint32_assign(u, timeout);
		espress_stream_uint32_sub(u, v);
		espress_stream_uint32_div(u, espress_stream_tick_us_1_msc);
		espress_stream_uint32_get_u(u, &t);
	}
	else
	{
		t = 0;
	}
	poll(fds, 2, t);

	espress_stream_mutex->lock(this->send_mutex);

	if ((0 < this->num_send_buffer) && (0 == this->num_sent_buffer))
	{
		errno = 0;
		i = _sendmmsg(this);
		if ((EAGAIN == errno) || (EWOULDBLOCK == errno))
		{
			espress_stream_event->set(this->send_event, 1);
		}
		else
		{
			this->num_send_buffer = 0;
			this->num_sent_buffer = (0 < i) ? i : 0;
			this->send_errno = errno;
			espress_stream_event->set(this->send_outer_event, ESPRESS_STREAM_EVENT_DGRAM_SEND_RESULT);
		}
	}
	else
	{
		espress_stream_event->set(this->send_outer_event, ESPRESS_STREAM_EVENT_DGRAM_SEND_RESULT);
	}

	if (POLLIN == fds[0].revents)
	{
		for (s = 0 ; sizeof(w) != s ; s = read(this->send_eventfd, &w, sizeof(w)));
	}

	espress_stream_mutex->unlock(this->send_mutex);

EXIT:

	return;
}

static int _recvmmsg(struct espress_stream_dgram *this)
{
	int ret;
	u_t u;
	u_t v;
	struct espress_stream_dgram_buffer *b;
	struct mmsghdr mmsghdr[ESPRESS_STREAM_RECV_QUEUE_SIZE];
	struct iovec iovec[ESPRESS_STREAM_RECV_QUEUE_SIZE];
	u_t pad_size;
	u_t msg_len;

	v = this->num_recv_buffer + this->num_recv_ex_buffer;
	if (ESPRESS_STREAM_RECV_QUEUE_SIZE < v)
	{
		v = ESPRESS_STREAM_RECV_QUEUE_SIZE;
	}
	for (u = 0 ; u < v ; u++)
	{
		if (u < this->num_recv_buffer)
		{
			b = this->recv_buffer[u];
		}
		else
		{
			b = this->recv_ex_buffer[u - this->num_recv_buffer];
		}
		iovec[u].iov_base = b->data;
		iovec[u].iov_len = b->data_size;
		mmsghdr[u].msg_hdr.msg_name = (struct sockaddr *)(b->address->storage);
		mmsghdr[u].msg_hdr.msg_namelen = sizeof(*(b->address->storage));
		mmsghdr[u].msg_hdr.msg_iov = iovec + u;
		mmsghdr[u].msg_hdr.msg_iovlen = 1;
		mmsghdr[u].msg_hdr.msg_control = NULL;
		mmsghdr[u].msg_hdr.msg_controllen = 0;
		mmsghdr[u].msg_hdr.msg_flags = 0;
		mmsghdr[u].msg_len = 0;
	}

	ret = recvmmsg(this->sock, mmsghdr, this->num_recv_buffer, 0, NULL);

	if (0 < ret)
	{
		for (u = 0 ; u < ret ; u++)
		{
			if ((PF_PACKET == ((struct sockaddr *)(mmsghdr[u].msg_hdr.msg_name))->sa_family) && (_PF_PACKET_MIN_TRANS_SIZE == mmsghdr[u].msg_len))
			{
				pad_size = ((b_t *)(iovec[u].iov_base))[_PF_PACKET_MIN_TRANS_SIZE - 1];
				if ((0 < pad_size) && (pad_size <= _PF_PACKET_MIN_TRANS_SIZE))
				{
					msg_len = _PF_PACKET_MIN_TRANS_SIZE - pad_size;
					if (0 == memcmp(_pad[pad_size], ((b_t *)(iovec[u].iov_base)) + msg_len, pad_size))
					{
						mmsghdr[u].msg_len = msg_len;
					}
				}
			}
			if (u < this->num_recv_buffer)
			{
				b = this->recv_buffer[u];
			}
			else
			{
				b = this->recv_ex_buffer[u - this->num_recv_buffer];
			}
			b->data_size = mmsghdr[u].msg_len;
		}
	}

	return ret;
}

static void _recv_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout)
{
	int i;
	struct espress_stream_event_flags f[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	u_t t;
	ssize_t s;
	uint64_t w;
	struct pollfd fds[2] = {{this->recv_eventfd, POLLIN, 0}, {this->sock, POLLIN, 0}};

	espress_stream_tick->get_us(u);
	espress_stream_event->wait(this->recv_event, f, timeout);
	if (!espress_stream_event_flags->is_set_any(f))
	{
		goto EXIT;
	}

	espress_stream_tick->get_us(v);
	espress_stream_uint32_sub(v, u);
	if (espress_stream_uint32_lt(v, timeout))
	{
		espress_stream_uint32_assign(u, timeout);
		espress_stream_uint32_sub(u, v);
		espress_stream_uint32_div(u, espress_stream_tick_us_1_msc);
		espress_stream_uint32_get_u(u, &t);
	}
	else
	{
		t = 0;
	}
	poll(fds, 2, t);

	espress_stream_mutex->lock(this->recv_mutex);

	if ((0 < this->num_recv_buffer) && (0 == this->num_rcvd_buffer))
	{
		errno = 0;
		i = _recvmmsg(this);
		if ((EAGAIN == errno) || (EWOULDBLOCK == errno))
		{
			espress_stream_event->set(this->recv_event, 1);
		}
		else
		{
			this->num_recv_buffer = 0;
			this->num_rcvd_buffer = (0 < i) ? i : 0;
			this->recv_errno = errno;
			espress_stream_event->set(this->recv_outer_event, ESPRESS_STREAM_EVENT_DGRAM_RECV_RESULT);
		}
	}
	else
	{
		espress_stream_event->set(this->recv_outer_event, ESPRESS_STREAM_EVENT_DGRAM_RECV_RESULT);
	}

	if (POLLIN == fds[0].revents)
	{
		for (s = 0 ; sizeof(w) != s ; s = read(this->recv_eventfd, &w, sizeof(w)));
	}

	espress_stream_mutex->unlock(this->recv_mutex);

EXIT:

	return;
}

static void _send_yield_break(struct espress_stream_dgram *this)
{
	ssize_t s;
	uint64_t const u = 1;
	espress_stream_event->set(this->send_event, 0);
	for (s = 0 ; sizeof(u) != s ; s = write(this->send_eventfd, &u, sizeof(u)));
}

static void _recv_yield_break(struct espress_stream_dgram *this)
{
	ssize_t s;
	uint64_t const u = 1;
	espress_stream_event->set(this->recv_event, 0);
	for (s = 0 ; sizeof(u) != s ; s = write(this->recv_eventfd, &u, sizeof(u)));
}
