#include <assert.h>
#include <stdio.h>

#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#if MAX_U_T_WIDTH < 4
#	include "espress_stream/mutex.h"
#endif
#include "espress_stream/ring_buffer.h"

void espress_stream_ring_buffer_test()
{
	i_t i;
	i_t j;
	i_t k;
	u_t u;
	b_t *p;
	b_t b0;
	b_t b1;
	b_t b2;
	b_t b3;
	struct espress_stream_ring_buffer ring_buffer[1];
	struct espress_stream_uint32 address[1];
	struct espress_stream_uint32 head_address[1];
	struct espress_stream_uint32 tail_address[1];
	b_t buffer[16];
	b_t data[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	espress_stream_ring_buffer->initialize(ring_buffer, buffer, 17);
	p = espress_stream_ring_buffer->head(ring_buffer, head_address, 0);
	assert((buffer == p) && espress_stream_uint32_eq(espress_stream_uint32_0, head_address));
	p = espress_stream_ring_buffer->tail(ring_buffer, tail_address, 0);
	assert((NULL == p) && espress_stream_uint32_eq(espress_stream_uint32_0, tail_address));
	i = espress_stream_ring_buffer->is_empty(ring_buffer);
	assert(i);
	i = espress_stream_ring_buffer->is_full(ring_buffer);
	assert(!i);
	u = espress_stream_ring_buffer->size(ring_buffer);
	assert(16 == u);
	u = espress_stream_ring_buffer->num_valid(ring_buffer);
	assert(0 == u);
	u = espress_stream_ring_buffer->num_invalid(ring_buffer);
	assert(16 == u);

	for (k = 0 ; k < 4 ; k++)
	{
		for (i = 0 ; i < 16 ; i++)
		{
			p = espress_stream_ring_buffer->head(ring_buffer, address, 0);
			p[0] = data[i];
			printf("ring_buffer: head = '%c' -> ", p[0]);
			j = espress_stream_ring_buffer->en(ring_buffer, 1);
			espress_stream_uint32_add(head_address, espress_stream_uint32_1);
			printf("en\n");
			assert(1 == j);
			espress_stream_ring_buffer->head(ring_buffer, address, 0);
			assert(espress_stream_uint32_eq(head_address, address));
			printf("ring_buffer:\n");
			for (j = 0 ; NULL != (p = espress_stream_ring_buffer->tail(ring_buffer, address, j)) ; j++)
			{
				espress_stream_uint32_get(address, &b0, &b1, &b2, &b3);
				printf("\ttail[%d(%02x%02x%02x%02x)] = '%c', \n", j, b0, b1, b2, b3, p[0]);
				assert((espress_stream_ring_buffer->valid_data(ring_buffer, address) == p) && (data[j] == p[0]));
			}
		}
		i = espress_stream_ring_buffer->is_empty(ring_buffer);
		assert(!i);
		i = espress_stream_ring_buffer->is_full(ring_buffer);
		assert(i);
		u = espress_stream_ring_buffer->size(ring_buffer);
		assert(16 == u);
		u = espress_stream_ring_buffer->num_valid(ring_buffer);
		assert(16 == u);
		u = espress_stream_ring_buffer->num_invalid(ring_buffer);
		assert(0 == u);

		for (i = 0 ; i < 16 ; i++)
		{
			printf("ring_buffer:\n");
			for (j = 0 ; NULL != (p = espress_stream_ring_buffer->tail(ring_buffer, address, j)) ; j++)
			{
				espress_stream_uint32_get(address, &b0, &b1, &b2, &b3);
				printf("\ttail[%d(%02x%02x%02x%02x)] = '%c', \n", j, b0, b1, b2, b3, p[0]);
				assert(data[(i + j) % 16] == p[0]);
			}
			p = espress_stream_ring_buffer->tail(ring_buffer, address, 0);
			assert(espress_stream_uint32_eq(tail_address, address));
			printf("ring_buffer: tail = '%c' -> ", p[0]);
			assert((espress_stream_ring_buffer->valid_data(ring_buffer, address) == p) && (data[i] == p[0]));
			j = espress_stream_ring_buffer->de(ring_buffer, 1);
			espress_stream_uint32_add(tail_address, espress_stream_uint32_1);
			printf("de\n");
			assert(1 == j);
		}
		p = espress_stream_ring_buffer->head(ring_buffer, address, 0);
		assert((buffer == p) && espress_stream_uint32_eq(head_address, address));
		p = espress_stream_ring_buffer->tail(ring_buffer, address, 0);
		assert((NULL == p) && espress_stream_uint32_eq(tail_address, address));
		i = espress_stream_ring_buffer->is_empty(ring_buffer);
		assert(i);
		i = espress_stream_ring_buffer->is_full(ring_buffer);
		assert(!i);
		u = espress_stream_ring_buffer->size(ring_buffer);
		assert(16 == u);
		u = espress_stream_ring_buffer->num_valid(ring_buffer);
		assert(0 == u);
		u = espress_stream_ring_buffer->num_invalid(ring_buffer);
		assert(16 == u);
	}

	espress_stream_ring_buffer->finalize(ring_buffer);
}
