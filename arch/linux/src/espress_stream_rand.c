#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/rand.h"

static void _init(void);
static void _final(void);
static void _get(b_t *buffer, u_t size);

struct espress_stream_rand_ops const espress_stream_rand[] =
{{
	_init,
	_final,
	_get
}};

static void _init(void)
{
}

static void _final(void)
{
}

static void _get(b_t *buffer, u_t size)
{
	ssize_t s;
	while (0 < size)
	{
		s = getrandom(buffer, size, 0);
		if (0 < s)
		{
			buffer += s;
			size -= s;
		}
	}
}
