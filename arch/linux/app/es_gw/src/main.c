#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <endian.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <fcntl.h>
#include <errno.h>
#include <wait.h>

#include "espress_stream/api.h"
#include "cipher_api.h"
#include "cert.h"

static unsigned char const secret_key[] = SECRET_KEY;
static unsigned char const public_key[] = PUBLIC_KEY;

#define IS_DEBUGGING (0)
#define NUM_STANDBY (16)
#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (4 * 1024)
#define CONNECT_MESSAGE_SIZE (64)

#if IS_DEBUGGING
#	define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#	define DEBUG_PRINT(...) ((void)0)
#endif

#define set_eventfd(fd) do{uint64_t u = 1; while (write(fd, &u, sizeof(u)) != sizeof(u));}while (0)
#define wait_eventfd(fd) do{uint64_t u = 1; while (read(fd, &u, sizeof(u)) != sizeof(u));}while (0) 

static void *ifd;
static int arg_protocol;
static const char *arg_address;
static const char *arg_port;
static char const *arg_ifname;
static char const *arg_type;
static const char **arg_cat_command_param;
static unsigned arg_num_cat_command_param;
static int arg_is_close_half;

static int inet_naton(int protocol, char const *name, char const *port, struct sockaddr *address)
{
	int ret = 0;
	struct addrinfo hint[1] = {0};
	struct addrinfo *addrinfo = NULL;

	hint->ai_family = protocol;
	hint->ai_socktype = SOCK_DGRAM;
	if (name == NULL) {
		hint->ai_flags = AI_PASSIVE;
	}
	if (port == NULL) {
		port = "0";
	}
	ret = getaddrinfo(name, port, hint, &addrinfo);
	if (ret != 0) {
		addrinfo = NULL;
		ret = 1;
		goto EXIT;
	}
	memcpy(address, addrinfo->ai_addr, addrinfo->ai_addrlen);

EXIT:
	if (addrinfo != NULL) freeaddrinfo(addrinfo);

	return ret;
}

