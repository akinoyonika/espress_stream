#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <fcntl.h>
#include <errno.h>

#include "espress_stream/api.h"
#include "cipher_api.h"
#include "cert.h"

static unsigned char const secret_key[] = SECRET_KEY;
static unsigned char const public_key[] = PUBLIC_KEY;

#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (2 * 1024)
#define CONNECT_MESSAGE_SIZE (64)

static void *ifd;
static const char *arg_address;
static const char *arg_port;
static int arg_is_close_half;
static int arg_is_silent_enable;

static int inet_naton(char const *name, char const *port, struct sockaddr *address)
{
	int ret;
	struct addrinfo hint[1] = {0};
	struct addrinfo *addrinfo;
	hint->ai_family = AF_INET;
	hint->ai_socktype = SOCK_DGRAM;
	if (name == NULL) {
		hint->ai_flags = AI_PASSIVE;
	}
	ret = getaddrinfo(name, port, hint, &addrinfo);
	if (ret == 0) {
		memcpy(address, addrinfo->ai_addr, addrinfo->ai_addrlen);
		freeaddrinfo(addrinfo);
	}
	return ret;
}

static int scan_arg(int argc, char const **argv)
{
	int i;
	char const *p;
	int ret = 0;
	char c = '\0';

	arg_address = NULL;
	arg_port = "12345";
	arg_is_close_half = 0;
	arg_is_silent_enable = 0;

	for (i = 1 ; i < argc ; i++) {
		if (argv[i][0] == '-') {
			c = argv[i][1];
			switch (c) {
			case 'a':
			case 'p':
				break;
			case 'h':
				arg_is_close_half = 1;
				c = '\0';
				break;
			case 's':
				arg_is_silent_enable = 1;
				c = '\0';
				break;
			default:
				c = '\0';
				break;
			}
			continue;
		}
		switch (c) {
			case 'a':
				arg_address = argv[i];
				c = '\0';
				break;
			case 'p':
				arg_port = argv[i];
				c = '\0';
				break;
			default:
				ret = 1;
				break;
		}
		if (ret) {
			break;
		}
	}

	if (ret) {
		p = strrchr(argv[0], '/');
		if (p == NULL) {
			p = argv[0];
		} else {
			p++;
			printf("%s "
					"[-a <bind address>] "
					"[-p <bind port>] "
					"[-h (half close)] "
					"[-s (silent)]\n",
				p);
			printf("%s (C) 2024 akinoyonika\n", p);
			printf("%s\n", espress_stream_copyright());
		}
	}

	return ret;
}

struct session_attr {
	void *sid;
	volatile int is_running;
	unsigned char message[CONNECT_MESSAGE_SIZE];
	unsigned message_size;
	int event;
};
struct session_attr session_attr[1];

static int send_connect_payload(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param)
{
	int ret;
	unsigned char *message = ((struct session_attr *)param)->message;
	unsigned message_size = ((struct session_attr *)param)->message_size;
	unsigned char sign_message[CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE];
	if (message_size == CONNECT_MESSAGE_SIZE) {
		cipher_ed25519_sign(sign_message, secret_key, message, message_size);
		memcpy(sign_message + CIPHER_ED25519_SIGNATURE_SIZE, message, message_size);
		ret = callback(sid, sign_message, CIPHER_ED25519_SIGNATURE_SIZE + message_size, 0);
	} else {
		ret = callback(sid, NULL, 0, 0);
	}
	return ret;
}

static int recv_connect_payload(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param)
{
	unsigned char *message = ((struct session_attr *)param)->message;
	unsigned *message_size = &(((struct session_attr *)param)->message_size);
	*message_size = CONNECT_MESSAGE_SIZE;
	return callback(sid, message, message_size);
}

static int accpt(struct session_attr *attr, unsigned timeout)
{
	int i;
	unsigned t;
	volatile int *is_running = &(attr->is_running);
	void **sid = &(attr->sid);
	void *new_sid;
	void *old_sid;
	unsigned const start_time = espress_stream_time();
	struct espress_stream_session_ex_param ex_param[1] = {{
		send_connect_payload, NULL,
		recv_connect_payload, NULL,
		NULL, NULL,
		NULL, NULL
	}};

	for (i = -1, t = 0 ; *is_running && (i == -1) ;) {
		t = timeout - t;
		if (1000000 < t) {
			t = 1000000;
		}
		i = espress_stream_connect_detect(*sid, t);
		if (0 < i) {
			ex_param->send_connect_payload_param = attr;
			ex_param->recv_connect_payload_param = attr;
			i = espress_stream_begin_session(&new_sid, ifd, 1, ex_param);
			if (i == 0) {
				old_sid = *sid;
				*sid = new_sid;
				espress_stream_end_session(old_sid);
			}
		}

		if (timeout <= (t = ((espress_stream_time() - start_time) & 0xffffffff))) {
			break;
		}
	}

	return i;
}

