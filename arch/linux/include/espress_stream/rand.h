#ifndef __ESPRESS_STREAM_RAND_H__
#define __ESPRESS_STREAM_RAND_H__

struct espress_stream_rand_ops
{
	void (*init)(void);
	void (*final)(void);
	void (*get)(b_t *buffer, u_t size);
};

extern struct espress_stream_rand_ops const espress_stream_rand[];

#endif /* __ESPRESS_STREAM_RAND_H__ */