static int packet_naton(char const *mac_address, char const *type, char const *ifname, struct sockaddr *address)
{
	int ret = 0;
	int i;
	int j;
	char *p;
	unsigned short type_num;
	unsigned mac[6] = {0};
	unsigned if_mac[6] = {0};
	int sock = -1;
	struct ifconf ifc[1];
	char buf[1024];
	struct ifreq ifr[1];

	((struct sockaddr_ll *)address)->sll_family = AF_PACKET;
	if (type != NULL) {
		type_num = strtoul(type, &p, 0);
		if ((*p != '\0') || (type_num < 0x0600)) {
			ret = 1;
			goto EXIT;
		}
	} else {
		type_num = ETH_P_ALL;
	}
	((struct sockaddr_ll *)address)->sll_protocol = htons(type_num);
	if ((mac_address != NULL) && (sscanf(mac_address, "%x:%x:%x:%x:%x:%x", mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5) != 6)) {
		ret = 1;
		goto EXIT;
	}
	sock = socket(PF_PACKET, SOCK_DGRAM, 0);
	if (sock == -1) {
		ret = 1;
		goto EXIT;
	}
	ifc->ifc_len = sizeof(buf);
	ifc->ifc_buf = buf;
	if (ioctl(sock, SIOCGIFCONF, ifc) == -1) {
		ret = 1;
		goto EXIT;
	}
	if ((ifname != NULL) && (sscanf(ifname, "%x:%x:%x:%x:%x:%x", if_mac, if_mac + 1, if_mac + 2, if_mac + 3, if_mac + 4, if_mac + 5) != 6)) {
		for (i = 0 ; i < (sizeof(buf) / sizeof(*ifr)) ; i++) {
			if (strcmp(ifname, (ifc->ifc_req + i)->ifr_name) == 0) {
				strcpy(ifr->ifr_name, (ifc->ifc_req + i)->ifr_name);
				if (ioctl(sock, SIOCGIFHWADDR, ifr) == -1) {
					ret = 1;
					goto EXIT;
				}
				for (j = 0 ; j < 6 ; j++) {
					if_mac[j] = ((unsigned char *)(ifr->ifr_hwaddr.sa_data))[j];
				}
				break;
			}
		}
		if (i == (sizeof(buf) / sizeof(*ifr))) {
			ret = 1;
			goto EXIT;
		}
		if (ioctl(sock, SIOCGIFINDEX, ifr) == -1) {
			ret = 1;
			goto EXIT;
		}
		((struct sockaddr_ll *)address)->sll_ifindex = ifr->ifr_ifindex;
	} else if ((if_mac[0] == 0) && (if_mac[1] == 0) && (if_mac[2] == 0) && (if_mac[3] == 0) && (if_mac[4] == 0) && (if_mac[5] == 0)) {
		((struct sockaddr_ll *)address)->sll_ifindex = 0;
	} else {
		for (i = 0 ; i < (sizeof(buf) / sizeof(*ifr)) ; i++) {
			strcpy(ifr->ifr_name, (ifc->ifc_req + i)->ifr_name);
			if (ioctl(sock, SIOCGIFHWADDR, ifr) == -1) {
				ret = 1;
				goto EXIT;
			}
			for (j = 0 ; j < 6 ; j++) {
				if (((unsigned char *)(ifr->ifr_hwaddr.sa_data))[j] != if_mac[j]) {
					break;
				}
			}
			if (j == 6) {
				break;
			}
		}
		if (i == (sizeof(buf) / sizeof(*ifr))) {
			ret = 1;
			goto EXIT;
		}
		if (ioctl(sock, SIOCGIFINDEX, ifr) == -1) {
			ret = 1;
			goto EXIT;
		}
		((struct sockaddr_ll *)address)->sll_ifindex = ifr->ifr_ifindex;
	}
	((struct sockaddr_ll *)address)->sll_hatype = 0;
	((struct sockaddr_ll *)address)->sll_pkttype = 0;
	if ((mac[0] == 0) && (mac[1] == 0) && (mac[2] == 0) && (mac[3] == 0) && (mac[4] == 0) && (mac[5] == 0)) {
		((struct sockaddr_ll *)address)->sll_halen = 0;
		((struct sockaddr_ll *)address)->sll_addr[0] = 0;
		((struct sockaddr_ll *)address)->sll_addr[1] = 0;
		((struct sockaddr_ll *)address)->sll_addr[2] = 0;
		((struct sockaddr_ll *)address)->sll_addr[3] = 0;
		((struct sockaddr_ll *)address)->sll_addr[4] = 0;
		((struct sockaddr_ll *)address)->sll_addr[5] = 0;
	} else {
		((struct sockaddr_ll *)address)->sll_halen = 6;
		((struct sockaddr_ll *)address)->sll_addr[0] = mac[0];
		((struct sockaddr_ll *)address)->sll_addr[1] = mac[1];
		((struct sockaddr_ll *)address)->sll_addr[2] = mac[2];
		((struct sockaddr_ll *)address)->sll_addr[3] = mac[3];
		((struct sockaddr_ll *)address)->sll_addr[4] = mac[4];
		((struct sockaddr_ll *)address)->sll_addr[5] = mac[5];
	}

EXIT:
	if (sock != -1) close(sock);

	return ret;
}

