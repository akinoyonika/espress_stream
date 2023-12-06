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
	this->counter = 0;
}

static void _final(struct espress_stream_mutex *this)
{
	if (0 != this->counter)
	{
		for ( ; ; );
	}
	memset(this, 0, sizeof(*this));
}

static void _lock(struct espress_stream_mutex *this)
{
	this->counter++;
}

static void _unlock(struct espress_stream_mutex *this)
{
	if (0 == this->counter)
	{
		for ( ; ; );
	}
	this->counter--;
}

static i_t _try_lock(struct espress_stream_mutex *this)
{
	_lock(this);
	return 0;
}
