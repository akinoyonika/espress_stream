#include "espress_stream/config.h"
#include "espress_stream/depend.h"
#include "espress_stream/uint.h"
#include "espress_stream/hash.h"

static void _initialize(struct espress_stream_hash *this);
static void _finalize(struct espress_stream_hash *this, b_t *digest);
static void _update(struct espress_stream_hash *this, b_t const *message, u_t size);

struct espress_stream_hash_ops const espress_stream_hash[] =
{{
	_initialize,
	_finalize,
	_update
}};

static void _initialize(struct espress_stream_hash *this)
{
	int ret = 0;
	struct sockaddr_alg sa[1] = {{0}};

	sa->salg_family = AF_ALG;
	strcpy((char *)sa->salg_type, "hash");
	strcpy((char *)sa->salg_name, "sha256");

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
	this->sock = accept(this->op_sock, NULL, 0);
	if (-1 == this->sock)
	{
		ret = 1;
		goto ACCEPT_ERROR_EXIT;
	}

ACCEPT_ERROR_EXIT:
BIND_ERROR_EXIT:
	if (0 != ret) close(this->op_sock);
SOCKET_ERROR_EXIT:

	if (0 != ret) abort();
}

static void _finalize(struct espress_stream_hash *this, b_t *digest)
{
	int ret = 0;
	struct msghdr msg[1] = {{0}};

	if (-1 == sendmsg(this->sock, msg, 0))
	{
		ret = 1;
		goto EXIT;
	}

	if (-1 == read(this->sock, digest, ESPRESS_STREAM_HASH_DIGEST_SIZE))
	{
		ret = 1;
		goto EXIT;
	}

EXIT:

	close(this->sock);
	close(this->op_sock);

	if (0 != ret) abort();
}

static void _update(struct espress_stream_hash *this, b_t const *message, u_t size)
{
	int ret = 0;
	struct msghdr msg[1] = {{0}};
	struct iovec iov[1];

	iov->iov_base = (void *)message;
	iov->iov_len = size;
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;

	if (-1 == sendmsg(this->sock, msg, MSG_MORE))
	{
		ret = 1;
		goto EXIT;
	}

EXIT:
	if (0 != ret) close(this->sock);
	if (0 != ret) close(this->op_sock);

	if (0 != ret) abort();
}