static int scan_arg(int argc, char const **argv)
{
	int i;
	char const *p;
	int ret = 0;
	char c = '\0';

	arg_protocol = PF_INET;
	arg_address = NULL;
	arg_port = "12345";
	arg_ifname = NULL;
	arg_type = NULL;
	arg_cat_command_param = NULL;
	arg_num_cat_command_param = 0;
	arg_is_close_half = 0;

	for (i = 1 ; i < argc ; i++) {
		if ((arg_num_cat_command_param == 0) && (argv[i][0] == '-')) {
			c = argv[i][1];
			switch (c) {
			case '4':
				arg_protocol = PF_INET;
				c = '\0';
				break;
			case '6':
				arg_protocol = PF_INET6;
				c = '\0';
				break;
			case '0':
				arg_protocol = PF_PACKET;
				c = '\0';
				break;
			case 'a':
			case 'p':
				break;
			case 'h':
				arg_is_close_half = 1;
				c = '\0';
				break;
			default:
				ret = 1;
				break;
			}
			if (ret) {
				break;
			}
			continue;
		}
		switch (c) {
		case 'a':
			arg_address = argv[i];
			arg_ifname = argv[i];
			c = '\0';
			break;
		case 'p':
			arg_port = argv[i];
			arg_type = argv[i];
			c = '\0';
			break;
		default:
			if (arg_num_cat_command_param == 0) {
				arg_cat_command_param = argv + i;
				arg_num_cat_command_param = 1;
			} else if ((arg_cat_command_param + arg_num_cat_command_param) == (argv + i)) {
				arg_num_cat_command_param++;
			} else {
				ret = 1;
			}
			break;
		}
		if (ret) {
			break;
		}
	}

	ret = ret || (arg_num_cat_command_param == 0);

	if (ret) {
		p = strrchr(argv[0], '/');
		if (p == NULL) {
			p = argv[0];
		} else {
			p++;
		}
		printf("%s "
				"[-4 (ipv4) | -6 (ipv6)] "
					"[-a <bind address>] "
					"[-p <bind port>] "
					"[-h (half close)] "
					"<cat command> [<cat command param> [...]]\n"
			"%s "
				"-0 (raw) "
					"[-a <interface name>] "
					"[-p <ether type>] "
					"[-h (half close)] "
					"<cat command> [<cat command param> [...]]\n",
			p, p);
		printf("%s (C) 2025 akinoyonika\n", p);
		printf("%s\n", espress_stream_copyright());
	}

	return ret;
}

static int popen_rw(char * const *command, int *rfd, int *wfd, int flags)
{
	int ret;
	int fd[2][2] = {{-1, -1}, {-1, -1}};
#	define PARENT_RFD fd[1][0]
#	define PARENT_WFD fd[0][1]
#	define CHILD_RFD fd[0][0]
#	define CHILD_WFD fd[1][1]

	ret = pipe2(fd[0], O_CLOEXEC);
	if (ret == -1) {
		fd[0][0] = -1;
		fd[0][1] = -1;
		goto EXIT;
	}

	ret = pipe2(fd[1], O_CLOEXEC);
	if (ret == -1) {
		fd[1][0] = -1;
		fd[1][1] = -1;
		goto EXIT;
	}

	ret = fork();
	if (ret == -1) {
		goto EXIT;
	}
	if (ret == 0) {
		dup2(CHILD_RFD, STDIN_FILENO);
		dup2(CHILD_WFD, STDOUT_FILENO);
		fcntl(STDIN_FILENO, F_SETFD, 0);
		fcntl(STDOUT_FILENO, F_SETFD, 0);
		close(PARENT_RFD);
		close(PARENT_WFD);
		close(CHILD_RFD);
		close(CHILD_WFD);
		execvp(command[0], command);
		exit(1);
	}

	fcntl(PARENT_RFD, F_SETFL, flags);
	*rfd = PARENT_RFD;
	PARENT_RFD = -1;
	fcntl(PARENT_WFD, F_SETFL, flags);
	*wfd = PARENT_WFD;
	PARENT_WFD = -1;

EXIT:
	if (fd[1][1] != -1) close(fd[1][1]);
	if (fd[1][0] != -1) close(fd[1][0]);
	if (fd[0][1] != -1) close(fd[0][1]);
	if (fd[0][0] != -1) close(fd[0][0]);
	
	return ret;
}

