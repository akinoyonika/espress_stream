#ifndef __ESPRESS_STREM_MUTEX_H__
#define __ESPRESS_STREM_MUTEX_H__

#define ESPRESS_STREAM_MUTEX_IS_MUTEX (1)
#define ESPRESS_STREAM_MUTEX_IS_SEMAPHRE (0)

struct espress_stream_mutex
{
#	if ESPRESS_STREAM_MUTEX_IS_SEMAPHRE
		sem_t sem[1];
#	else /* ESPRESS_STREAM_MUTEX_IS_MUTEX */
		pthread_mutex_t mutex[1];
#	endif
};

struct espress_stream_mutex_ops
{
	void (*init)(struct espress_stream_mutex *this);
	void (*final)(struct espress_stream_mutex *this);
	void (*lock)(struct espress_stream_mutex *this);
	void (*unlock)(struct espress_stream_mutex *this);
	i_t (*try_lock)(struct espress_stream_mutex *this);
};

extern struct espress_stream_mutex_ops const espress_stream_mutex[];

#endif /* __ESPRESS_STREM_MUTEX_H__ */
