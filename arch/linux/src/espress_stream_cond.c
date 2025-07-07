#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"
#include "espress_stream/cond.h"

static i_t _initialize(struct espress_stream_cond *this);
static void _finalize(struct espress_stream_cond *this);
static void _signal_lock(struct espress_stream_cond *this);
static void _signal_unlock(struct espress_stream_cond *this);
static void _signal(struct espress_stream_cond *this);
static void _wait_lock(struct espress_stream_cond *this);
static void _wait_unlock(struct espress_stream_cond *this);
static i_t _wait(struct espress_stream_cond *this, struct espress_stream_uint32 const *timeout);
static i_t _poll(struct espress_stream_cond * const *these, u_t num, struct espress_stream_uint32 const *timeout);

struct espress_stream_cond_ops const espress_stream_cond[] =
{{
	_initialize,
	_finalize,
	_signal_lock,
	_signal_unlock,
	_signal,
	_wait_lock,
	_wait_unlock,
	_wait,
	_poll
}};

static void _event_initialize(int *event)
{
	*event = eventfd(0, EFD_CLOEXEC);
}

static void _event_destroy(int event)
{
	close(event);
}

static void _event_signal(int event)
{
	uint64_t const buf[1] = {1};
	while (sizeof(*buf) != write(event, buf, sizeof(*buf)));
}

static i_t _is_event_signaled(int event)
{
	struct pollfd fds[1] = {{event, POLLIN, 0}};
	return 0 < poll(fds, 1, 0);
}

static i_t _reset_event_signal(int event)
{
	i_t ret = 0;
	struct pollfd fds[1] = {{event, POLLIN, 0}};
	uint64_t buf[1];
	if (0 < poll(fds, 1, 0))
	{
		while (sizeof(*buf) != read(event, buf, sizeof(*buf)));
		ret = 1;
	}
	return ret;
}

static i_t _initialize(struct espress_stream_cond *this)
{
	i_t ret = 0;
	pthread_condattr_t condattr[1];
	pthread_mutexattr_t mutexattr[1];
	pthread_condattr_init(condattr);
	pthread_condattr_setclock(condattr, ESPRESS_STREAM_COND_CLOCK_TYPE);
	if (0 != pthread_cond_init(this->cond, condattr))
	{
		ret = 1;
		goto COND_INIT_FAIL_EXIT;
	}
	pthread_mutexattr_init(mutexattr);
	pthread_mutexattr_settype(mutexattr, PTHREAD_MUTEX_TIMED_NP);
	//pthread_mutexattr_settype(mutexattr, PTHREAD_MUTEX_FAST_NP);
	//pthread_mutexattr_settype(mutexattr, PTHREAD_MUTEX_ADAPTIVE_NP);
	//pthread_mutexattr_settype(mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
	if (0 != pthread_mutex_init(this->mutex, mutexattr))
	{
		ret = 1;
		goto MUTEX_INIT_FAIL_EXIT;
	}
	_event_initialize(&(this->event));
	if (-1 == this->event)
	{
		ret = 1;
		goto EVENTFD_INIT_FAIL_EXIT;
	}
EVENTFD_INIT_FAIL_EXIT:
	if (0 != ret) pthread_mutex_destroy(this->mutex);
MUTEX_INIT_FAIL_EXIT:
	if (0 != ret) pthread_cond_destroy(this->cond);
	pthread_mutexattr_destroy(mutexattr);
COND_INIT_FAIL_EXIT:
	pthread_condattr_destroy(condattr);
	return ret;
}

static void _finalize(struct espress_stream_cond *this)
{
	_event_destroy(this->event);
	pthread_mutex_destroy(this->mutex);
	pthread_cond_destroy(this->cond);
	memset(this, 0, sizeof(*this));
}

static void _signal_lock(struct espress_stream_cond *this)
{
	pthread_mutex_lock(this->mutex);
}

static void _signal_unlock(struct espress_stream_cond *this)
{
	pthread_mutex_unlock(this->mutex);
}

static void _signal(struct espress_stream_cond *this)
{
	_event_signal(this->event);
	pthread_cond_signal(this->cond);
}

static void _wait_lock(struct espress_stream_cond *this)
{
	pthread_mutex_lock(this->mutex);
}

static void _wait_unlock(struct espress_stream_cond *this)
{
	pthread_mutex_unlock(this->mutex);
}

static i_t _wait(struct espress_stream_cond *this, struct espress_stream_uint32 const *timeout)
{
	i_t ret = 0;
	u_t t;
	struct timespec ts[1];

	espress_stream_uint32_get_u(timeout, &t);
	if (1000 < t)
	{
		clock_gettime(ESPRESS_STREAM_COND_CLOCK_TYPE, ts);
		ts->tv_sec += t / 1000000;
		ts->tv_nsec += (t % 1000000) * 1000;
		if (1000000000 <= ts->tv_nsec)
		{
			ts->tv_sec++;
			ts->tv_nsec -= 1000000000;
		}
		while (!_is_event_signaled(this->event) && (0 == ret))
		{
			ret = pthread_cond_timedwait(this->cond, this->mutex, ts);
		}
	}

	ret = _reset_event_signal(this->event);

	return ret;
}

static i_t _poll(struct espress_stream_cond * const *these, u_t num, struct espress_stream_uint32 const *timeout)
{
	i_t ret;
	u_t u;
	int nfds;
	fd_set rfds[1];
	struct timeval tv[1];

	FD_ZERO(rfds);
	for (u = 0, nfds = 0 ; u < num ; u++)
	{
		if (nfds < these[u]->event)
		{
			nfds = these[u]->event;
		}
		FD_SET(these[u]->event, rfds);
	}
	nfds++;
	espress_stream_uint32_get_u(timeout, &u);
	tv->tv_sec = u / 1000000;
	tv->tv_usec =  u % 1000000;
	ret = select(nfds, rfds, NULL, NULL, tv);

	return ret;
}
