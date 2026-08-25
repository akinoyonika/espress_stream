#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/block_cipher.h"

static void _initialize(struct espress_stream_block_cipher *this, b_t const *key);
static void _finalize(struct espress_stream_block_cipher *this);
static void _encrypt(struct espress_stream_block_cipher *this, b_t *block);

struct espress_stream_block_cipher_ops espress_stream_block_cipher[] =
{{
	_initialize,
	_finalize,
	_encrypt
}};

static void _initialize(struct espress_stream_block_cipher *this, b_t const *key)
{
	int ret = 0;
	struct sockaddr_alg sa[1] = {{0}};
	struct msghdr msg[1] = {{0}};
	struct
	{
		struct cmsghdr cmsg_op[1];
		uint32_t op;
		struct cmsghdr cmsg_iv[1];
		struct af_alg_iv af_alg_iv[1];
	} cmsg[1];

	sa->salg_family = AF_ALG;
	strcpy((char *)sa->salg_type, "skcipher");
	strcpy((char *)sa->salg_name, "ecb(aes)");

	this->op_sock = socket(PF_ALG, SOCK_SEQPACKET | SOCK_CLOEXEC, 0);
	if (-1 == this->op_sock)
	{
		ret = 1;
		goto SOCKET_ERROR_EXIT;
	}
	if (-1 == bind(this->op_sock, (struct sockaddr *)sa, sizeof(*sa)))
	{
		ret = 1;
		goto BIND_ERROR_EXIT;
	}
	if (-1 == setsockopt(this->op_sock, SOL_ALG, ALG_SET_KEY, key, ESPRESS_STREAM_BLOCK_CIPHER_KEY_SIZE))
	{
		ret = 1;
		goto SETSOCKOPT_ERROR_EXIT;
	}
	this->sock = accept(this->op_sock, NULL, 0);
	if (-1 == this->sock)
	{
		ret = 1;
		goto ACCEPT_ERROR_EXIT;
	}

	msg->msg_control = cmsg;
	msg->msg_controllen = sizeof(*cmsg);
	cmsg->cmsg_op->cmsg_level = SOL_ALG;
	cmsg->cmsg_op->cmsg_type = ALG_SET_OP;
	cmsg->cmsg_op->cmsg_len = CMSG_LEN(sizeof(uint32_t));
	cmsg->op = ALG_OP_ENCRYPT;
	cmsg->cmsg_iv->cmsg_level = SOL_ALG;
	cmsg->cmsg_iv->cmsg_type = ALG_SET_IV;
	cmsg->cmsg_iv->cmsg_len = CMSG_LEN(sizeof(struct af_alg_iv));
	cmsg->af_alg_iv->ivlen = 0;

	if (sendmsg(this->sock, msg, MSG_MORE) == -1)
	{
		ret = 1;
		goto SENDMSG_ERROR_EXIT;
	}

SENDMSG_ERROR_EXIT:
ACCEPT_ERROR_EXIT:
SETSOCKOPT_ERROR_EXIT:
BIND_ERROR_EXIT:
	if (0 != ret) close(this->op_sock);
SOCKET_ERROR_EXIT:

	if (0 != ret) abort();
}

static void _finalize(struct espress_stream_block_cipher *this)
{
	close(this->sock);
	close(this->op_sock);
}

static void _encrypt(struct espress_stream_block_cipher *this, b_t *block)
{
	int ret = 0;
	struct msghdr msg[1] = {{0}};
	struct iovec iov[1];

	iov->iov_base = block;
	iov->iov_len = ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE;
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;

	if (-1 == sendmsg(this->sock, msg, 0))
	{
		ret = 1;
		goto EXIT;
	}
	if (-1 == read(this->sock, block, ESPRESS_STREAM_BLOCK_CIPHER_BLOCK_SIZE))
	{
		ret = 1;
		goto EXIT;
	}

EXIT:
	if (0 != ret) close(this->sock);
	if (0 != ret) close(this->op_sock);

	if (0 != ret) abort();
}
