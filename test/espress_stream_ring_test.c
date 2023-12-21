#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/ring.h"

void espress_stream_ring_test()
{
	i_t i;
	i_t j;
	u_t u;
	void * const *p;
	struct espress_stream_ring ring[1];
	u_t buffer1[4][5];
	u_t * const buffer[4] = {buffer1[0], buffer1[1], buffer1[2], buffer1[3]};
	char *data[] = {"0123", "4567", "89AB", "CDEF"};

	espress_stream_ring->initialize(ring, (void * const *)buffer, 5);
	p = espress_stream_ring->head(ring, 0);
	assert((void * const *)buffer == p);
	p = espress_stream_ring->tail(ring, 0);
	assert(NULL == p);
	i = espress_stream_ring->is_empty(ring);
	assert(i);
	i = espress_stream_ring->is_full(ring);
	assert(!i);
	u = espress_stream_ring->size(ring);
	assert(4 == u);
	u = espress_stream_ring->num_valid(ring);
	assert(0 == u);
	u = espress_stream_ring->num_invalid(ring);
	assert(4 == u);

	for (i = 0 ; i < 4 ; i++)
	{
		p = espress_stream_ring->head(ring, 0);
		strcpy((char *)p[0], data[i]);
		printf("ring: head = \"%s\" -> ", (char *)p[0]);
		j = espress_stream_ring->en(ring, 1);
		printf("en\n");
		assert(1 == j);
		printf("ring: ");
		for (j = 0 ; NULL != (p = espress_stream_ring->tail(ring, j)) ; j++)
		{
			printf("tail[%d] = \"%s\", ", j, (char *)p[0]);
			assert(0 == strcmp(data[j], (char *)p[0]));
		}
		printf("\n");
	}
	i = espress_stream_ring->is_empty(ring);
	assert(!i);
	i = espress_stream_ring->is_full(ring);
	assert(i);
	u = espress_stream_ring->size(ring);
	assert(4 == u);
	u = espress_stream_ring->num_valid(ring);
	assert(4 == u);
	u = espress_stream_ring->num_invalid(ring);
	assert(0 == u);

	for (i = 0 ; i < 4 ; i++)
	{
		printf("ring: ");
		for (j = 0 ; NULL != (p = espress_stream_ring->tail(ring, j)) ; j++)
		{
			printf("tail[%d] = \"%s\", ", j, (char *)p[0]);
			assert(0 == strcmp(data[(i + j) % 4], (char *)p[0]));
		}
		printf("\n");
		p = espress_stream_ring->tail(ring, 0);
		printf("ring: tail = \"%s\" -> ", (char *)p[0]);
		assert(0 == strcmp(data[i], (char *)p[0]));
		j = espress_stream_ring->de(ring, 1);
		printf("de\n");
		assert(1 == j);
	}
	p = espress_stream_ring->head(ring, 0);
	assert((void * const *)buffer == p);
	p = espress_stream_ring->tail(ring, 0);
	assert(NULL == p);
	i = espress_stream_ring->is_empty(ring);
	assert(i);
	i = espress_stream_ring->is_full(ring);
	assert(!i);
	u = espress_stream_ring->size(ring);
	assert(4 == u);
	u = espress_stream_ring->num_valid(ring);
	assert(0 == u);
	u = espress_stream_ring->num_invalid(ring);
	assert(4 == u);

	espress_stream_ring->finalize(ring);
}
