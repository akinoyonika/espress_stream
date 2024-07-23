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
#include <sys/random.h>

#include "espress_stream/api.h"
#include "cipher_api.h"

#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (8 * 1024)
#define CONNECT_MESSAGE_SIZE (64)

static void *ifd;
static const char *arg_address;
static const char *arg_port;
static unsigned char *arg_public_key;
static unsigned char _arg_public_key[CIPHER_ED25519_PUBLIC_KEY_SIZE];
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
	int j;
	int k;
	char const *p;
	int ret = 0;
	char c = '\0';

	arg_address = "localhost";
	arg_port = "12345";
	arg_public_key = NULL;
	arg_is_close_half = 0;
	arg_is_silent_enable = 0;

	for (i = 1 ; i < argc ; i++) {
		if (argv[i][0] == '-') {
			c = argv[i][1];
			switch (c) {
			case 'a':
			case 'p':
			case 'k':
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
			case 'k':
				for (j = 0 ; (j < CIPHER_ED25519_PUBLIC_KEY_SIZE * 2) &&  (argv[i][j] != '\0') ; j++) {
					if (('0' <= argv[i][j]) && (argv[i][j] <= '9')) {
						k = argv[i][j] - '0';
					} else if (('a' <= argv[i][j]) && (argv[i][j] <= 'f')) {
						k = argv[i][j] - 'a' + 10;
					} else if (('A' <= argv[i][j]) && (argv[i][j] <= 'F')) {
						k = argv[i][j] - 'A' + 10;
					} else {
						break;
					}
					if ((j & 1) == 0) {
						_arg_public_key[j >> 1] = (k << 4) & 0xf0;
					} else {
						_arg_public_key[j >> 1] |= k & 0x0f;
					}
				}
				if (j == (CIPHER_ED25519_PUBLIC_KEY_SIZE * 2)) {
					arg_public_key = _arg_public_key;
				} else {
					arg_public_key = NULL;
				}
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
					"[-a <destination address>] "
					"[-p <destination port>] "
					"[-k <destination public key>] "
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
	unsigned *message_size = &(((struct session_attr *)param)->message_size);
#	define get_random(buf, buf_sz) do \
	{                                 \
		unsigned char *b = (buf);     \
		unsigned u = (buf_sz);        \
		ssize_t s;                    \
		while (0 < u) {               \
			s = getrandom(b, u, 0);   \
			if (0 < s) {              \
				b += s;               \
				u -= s;               \
			}                         \
		}                             \
	} while(0)
	if (arg_public_key == NULL) {
		*message_size = CONNECT_MESSAGE_SIZE; /* dummy size */
		ret = callback(sid, NULL, 0, 0);
	} else {
		*message_size = CONNECT_MESSAGE_SIZE;
		get_random(message, *message_size);
		ret = callback(sid, message, *message_size, 0);
	}
	return ret;
}

static int recv_connect_payload(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param)
{
	int ret;
	unsigned char *message = ((struct session_attr *)param)->message;
	unsigned *message_size = &(((struct session_attr *)param)->message_size);
	unsigned char sign_message[CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE];
	unsigned sign_message_size = CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE;
	ret = callback(sid, sign_message, &sign_message_size);
	if ((arg_public_key != NULL) &&
			((ret != 0) ||
				(sign_message_size != CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE) ||
				(memcmp(message, sign_message + CIPHER_ED25519_SIGNATURE_SIZE, CONNECT_MESSAGE_SIZE) != 0) ||
				(cipher_ed25519_auth(sign_message, arg_public_key, sign_message + CIPHER_ED25519_SIGNATURE_SIZE, sign_message_size - CIPHER_ED25519_SIGNATURE_SIZE) != 0))) {
		*message_size = 0;
	}
	return ret;
}

static int conn(struct session_attr *attr,
	struct sockaddr_storage const *address,
	unsigned timeout)
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
	unsigned *message_size = &(attr->message_size);

	i = espress_stream_connect(*sid, address);
	if (i == 0) {
		for (i = -1, t = 0 ; *is_running && (i == -1) ; ) {
			t = timeout - t;
			if (1000000 < t) {
				t = 1000000;
			}
			i = espress_stream_connect_result(*sid, t);
			if (0 < i) {
				ex_param->send_connect_payload_param = attr;
				ex_param->recv_connect_payload_param = attr;
				i = espress_stream_begin_session(&new_sid, ifd, 0, ex_param);
				if (i == 0) {
					old_sid = *sid;
					*sid = new_sid;
					espress_stream_end_session(old_sid);
					i = espress_stream_connect(*sid, address);
					if (i == 0) {
						i = -1;
					}
				}
			} else if ((i == 0) && (*message_size == 0)) {
				espress_stream_disconnect(*sid);
				espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
				i = 1;
			}

			if (timeout <= (t = ((espress_stream_time() - start_time) & 0xffffffff))) {
				break;
			}
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

static void sub_wait(void *param, unsigned events)
{
	void **sid = &(((struct session_attr *)param)->sid);
	int *event = &(((struct session_attr *)param)->event);
	uint64_t const u64 = 1;
	espress_stream_wait_any(*sid, events, 0xffffffff);
	while (sizeof(u64) != write(*event, &u64, sizeof(u64)));
}

static void *s_wait(void *param)
{
	sub_wait(param, ESPRESS_STREAM_EVENT_SEND);
	return NULL;
}

static void *r_wait(void *param)
{
	sub_wait(param, ESPRESS_STREAM_EVENT_RECV);
	return NULL;
}

static void *sr_wait(void *param)
{
	sub_wait(param, ESPRESS_STREAM_EVENT_SEND | ESPRESS_STREAM_EVENT_RECV);
	return NULL;
}

static void wait(struct session_attr *attr,
	int is_stdin, int is_stdout, int is_sidin, int is_sidout,
	unsigned timeout)
{
	int i = -1;
	uint64_t u64;
	void **sid = &(attr->sid);
	int *event = &(attr->event);
	pthread_t sub_wait_thread;
	struct pollfd fds[3];
	nfds_t nfds = 0;

	if (is_stdin) {
		fds[nfds].fd = STDIN_FILENO;
		fds[nfds].events = POLLIN;
		fds[nfds].revents = 0;
		nfds++;
	}

	if (is_stdout) {
		fds[nfds].fd = STDOUT_FILENO;
		fds[nfds].events = POLLOUT;
		fds[nfds].revents = 0;
		nfds++;
	}

	if (is_sidin || is_sidout) {
		*event = eventfd(0, EFD_NONBLOCK);
		if (0 <= *event) {
			if (is_sidin && is_sidout) {
				i = pthread_create(&sub_wait_thread, NULL, sr_wait, attr);
			} else if (is_sidin) {
				i = pthread_create(&sub_wait_thread, NULL, r_wait, attr);
			} else if (is_sidout) {
				i = pthread_create(&sub_wait_thread, NULL, s_wait, attr);
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

	if (is_sidin || is_sidout) {
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
	int is_stdout_open;
	int is_sidin_open;
	int is_sidout_open;

	for (is_stdin_open = 1, is_stdout_open = 1, is_sidin_open = 1, is_sidout_open = 1 ;
		*is_running && (is_stdin_open || is_stdout_open || is_sidin_open || is_sidout_open) ;
		) {
		wait(attr,
			is_stdin_open && (read_buf_size == 0),
			is_stdout_open && (0 < write_buf_size),
			is_sidin_open && (recv_buf_size == 0),
			is_sidout_open && (0 < send_buf_size),
			1000000);

		if (is_stdin_open) {
			if (read_buf_size == 0) {
				st = read(STDIN_FILENO, read_buf, BUFFER_SIZE);
				if (0 < st) {
					read_buf_size = st;
				} else if (st == 0) {
					is_stdin_open = 0;
					if (send_buf_size == 0) {
						flush(attr, TIMEOUT);
						espress_stream_disconnect(*sid);
						espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
						is_sidout_open = 0;
					}
				} else if (errno != EAGAIN) {
					break;
				}
			} else {
				st = read(STDIN_FILENO, NULL, 0);
				if ((st == -1) && (errno != EAGAIN)) {
					break;
				}
			}
		}

		if (is_sidin_open) {
			if (recv_buf_size == 0) {
				u = BUFFER_SIZE;
				i = espress_stream_recv(*sid, recv_buf, &u, 0);
				if (i == 0) {
					if (0 < u) {
						recv_buf_size = u;
					} else {
						is_sidin_open = 0;
						if (write_buf_size == 0) {
							close(STDOUT_FILENO);
							is_stdout_open = 0;
						}
						if (!arg_is_close_half && is_stdin_open)
						{
							is_stdin_open = 0;
							if (send_buf_size == 0) {
								flush(attr, TIMEOUT);
								espress_stream_disconnect(*sid);
								espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
								is_sidout_open = 0;
							}
						}
					}
				} else if (0 < i) {
					break;
				}
			} else {
				u = 1;
				i = espress_stream_recv(*sid, NULL, &u, 0);
				if (0 < i) {
					break;
				}
			}
		}

		if (is_stdout_open) {
			if (0 < write_buf_size) {
				st = write(STDOUT_FILENO, write_buf, write_buf_size);
				if (0 < st) {
					write_buf += st;
					write_buf_size -= st;
					if (write_buf_size == 0) {
						write_buf = recv_buf;
						if (is_sidin_open) {
							//fsync(STDOUT_FILENO);
						} else {
							close(STDOUT_FILENO);
							is_stdout_open = 0;
						}
					}
				} else if (errno != EAGAIN) {
					break;
				}
			} else {
				st = write(STDOUT_FILENO, NULL, 0);
				if ((st == -1) && (errno != EAGAIN)) {
					break;
				}
			}
		}

		if (is_sidout_open) {
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
							is_sidout_open = 0;
						}
					}
				} else if (0 < i) {
					break;
				}
			} else {
				u = 1;
				i = espress_stream_send(*sid, NULL, &u, 0);
				if (0 < i) {
					break;
				}
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
	struct sockaddr_storage src_address[1] = {0};
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
		printf("destination address: %s\n", arg_address);
		printf("destination port: %s\n", arg_port);
		printf("destination public key: ");
		if (arg_public_key == NULL) {
			printf("(none)");
		} else {
			for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
				printf("%02x", arg_public_key[i]);
			}
		}
		printf("\n");
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

	((struct sockaddr_in *)src_address)->sin_family = AF_INET;
	((struct sockaddr_in *)src_address)->sin_port = htons(0);
	((struct sockaddr_in *)src_address)->sin_addr.s_addr = htonl(INADDR_ANY);
	i = espress_stream_begin_interface(&ifd, src_address);
	if (i != 0) {
		ret = 1;
		fprintf(stderr, "error: can't begin interface.\n");
		goto BEGIN_INTERFACE_FAIL_EXIT;
	}

	attr->is_running = 1;
	ex_param->send_connect_payload_param = attr;
	ex_param->recv_connect_payload_param = attr;
	i = espress_stream_begin_session(&(attr->sid), ifd, 0, ex_param);
	if (i != 0) {
		ret = 1;
		fprintf(stderr, "error: can't begin session.\n");
		goto BEGIN_SESSION_FAIL_EXIT;
	}

	org_sigh = signal(SIGINT, sigh);

	i = conn(attr, address, attr->is_running ? TIMEOUT : 0);
	if (i != 0) {
		fprintf(stderr, "error: fail to connect.\n");
		ret = 1;
		goto CONN_FAIL_EXIT;
	}

	if (!arg_is_silent_enable) {
		printf("CONNECT\n");
		fflush(stdout);
	}

	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	fcntl(STDOUT_FILENO, F_SETFL, O_NONBLOCK);

	cat(attr);

CONN_FAIL_EXIT:
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