struct session_attr {
	int is_attached;
	pthread_t thread;
	int pstdin;
	int pstdout;
	void *sid;
	volatile int is_running;
	unsigned char message[CONNECT_MESSAGE_SIZE];
	unsigned message_size;
	int event;
};

struct standby_attr {
	int is_attached;
	pthread_t thread;
	volatile int is_running;
	volatile int is_blocking;
	void *sid;
	unsigned char message[CONNECT_MESSAGE_SIZE];
	unsigned message_size;
};
static struct standby_attr standby_attr[NUM_STANDBY];

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
	espress_stream_wait_any(*sid, events, 0xffffffff);
	set_eventfd(*event);
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

static void _wait(struct session_attr *attr,
	int is_stdin, int is_stdout, int is_sidin, int is_sidout,
	unsigned timeout)
{
	int i = -1;
	int *pstdin = &(attr->pstdin);
	int *pstdout = &(attr->pstdout);
	void **sid = &(attr->sid);
	int *event = &(attr->event);
	pthread_t sub_wait_thread;
	struct pollfd fds[3];
	nfds_t nfds = 0;

	if (is_stdin) {
		fds[nfds].fd = *pstdin;
		fds[nfds].events = POLLIN;
		fds[nfds].revents = 0;
		nfds++;
	}

	if (is_stdout) {
		fds[nfds].fd = *pstdout;
		fds[nfds].events = POLLOUT;
		fds[nfds].revents = 0;
		nfds++;
	}

	if (is_sidin || is_sidout) {
		*event = eventfd(0, EFD_CLOEXEC);
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
				wait_eventfd(*event);
				pthread_join(sub_wait_thread, NULL);
				espress_stream_wait_cancel_break(*sid);
			}
			close(*event);
		}
	}
}

