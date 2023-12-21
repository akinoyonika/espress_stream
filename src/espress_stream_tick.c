#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/tick.h"

static void _initialize(void);
static void _finalize(void);
static void _get_us(struct espress_stream_uint32 *tick);

struct espress_stream_tick_ops const espress_stream_tick[] =
{{
	_initialize,
	_finalize,
	_get_us
}};

struct espress_stream_uint32 const espress_stream_tick_us_1_msc[] = {ESPRESS_STREAM_TICK_US_1_MSC};
struct espress_stream_uint32 const espress_stream_tick_us_1_sec[] = {ESPRESS_STREAM_TICK_US_1_SEC};
struct espress_stream_uint32 const espress_stream_tick_us_1_min[] = {ESPRESS_STREAM_TICK_US_1_MIN};
struct espress_stream_uint32 const espress_stream_tick_us_1_hor[] = {ESPRESS_STREAM_TICK_US_1_HOR};

static void _initialize()
{
}

static void _finalize()
{
}

static void _get_us(struct espress_stream_uint32 *tick)
{
	struct timespec t;
	unsigned long tv;

	clock_gettime(CLOCK_MONOTONIC, &t);

	tv = (unsigned long)(t.tv_sec) * 1000000UL + (unsigned long)(t.tv_nsec) / 1000UL;
	espress_stream_uint32_set(tick, (tv >> 24) & 0xff, (tv >> 16) & 0xff, (tv >> 8) & 0xff, tv & 0xff);
}
