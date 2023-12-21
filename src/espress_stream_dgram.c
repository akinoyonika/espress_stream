#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/mutex.h"
#include "espress_stream/cond.h"
#include "espress_stream/event.h"
#include "espress_stream/ring.h"
#include "espress_stream/dgram.h"

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

	espress_stream_event->initialize(this->send_event);
	espress_stream_event->initialize(this->recv_event);
	espress_stream_mutex->initialize(this->send_mutex);
	espress_stream_mutex->initialize(this->recv_mutex);

	this->sock = socket(bind_address->storage->ss_family, SOCK_DGRAM | SOCK_NONBLOCK, 0);
	if (-1 == this->sock)
	{
		ret = errno;
		goto EXIT;
	}
	i = bind(this->sock, (struct sockaddr const *)bind_address->storage, sizeof(*(bind_address->storage)));
	if (-1 == i)
	{
		ret = errno;
		goto EXIT;
	}

	this->send_eventfd = eventfd(0, 0);
	if (-1 == this->send_eventfd)
	{
		ret = errno;
		goto EXIT;
	}
	this->num_send_buffer = 0;
	this->num_sent_buffer = 0;
	this->recv_eventfd = eventfd(0, 0);
	if (-1 == this->recv_eventfd)
	{
		ret = errno;
		goto EXIT;
	}
	this->num_recv_buffer = 0;
	this->num_rcvd_buffer = 0;

EXIT:

	if (0 != ret)
	{
		if (-1 != this->recv_eventfd)
		{
			close(this->recv_eventfd);
		}
		if (-1 != this->send_eventfd)
		{
			close(this->send_eventfd);
		}
		if (-1 != this->sock)
		{
			close(this->sock);
		}
		espress_stream_mutex->finalize(this->recv_mutex);
		espress_stream_mutex->finalize(this->send_mutex);
		espress_stream_event->finalize(this->recv_event);
		espress_stream_event->finalize(this->send_event);
	}

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
	(void)ex_buffer;
	(void)ex_num;
	espress_stream_mutex->lock(this->send_mutex);
	if ((0 == this->num_send_buffer) && (0 == this->num_sent_buffer))
	{
		this->send_buffer = buffer;
		this->num_send_buffer = num;
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
	(void)ex_buffer;
	(void)ex_num;
	espress_stream_mutex->lock(this->recv_mutex);
	if ((0 == this->num_recv_buffer) && (0 == this->num_rcvd_buffer))
	{
		this->recv_buffer = buffer;
		this->num_recv_buffer = num;
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
	return ESPRESS_STREAM_DGRAM_MTU;
}

static void _send_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout)
{
	struct espress_stream_event_flags f[1];
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	u_t t;
	ssize_t s;
	uint64_t w;
	struct pollfd fds[2] = {{this->send_eventfd, POLLIN, 0}, {this->sock, POLLOUT, 0}};

	espress_stream_tick->get_us(u);
	espress_stream_event->wait(this->send_event, f, timeout);

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
		sendto(this->sock,
			this->send_buffer[0]->data,
			this->send_buffer[0]->data_size,
			0,
			(struct sockaddr const *)this->send_buffer[0]->address->storage,
			sizeof(*(this->send_buffer[0]->address->storage)));
		if ((EAGAIN == errno) || (EWOULDBLOCK == errno))
		{
			espress_stream_event->set(this->send_event, 1);
		}
		else
		{
			this->num_send_buffer = 0;
			this->num_sent_buffer = 1;
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

	return;
}

static void _recv_yield(struct espress_stream_dgram *this, struct espress_stream_uint32 const *timeout)
{
	struct espress_stream_event_flags f[1];
	ssize_t r;
	socklen_t l;
	struct espress_stream_uint32 u[1];
	struct espress_stream_uint32 v[1];
	u_t t;
	ssize_t s;
	uint64_t w;
	struct pollfd fds[2] = {{this->recv_eventfd, POLLIN, 0}, {this->sock, POLLIN, 0}};

	espress_stream_tick->get_us(u);
	espress_stream_event->wait(this->recv_event, f, timeout);

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
		l = sizeof(*(this->recv_buffer[0]->address->storage));
		errno = 0;
		r = recvfrom(this->sock,
			this->recv_buffer[0]->data,
			this->recv_buffer[0]->data_size,
			0,
			(struct sockaddr *)this->recv_buffer[0]->address->storage,
			&l);
		if ((EAGAIN == errno) || (EWOULDBLOCK == errno))
		{
			espress_stream_event->set(this->recv_event, 1);
		}
		else
		{
			this->recv_buffer[0]->data_size = (0 < r) ? r : 0;
			this->num_recv_buffer = 0;
			this->num_rcvd_buffer = 1;
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
		for (s = 0 ; sizeof(w) != s ; s = read(this->send_eventfd, &w, sizeof(w)));
	}

	espress_stream_mutex->unlock(this->recv_mutex);

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
