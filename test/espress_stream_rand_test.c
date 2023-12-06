#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/mutex.h"
#include "espress_stream/tick.h"
#include "espress_stream/rand.h"

static void _dump(b_t const *buffer, u_t size)
{
	u_t u;
	for (u = 0 ; u < size ; u++)
	{
		printf("%02x", buffer[u]);
	}
}

void espress_stream_rand_test()
{
	i_t i;
	b_t buffer[24];

	for (i = 0 ; i < sizeof(buffer) ; i++)
	{
		espress_stream_rand->get(buffer, i);
		printf("rand: "); _dump(buffer, i); printf("\n");
	}
}
