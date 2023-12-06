#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"

static void _init(struct espress_stream_mutex *this);
static void _final(struct espress_stream_mutex *this);
static void _lock(struct espress_stream_mutex *this);
static void _unlock(struct espress_stream_mutex *this);
static i_t _try_lock(struct espress_stream_mutex *this);

struct espress_stream_mutex_ops const espress_stream_mutex[] =
{{
	_init,
	_final,
	_lock,
	_unlock,
	_try_lock
}};

static void _init(struct espress_stream_mutex *this)
{
#	if ESPRESS_STREAM_MUTEX_IS_SEMAPHRE
		sem_init(this->sem, 0, 1);
#	else /* ESPRESS_STREAM_MUTEX_IS_MUTEX */
		pthread_mutexattr_t attr[1];
		pthread_mutexattr_init(attr);
		//pthread_mutexattr_settype(attr, PTHREAD_MUTEX_TIMED_NP);
		//pthread_mutexattr_settype(attr, PTHREAD_MUTEX_FAST_NP);
		//pthread_mutexattr_settype(attr, PTHREAD_MUTEX_ADAPTIVE_NP);
		pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(this->mutex, attr);
		pthread_mutexattr_destroy(attr);
#	endif
}

static void _final(struct espress_stream_mutex *this)
{
#	if ESPRESS_STREAM_MUTEX_IS_SEMAPHRE
		sem_destroy(this->sem);
#	else /* ESPRESS_STREAM_MUTEX_IS_MUTEX */
		pthread_mutex_destroy(this->mutex);
#	endif
	memset(this, 0, sizeof(*this));
}

static void _lock(struct espress_stream_mutex *this)
{
#	if ESPRESS_STREAM_MUTEX_IS_SEMAPHRE
		sem_wait(this->sem);
#	else /* ESPRESS_STREAM_MUTEX_IS_MUTEX */
		pthread_mutex_lock(this->mutex);
#	endif
}

static void _unlock(struct espress_stream_mutex *this)
{
#	if ESPRESS_STREAM_MUTEX_IS_SEMAPHRE
		sem_post(this->sem);
#	else /* ESPRESS_STREAM_MUTEX_IS_MUTEX */
		pthread_mutex_unlock(this->mutex);
#	endif
}

static i_t _try_lock(struct espress_stream_mutex *this)
{
#	if ESPRESS_STREAM_MUTEX_IS_SEMAPHRE
		return 0 != sem_trywait(this->sem);
#	else /* ESPRESS_STREAM_MUTEX_IS_MUTEX */
		return 0 != pthread_mutex_trylock(this->mutex);
#	endif
}
