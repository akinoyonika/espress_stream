#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"

static i_t _initialize(struct espress_stream_mutex *this);
static void _finalize(struct espress_stream_mutex *this);
static void _lock(struct espress_stream_mutex *this);
static void _unlock(struct espress_stream_mutex *this);
static i_t _try_lock(struct espress_stream_mutex *this);

struct espress_stream_mutex_ops const espress_stream_mutex[] =
{{
	_initialize,
	_finalize,
	_lock,
	_unlock,
	_try_lock
}};

static i_t _initialize(struct espress_stream_mutex *this)
{
	i_t ret = 0;
	pthread_mutexattr_t attr[1];
	pthread_mutexattr_init(attr);
	//pthread_mutexattr_settype(attr, PTHREAD_MUTEX_TIMED_NP);
	//pthread_mutexattr_settype(attr, PTHREAD_MUTEX_FAST_NP);
	//pthread_mutexattr_settype(attr, PTHREAD_MUTEX_ADAPTIVE_NP);
	pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE_NP);
	if (0 != pthread_mutex_init(this->mutex, attr))
	{
		ret = 1;
	}
	pthread_mutexattr_destroy(attr);
	return ret;
}

static void _finalize(struct espress_stream_mutex *this)
{
	pthread_mutex_destroy(this->mutex);
	memset(this, 0, sizeof(*this));
}

static void _lock(struct espress_stream_mutex *this)
{
	pthread_mutex_lock(this->mutex);
}

static void _unlock(struct espress_stream_mutex *this)
{
	pthread_mutex_unlock(this->mutex);
}

static i_t _try_lock(struct espress_stream_mutex *this)
{
	return 0 != pthread_mutex_trylock(this->mutex);
}