static void *cat(void *param)
{
	void *ret = param;
	struct session_attr *attr = (struct session_attr *)param;
	int i;
	ssize_t st;
	unsigned u;
	unsigned v;
	volatile int *is_running = &(attr->is_running);
	int *pstdin = &(attr->pstdin);
	int *pstdout = &(attr->pstdout);
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
	unsigned stdout_tm;
	unsigned sidout_tm;
	char **command = (char **)calloc(arg_num_cat_command_param + 1, sizeof(char *));
	int pid;

	if (command == NULL) {
		pid = -1;
	} else {
		memcpy(command, arg_cat_command_param, sizeof(char *) * arg_num_cat_command_param);
		pid = popen_rw(command, pstdin, pstdout, O_NONBLOCK);
	}

	if (pid == -1) {
		ret = NULL;
		goto EXIT;
	}

	for (is_stdin_open = 1, is_stdout_open = 1, is_sidin_open = 1, is_sidout_open = 1, stdout_tm = sidout_tm = espress_stream_time() ;
		*is_running && (is_stdin_open || is_stdout_open || is_sidin_open || is_sidout_open) ;
		) {
		v = espress_stream_time();
		if (is_stdout_open && (0 < write_buf_size)) {
			u = (v - stdout_tm) & 0xffffffff;
			u = (u < TIMEOUT) ? (TIMEOUT - u) : TIMEOUT;
		} else {
			u = 0xffffffff;
		}
		if (is_sidout_open && (0 < send_buf_size)) {
			v = (v - sidout_tm) & 0xffffffff;
			v = (v < TIMEOUT) ? (TIMEOUT - v) : TIMEOUT;
		} else {
			v = 0xffffffff;
		}
		_wait(attr,
			is_stdin_open && (read_buf_size == 0),
			is_stdout_open && (0 < write_buf_size),
			is_sidin_open && (recv_buf_size == 0),
			is_sidout_open && (0 < send_buf_size),
			*is_running ? ((u < v) ? u : v) : 0);

		if (is_stdin_open) {
			if (read_buf_size == 0) {
				st = read(*pstdin, read_buf, BUFFER_SIZE);
				if (0 < st) {
					read_buf_size = st;
				} else if (st == 0) {
					close(*pstdin);
					is_stdin_open = 0;
					if (send_buf_size == 0) {
						i = flush(attr, TIMEOUT);
						espress_stream_disconnect(*sid);
						espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
						is_sidout_open = 0;
						if (0 < i) {
							break;
						}
					}
				} else if (errno != EAGAIN) {
					break;
				}
			} else {
				st = read(*pstdin, NULL, 0);
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
							close(*pstdout);
							is_stdout_open = 0;
						}
						if (!arg_is_close_half && is_stdin_open)
						{
							close(*pstdin);
							is_stdin_open = 0;
							if (send_buf_size == 0) {
								i = flush(attr, TIMEOUT);
								espress_stream_disconnect(*sid);
								espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
								is_sidout_open = 0;
								if (0 < i) {
									break;
								}
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
				st = write(*pstdout, write_buf, write_buf_size);
				if (0 < st) {
					write_buf += st;
					write_buf_size -= st;
					if (write_buf_size == 0) {
						write_buf = recv_buf;
						if (is_sidin_open) {
							//fsync(*pstdout);
						} else {
							close(*pstdout);
							is_stdout_open = 0;
						}
					}
					stdout_tm = espress_stream_time();
				} else if (errno != EAGAIN) {
					break;
				} else {
					if (TIMEOUT <= ((espress_stream_time() - stdout_tm) & 0xffffffff)) {
						break;
					}
				}
			} else {
				st = write(*pstdout, NULL, 0);
				if ((st == -1) && (errno != EAGAIN)) {
					break;
				}
				stdout_tm = espress_stream_time();
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
							i = flush(attr, TIMEOUT);
							espress_stream_disconnect(*sid);
							espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
							is_sidout_open = 0;
							if (0 < i) {
								break;
							}
						}
					}
					sidout_tm = espress_stream_time();
				} else if (0 < i) {
					break;
				} else {
					if (TIMEOUT <= ((espress_stream_time() - sidout_tm) & 0xffffffff)) {
						break;
					}
				}
			} else {
				u = 1;
				i = espress_stream_send(*sid, NULL, &u, 0);
				if (0 < i) {
					break;
				}
				sidout_tm = espress_stream_time();
			}
		}
	}

	if (is_stdin_open) {
		close(*pstdin);
	}
	if (is_stdout_open) {
		close(*pstdout);
	}
	if (is_sidin_open) {
		;
	}
	if (is_sidout_open) {
		espress_stream_disconnect(*sid);
		espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
	}

	if (waitpid(pid, NULL, WNOHANG) == 0) {
		kill(pid, SIGTERM);
		waitpid(pid, NULL, 0);
	}

EXIT:

	if (command != NULL) {
		free(command);
	}

	*is_running = 0;

	for (u = 0 ; u < NUM_STANDBY ; u++) {
		if (standby_attr[u].is_attached) {
			standby_attr[u].is_blocking = 0;
			if (standby_attr[u].sid != NULL) {
				espress_stream_wait_break(standby_attr[u].sid);
			}
		}
	}

	return ret;
}

static int send_connect_payload(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param)
{
	int ret;
	unsigned char *message = ((struct standby_attr *)param)->message;
	unsigned message_size = ((struct standby_attr *)param)->message_size;
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
	unsigned char *message = ((struct standby_attr *)param)->message;
	unsigned *message_size = &(((struct standby_attr *)param)->message_size);
	*message_size = CONNECT_MESSAGE_SIZE;
	return callback(sid, message, message_size);
}

