#ifndef __ESPRESS_STREAM_BLOCK_CIPHER_H__
#define __ESPRESS_STREAM_BLOCK_CIPHER_H__

#define ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE (4 * 4)
#define ESPRESS_STREAM_BLOCK_CIPHER_KEY_SIZE (8 * 4)

struct espress_stream_block_cipher
{
	int op_sock;
	int sock;
};

struct espress_stream_block_cipher_ops
{
	void (*initialize)(struct espress_stream_block_cipher *this, b_t const *key);
	void (*finalize)(struct espress_stream_block_cipher *this);
	void (*encrypt)(struct espress_stream_block_cipher *this, b_t *block);
};

extern struct espress_stream_block_cipher_ops espress_stream_block_cipher[];

#endif /* __ESPRESS_STREAM_BLOCK_CIPHER_H__ */