static int flush(struct session_attr *attr, unsigned timeout)
{
	int i;
	unsigned u;
	unsigned v;
	unsigned t;
	volatile int *is_running = &(attr->is_running);
	void **sid = &(attr->sid);
	unsigned start_time;

	espress_stream_flush(*sid, 0);
	for (i = -1, u = espress_stream_buffered_send_size(*sid, NULL), start_time = espress_stream_time(), t = 0 ;
		*is_running & (i == -1) ;
		u = v) {
		t = timeout - t;
		if (1000000 < t) {
			t = 1000000;
		}
		i = espress_stream_flush(*sid, t);

		v = espress_stream_buffered_send_size(*sid, NULL);
		if (u != v) {
			start_time = espress_stream_time();
		}

		if (timeout <= (t = ((espress_stream_time() - start_time) & 0xffffffff))) {
			break;
		}
	}

	return i;
}

static void *r_wait(void *param)
{
	void **sid = &(((struct session_attr *)param)->sid);
	int *event = &(((struct session_attr *)param)->event);
	uint64_t const u64 = 1;
	espress_stream_wait_any(*sid, ESPRESS_STREAM_EVENT_RECV, 0xffffffff);
	while (sizeof(u64) != write(*event, &u64, sizeof(u64)));
	return NULL;
}

static void *sr_wait(void *param)
{
	void **sid = &(((struct session_attr *)param)->sid);
	int *event = &(((struct session_attr *)param)->event);
	uint64_t const u64 = 1;
	espress_stream_wait_any(*sid, ESPRESS_STREAM_EVENT_SEND | ESPRESS_STREAM_EVENT_RECV, 0xffffffff);
	while (sizeof(u64) != write(*event, &u64, sizeof(u64)));
	return NULL;
}

static void wait(struct session_attr *attr, int is_stdin_open, int is_sidin_open, int is_write, int is_send, unsigned timeout)
{
	int i = -1;
	uint64_t u64;
	void **sid = &(attr->sid);
	int *event = &(attr->event);
	pthread_t sub_wait_thread;
	struct pollfd fds[2];
	nfds_t nfds = 0;

	if (is_stdin_open) {
		fds[nfds].fd = STDIN_FILENO;
		fds[nfds].events = POLLIN;
		if (is_write) {
			fds[nfds].events |= POLLOUT;
		}
		fds[nfds].revents = 0;
		nfds++;
	}

	if (is_sidin_open) {
		*event = eventfd(0, EFD_NONBLOCK);
		if (0 <= *event) {
			if (is_send) {
				i = pthread_create(&sub_wait_thread, NULL, sr_wait, attr);
			} else {
				i = pthread_create(&sub_wait_thread, NULL, r_wait, attr);
			}
			if (i == 0) {
				fds[nfds].fd = *event;
				fds[nfds].events = POLLIN;
				fds[nfds].revents = 0;
				nfds++;
			}
		}
	}

	poll(fds, nfds, timeout / 1000);

	if (is_sidin_open) {
		if (0 <= *event) {
			if (i == 0) {
				espress_stream_wait_break(*sid);
				while (sizeof(u64) != read(*event, &u64, sizeof(u64)));
				pthread_join(sub_wait_thread, NULL);
				espress_stream_wait_cancel_break(*sid);
			}
			close(*event);
		}
	}
}