static void *standby(void *param)
{
	void *ret = param;
	int i;
	void *p;
	void *q;
	unsigned t;
	unsigned u;
	volatile int *is_running = &(((struct standby_attr *)param)->is_running);
	volatile int *is_blocking = &(((struct standby_attr *)param)->is_blocking);
	void **sid = &(((struct standby_attr *)param)->sid);
	unsigned num_thread = 0;
	struct session_attr **attr = NULL;
	struct espress_stream_session_ex_param ex_param[1] = {{
		send_connect_payload, NULL,
		recv_connect_payload, NULL,
		NULL, NULL,
		NULL, NULL
	}};

	while (*is_running) {
		for (t = 0, u = num_thread ; t < num_thread ; t++) {
			if (!attr[t]->is_attached) {
				u = t;
				if (attr[t]->sid != NULL) {
					break;
				}
			}
		}
		if (t == num_thread) {
			t = u;
		}
		if (t == num_thread) {
			num_thread++;
			p = realloc(attr, sizeof(struct session_attr *) * num_thread);
			q = malloc(sizeof(struct session_attr));
			if ((p == NULL) || (q == NULL)) {
				if (p != NULL) free(p);
				if (q != NULL) free(q);
				num_thread--;
			} else {
				attr = (struct session_attr **)p;
				attr[t] = (struct session_attr *)q;
				attr[t]->is_attached = 0;
				attr[t]->is_running = 0;
				attr[t]->sid = NULL;
			}
		}

		if (t < num_thread) {
			if (attr[t]->sid == NULL) {
				ex_param->send_connect_payload_param = param;
				ex_param->recv_connect_payload_param = param;
				if (espress_stream_begin_session(&(attr[t]->sid), ifd, 1, ex_param) != 0) {
					attr[t]->sid = NULL;
				}
			}
			if (attr[t]->sid != NULL) {
				*sid = attr[t]->sid;
				i = espress_stream_connect_detect(attr[t]->sid, (*is_running && *is_blocking) ? 0xffffffff : 0);
				*sid = NULL;
				espress_stream_wait_cancel_break(attr[t]->sid);
				*is_blocking = 1;
				if (i == 0) {
					attr[t]->is_attached = 1;
					attr[t]->is_running = 1;
					if (pthread_create(&(attr[t]->thread), NULL, cat, attr[t])) {
						espress_stream_disconnect(attr[t]->sid);
						espress_stream_disconnect_result(attr[t]->sid, *is_running ? TIMEOUT : 0);
						espress_stream_end_session(attr[t]->sid);
						attr[t]->sid = NULL;
						attr[t]->is_attached = 0;
						attr[t]->is_running = 0;
					}
				} else if (0 < i) {
					espress_stream_end_session(attr[t]->sid);
					attr[t]->sid = NULL;
				}
			}
		}

		for (t = 0 ; t < num_thread ; t++) {
			if (attr[t]->is_attached && !attr[t]->is_running) {
				pthread_join(attr[t]->thread, NULL);
				attr[t]->is_attached = 0;
				espress_stream_end_session(attr[t]->sid);
				attr[t]->sid = NULL;
			}
		}
	}

	for (t = 0 ; t < num_thread ; t++) {
		if (attr[t]->is_attached) {
			attr[t]->is_running = 0;
		}
	}
	for (t = 0 ; t < num_thread ; t++) {
		if (attr[t]->is_attached) {
			espress_stream_wait_break(attr[t]->sid);
			pthread_join(attr[t]->thread, NULL);
			espress_stream_wait_cancel_break(attr[t]->sid);
			attr[t]->is_attached = 0;
			espress_stream_end_session(attr[t]->sid);
			attr[t]->sid = NULL;
		}
	}

	for (t = 0 ; t < num_thread ; t++) {
		free(attr[t]);
	}
	free(attr);

	*is_running = 0;
	return ret;
}

static int sige = -1;

static void sigh(int signum)
{
	set_eventfd(sige);
}

static void *sigt(void *param)
{
	unsigned u;
	wait_eventfd(sige);
	for (u = 0 ; u < NUM_STANDBY ; u++) {
		if (standby_attr[u].is_attached) {
			standby_attr[u].is_running = 0;
			if (standby_attr[u].sid != NULL) {
				espress_stream_wait_break(standby_attr[u].sid);
			}
		}
	}
	return param;
}

