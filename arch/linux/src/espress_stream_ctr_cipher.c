#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/block_cipher.h"
#include "espress_stream/ctr_cipher.h"

static void _initialize(struct espress_stream_ctr_cipher *this, b_t const *nonce, struct espress_stream_uint32 const *init_counter, b_t const *key);
static void _finalize(struct espress_stream_ctr_cipher *this);
static void _encrypt(struct espress_stream_ctr_cipher *this, b_t *text, u_t text_size);

struct espress_stream_ctr_cipher_ops const espress_stream_ctr_cipher[] =
{{
	_initialize,
	_finalize,
	_encrypt,
	_encrypt,
}};

static void _initialize(struct espress_stream_ctr_cipher *this, b_t const *nonce, struct espress_stream_uint32 const *init_counter, b_t const *key)
{
	espress_stream_block_cipher->initialize(this->block_cipher, key);
	memcpy(this->counter, nonce, ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE);
	memcpy(this->counter + ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE, init_counter, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE - ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE);
	this->total_size = 0;
}

static void _finalize(struct espress_stream_ctr_cipher *this)
{
	espress_stream_block_cipher->finalize(this->block_cipher);
	memset(this, 0, sizeof(*this));
}

static void _encrypt_block(struct espress_stream_ctr_cipher *this, b_t *text, u_t text_size, u_t counter)
{
	b_t *p;
	b_t *t;
	b_t *te;
	u_t offset;
	u_t inv_offset;
	struct espress_stream_uint32 u[1];
	b_t *cipher_counter;
	b_t _cipher_counter[ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE];

	offset = 0;
	if ((counter * ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE) < this->total_size)
	{
		offset = this->total_size - (counter * ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE);
	}
	inv_offset = 0;
	if ((this->total_size + text_size) < ((counter + 1) * ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE))
	{
		inv_offset = ((counter + 1) * ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE) - (this->total_size + text_size);
	}

	if (0 == offset)
	{
		if (0 == inv_offset)
		{
			cipher_counter = _cipher_counter;
		}
		else
		{
			cipher_counter = this->cipher_counter;
		}
		memcpy(cipher_counter, this->counter, ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE);
		espress_stream_uint32_set_u(u, counter);
		espress_stream_uint32_add((struct espress_stream_uint32 *)(cipher_counter + ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE), u);
		espress_stream_uint32_htob((struct espress_stream_uint32 *)(cipher_counter + ESPRESS_STREAM_CTR_CIPHER_NONCE_SIZE));
		espress_stream_block_cipher->encrypt(this->block_cipher, cipher_counter);
	}
	else
	{
		cipher_counter = this->cipher_counter;
	}

	for (p = cipher_counter + offset,
			t = text + (counter * ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE + offset - this->total_size),
			te = t + (ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE - offset - inv_offset) ;
		t < te ;
		p++, t++)
	{
		*t ^= *p;
	}
}

static void _encrypt(struct espress_stream_ctr_cipher *this, b_t *text, u_t text_size)
{
	u_t head = this->total_size / ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE;
	u_t tail = (this->total_size + text_size + (ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE - 1)) / ESPRESS_STREAM_CTR_CIPHER_BLOCK_SIZE;
	u_t counter;

#	pragma omp parallel for
	for (counter = head ; counter < tail ; counter++)
	{
		_encrypt_block(this, text, text_size, counter);
	}

	this->total_size += text_size;
}