static void cat(struct session_attr *attr)
{
	int i;
	ssize_t st;
	unsigned u;
	volatile int *is_running = &(attr->is_running);
	void **sid = &(attr->sid);
	unsigned char read_buf[BUFFER_SIZE];
	unsigned read_buf_size = 0;
	unsigned char recv_buf[BUFFER_SIZE];
	unsigned recv_buf_size = 0;
	unsigned char *write_buf = recv_buf;
#	define write_buf_size recv_buf_size
	unsigned char *send_buf = read_buf;
#	define send_buf_size read_buf_size
	int is_stdin_open;
	int is_sidin_open;

	for (is_stdin_open = 1, is_sidin_open = 1 ; *is_running && (is_stdin_open || is_sidin_open) ; ) {
		wait(attr, is_stdin_open, is_sidin_open, 0 < write_buf_size, 0 < send_buf_size, 1000000);

		if (is_stdin_open && (read_buf_size == 0)) {
			st = read(STDIN_FILENO, read_buf, BUFFER_SIZE);
			if (0 < st) {
				read_buf_size = st;
			} else if (st == 0) {
				is_stdin_open = 0;
				if (send_buf_size == 0) {
					flush(attr, TIMEOUT);
					espress_stream_disconnect(*sid);
					espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
				}
			} else if (errno != EAGAIN) {
				break;
			}
		}

		if (is_sidin_open && (recv_buf_size == 0)) {
			u = BUFFER_SIZE;
			i = espress_stream_recv(*sid, recv_buf, &u, 0);
			if (i == 0) {
				if (0 < u) {
					recv_buf_size = u;
				} else {
					is_sidin_open = 0;
					if (write_buf_size == 0) {
						close(STDOUT_FILENO);
					}
					if (!arg_is_close_half && is_stdin_open)
					{
						is_stdin_open = 0;
						if (send_buf_size == 0) {
							flush(attr, TIMEOUT);
							espress_stream_disconnect(*sid);
							espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
						}
					}
				}
			} else if (0 < i) {
				break;
			}
		}

		if (0 < write_buf_size) {
			st = write(STDOUT_FILENO, write_buf, write_buf_size);
			if (0 < st) {
				write_buf += st;
				write_buf_size -= st;
				if (write_buf_size == 0) {
					write_buf = recv_buf;
					if (is_sidin_open) {
						fsync(STDOUT_FILENO);
					} else {
						close(STDOUT_FILENO);
					}
				}
			} else if (errno != EAGAIN) {
				break;
			}
		}

		if (0 < send_buf_size) {
			u = send_buf_size;
			i = espress_stream_send(*sid, send_buf, &u, 0);
			if (i == 0) {
				send_buf += u;
				send_buf_size -= u;
				if (send_buf_size == 0) {
					send_buf = read_buf;
					if (is_stdin_open) {
						espress_stream_flush(*sid, 0);
					} else {
						flush(attr, TIMEOUT);
						espress_stream_disconnect(*sid);
						espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
					}
				}
			} else if (0 < i) {
				break;
			}
		}
	}
}

static void sigh(int signum)
{
	session_attr->is_running = 0;
}

int main(int argc, char const **argv)
{
	int ret = 0;
	int i;
	struct sockaddr_storage address[1] = {0};
	struct session_attr *attr = session_attr;
	struct espress_stream_session_ex_param ex_param[1] = {{
		send_connect_payload, NULL,
		recv_connect_payload, NULL,
		NULL, NULL,
		NULL, NULL
	}};
	sighandler_t org_sigh;

	i = scan_arg(argc, argv);
	if (i) {
		ret = 1;
		goto SCAN_ARG_FAIL_EXIT;
	}

	if (!arg_is_silent_enable) {
		printf("public key: ");
		for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
			printf("%02x", public_key[i]);
		}
		printf("\n");
		printf("bind address: %s%s%s\n",
			(arg_address == NULL) ? "(" : "",
			(arg_address == NULL) ? "any" : arg_address,
			(arg_address == NULL) ? ")" : "");
		printf("bind port: %s\n", arg_port);
		printf("close: %s\n", arg_is_close_half ? "half" : "full");
	}

	i = inet_naton(arg_address, arg_port, (struct sockaddr *)address);
	if (i) {
		ret = 1;
		fprintf(stderr, "error: invalid address/port\n");
		goto SCAN_ARG_FAIL_EXIT;
	}

	i = espress_stream_begin();
	if (i != 0) {
		ret = 1;
		fprintf(stderr, "error: can't begin stream.\n");
		goto BEGIN_STREAM_FAIL_EXIT;
	}

	i = espress_stream_begin_interface(&ifd, address);
	if (i != 0) {
		ret = 1;
		fprintf(stderr, "error: can't begin interface.\n");
		goto BEGIN_INTERFACE_FAIL_EXIT;
	}

	attr->is_running = 1;
	ex_param->send_connect_payload_param = attr;
	ex_param->recv_connect_payload_param = attr;
	i = espress_stream_begin_session(&(attr->sid), ifd, 1, ex_param);
	if (i != 0) {
		fprintf(stderr, "error: can't begin session.\n");
		ret = 1;
		goto BEGIN_SESSION_FAIL_EXIT;
	}

	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK);

	org_sigh = signal(SIGINT, sigh);

	i = accpt(attr, attr->is_running ? TIMEOUT : 0);
	if (i != 0) {
		fprintf(stderr, "error: no acceptable connection.\n");
		ret = 1;
		goto ACCEPT_FAIL_EXIT;
	}

	if (!arg_is_silent_enable) {
		printf("CONNECT\n");
		fflush(stdout);
	}

	cat(attr);

ACCEPT_FAIL_EXIT:
	signal(SIGINT, org_sigh);
	espress_stream_end_session(attr->sid);
BEGIN_SESSION_FAIL_EXIT:
	espress_stream_end_interface(ifd);
BEGIN_INTERFACE_FAIL_EXIT:
	espress_stream_end();
BEGIN_STREAM_FAIL_EXIT:
SCAN_ARG_FAIL_EXIT:

	return ret;
}