int main(int argc, char const **argv)
{
	int ret = 0;
	int i;
	unsigned u;
	int is_stream_ng = 1;
	struct sockaddr_storage address[1];
	pthread_t signal_thread;

	i = scan_arg(argc, argv);
	if (i) {
		ret = 1;
		//fprintf(stderr, "error: scan argument failure.\n");
		goto EXIT;
	}

	printf("public key: ");
	for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
		printf("%02x", public_key[i]);
	}
	printf("\n");
	switch (arg_protocol) {
	case PF_PACKET:
		printf("protocol: raw\n");
		printf("interface name: %s\n", (arg_ifname == NULL) ? "(any)" : arg_ifname);
		printf("ether type: %s\n", (arg_type == NULL) ? "(any)" : arg_type);
		break;
	case PF_INET6:
	case PF_INET:
	default:
		printf("protocol: %s\n", (arg_protocol == PF_INET6) ? "ipv6" : "ipv4");
		printf("bind address: %s\n", (arg_address == NULL) ? "(any)" : arg_address);
		printf("bind port: %s\n", arg_port);
		break;
	}
	printf("close: %s\n", arg_is_close_half ? "half" : "full");
	printf("cat command:");
	for (u = 0 ; u < arg_num_cat_command_param ; u++) {
		printf(" %s", arg_cat_command_param[u]);
	}
	printf("\n");

	if (arg_protocol == PF_PACKET) {
		i = packet_naton(NULL, arg_type, arg_ifname, (struct sockaddr *)address);
		if (i) {
			ret = 1;
			fprintf(stderr, "error: invalid bind_ether_type/bind_interface_name\n");
			goto EXIT;
		}
	} else {
		i = inet_naton(arg_protocol, arg_address, arg_port, (struct sockaddr *)address);
		if (i) {
			ret = 1;
			fprintf(stderr, "error: invalid bind_address/bind_port\n");
			goto EXIT;
		}
	}

	is_stream_ng = (espress_stream_begin() != 0);
	if (is_stream_ng) {
		ret = 1;
		fprintf(stderr, "error: can't begin stream.\n");
		goto EXIT;
	}

	i = espress_stream_begin_interface(&ifd, address);
	if (i != 0) {
		ifd = NULL;
		ret = 1;
		fprintf(stderr, "error: can't begin interface.\n");
		goto EXIT;
	}

	sige = eventfd(0, EFD_CLOEXEC);
	if (sige == -1) {
		ret = 1;
		fprintf(stderr, "error: can't create eventfd.\n");
		goto EXIT;
	}

	i = pthread_create(&signal_thread, NULL, sigt, NULL);
	if (i != 0) {
		ret = 1;
		fprintf(stderr, "error: can't create thread.\n");
		goto EXIT;
	}

	for (u = 0 ; u < NUM_STANDBY ; u++) {
		standby_attr[u].is_attached = 1;
		standby_attr[u].is_running = 1;
		standby_attr[u].is_blocking = 1;
		if (pthread_create(&(standby_attr[u].thread), NULL, standby, standby_attr + u)) {
			standby_attr[u].is_attached = 0;
			standby_attr[u].is_running = 0;
		}
	}

	signal(SIGINT, sigh);
	signal(SIGTERM, sigh);
	signal(SIGPIPE, SIG_IGN);

	for (u = 0 ; u < NUM_STANDBY ; u++) {
		if (standby_attr[u].is_attached) {
			pthread_join(standby_attr[u].thread, NULL);
			standby_attr[u].is_attached = 0;
		}
	}

	signal(SIGPIPE, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, SIG_DFL);

	set_eventfd(sige);
	pthread_join(signal_thread, NULL);

EXIT:

	if (sige != -1) close(sige);
	if (ifd != NULL) espress_stream_end_interface(ifd);
	if (!is_stream_ng) espress_stream_end();

	return ret;
}
