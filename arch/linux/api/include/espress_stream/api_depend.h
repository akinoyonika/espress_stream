#ifndef __ESPRESS_STREAM_API_DEPEND_H__
#define __ESPRESS_STREAM_API_DEPEND_H__

#include <netinet/ip.h>

#if ESPRESS_STREAM_API_IS_THREAD_ENABLED
	struct thread {pthread_t pthread;};
#	define thread_create(thread, routine, param) pthread_create(&((thread)->pthread), NULL, routine, param)
#	define thread_join(thread) pthread_join((thread)->pthread, NULL)
#endif

#endif /* __ESPRESS_STREAM_API_DEPEND_H__ */