#ifndef __ESPRESS_STREAM_COND_H__
#define __ESPRESS_STREAM_COND_H__

#define ESPRESS_STREAM_COND_CLOCK_TYPE (CLOCK_MONOTONIC)

struct espress_stream_cond
{
	pthread_cond_t cond[1];
	pthread_mutex_t mutex[1];
	int event;
};

struct espress_stream_cond_ops
{
	void (*init)(struct espress_stream_cond *this);
	void (*final)(struct espress_stream_cond *this);
	void (*signal_lock)(struct espress_stream_cond *this);
	void (*signal_unlock)(struct espress_stream_cond *this);
	void (*signal)(struct espress_stream_cond *this);
	void (*wait_lock)(struct espress_stream_cond *this);
	void (*wait_unlock)(struct espress_stream_cond *this);
	i_t (*wait)(struct espress_stream_cond *this, struct espress_stream_uint32 const *timeout);
	i_t (*poll)(struct espress_stream_cond * const *these, u_t num, struct espress_stream_uint32 const *timeout);
};

extern struct espress_stream_cond_ops const espress_stream_cond[];

#endif /* __ESPRESS_STREAM_COND_H__ */
