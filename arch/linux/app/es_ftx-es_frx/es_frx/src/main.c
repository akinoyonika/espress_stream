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
#include <sys/eventfd.h>

#include "espress_stream/api.h"
#include "cipher_api.h"
#include "cert.h"

static unsigned char const secret_key[] = SECRET_KEY;
static unsigned char const public_key[] = PUBLIC_KEY;
static char const *user_id_pass[] = USER_ID_PASS;

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
static int arg_is_overwrite;
static const char *arg_dir_name;

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
	arg_is_overwrite = 0;
	arg_dir_name = NULL;

	for (i = 1 ; i < argc ; i++) {
		if (argv[i][0] == '-') {
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
			case 'o':
				arg_is_overwrite = 1;
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
			if (arg_dir_name == NULL) {
				arg_dir_name = argv[i];
			} else {
				ret = 1;
			}
			break;
		}
		if (ret) {
			break;
		}
	}

	ret = ret || (arg_dir_name == NULL);

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
					"[-o (overwrite)] "
					"<dir name>\n"
			"%s "
				"-0 (raw) "
					"[-a <interface name>] "
					"[-p <ether type>] "
					"[-o (overwrite)] "
					"<dir name>\n",
			p, p);
		printf("%s (C) 2023 akinoyonika\n", p);
		printf("%s\n", espress_stream_copyright());
	}

	return ret;
}

struct file_rx_attr {
	int is_attached;
	pthread_t thread;
	volatile int is_running;
	void *sid;
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

static void *file_rx(void *param)
{
	void *ret = param;
	int i;
	unsigned u;
	unsigned v;
	size_t s;
	volatile int *is_running = &(((struct file_rx_attr *)param)->is_running);
	void **sid = &(((struct file_rx_attr *)param)->sid);
	unsigned char buf[BUFFER_SIZE];
	char file_name[BUFFER_SIZE];
	FILE *file;
	uint64_t file_size;

	for (u = 0 ; u < sizeof(buf) ; u++) {
		v = 1;
		i = espress_stream_recv(*sid, buf + u, &v, *is_running ? TIMEOUT : 0);
		if (i != 0) {
			break;
		}
		if (v == 0) {
			i = 1;
			break;
		}
		if (buf[u] == '\0') {
			break;
		}
	}
	if ((i != 0) || (u == sizeof(buf))) {
		ret = NULL;
		goto USER_ID_PASS_SCAN_FAILURE_EXIT;
	}
	for (i = 0 ; i < (sizeof(user_id_pass) / sizeof(user_id_pass[0])) ; i++) {
		if (strcmp((char *)buf, user_id_pass[i]) == 0) {
			break;
		}
	}
	if (i == (sizeof(user_id_pass) / sizeof(user_id_pass[0]))) {
		ret = NULL;
		goto USER_ID_PASS_SCAN_FAILURE_EXIT;
	}

	i = (arg_dir_name[strlen(arg_dir_name) - 1] == '/') ? 0 : 1;
	i = snprintf(file_name, sizeof(file_name), "%s%s", arg_dir_name, i ? "/" : "");
	if (sizeof(file_name) <= i) {
		ret = NULL;
		goto DIR_NAME_TOO_LONG_EXIT;
	}
	for (u = strlen(file_name) ; u < sizeof(file_name) ; u++) {
		v = 1;
		i = espress_stream_recv(*sid, file_name + u, &v, *is_running ? TIMEOUT : 0);
		if (i != 0) {
			break;
		}
		if (v == 0) {
			i = 1;
			break;
		}
		if (file_name[u] == '\0') {
			break;
		}
	}
	if ((i != 0) || (u == sizeof(file_name))) {
		ret = NULL;
		goto FILE_NAME_SCAN_FAILURE_EXIT;
	}

	if (!arg_is_overwrite) {
		file = fopen(file_name, "rb");
		if (file != NULL) {
			fclose(file);
			ret = NULL;
			goto FILE_OPEN_FAILURE_EXIT;
		}
	}

	file = fopen(file_name, "wb");
	if (file == NULL) {
		ret = NULL;
		goto FILE_OPEN_FAILURE_EXIT;
	}

	DEBUG_PRINT("download start: \"%s\"\n", file_name);

	while (*is_running) {
		u = sizeof(buf);
		i = espress_stream_recv(*sid, buf, &u, *is_running ? TIMEOUT : 0);
		if (i != 0) {
			break;
		}
		if (u == 0) {
			break;
		}
		s = fwrite(buf, 1, u, file);
		if (s < u) {
			i = 1;
			break;
		}
	}
	if (!*is_running || (i != 0)) {
		ret = NULL;
		DEBUG_PRINT("download break: \"%s\"\n", file_name);
		goto DATA_RECV_BREAK_EXIT;
	}

	DEBUG_PRINT("download finish: \"%s\"\n", file_name);

DATA_RECV_BREAK_EXIT:
	file_size = ftell(file);
	fclose(file);
	if (ret == NULL) {
		unlink(file_name);
	} else {
		*((uint64_t *)buf) = htobe64(file_size);
		for (u = 0 ; u < sizeof(uint64_t) ; u += v) {
			v = sizeof(uint64_t) - u;
			i = espress_stream_send(*sid, buf + u, &v, *is_running ? TIMEOUT : 0);
			if (i != 0) {
				break;
			}
		}
		if (i != 0) {
			ret = NULL;
			DEBUG_PRINT("send response failure: \"%s\"\n", file_name);
		} else {
			i = espress_stream_flush(*sid, *is_running ? TIMEOUT : 0);
			if (i != 0) {
				ret = NULL;
				DEBUG_PRINT("send response failure: \"%s\"\n", file_name);
			} else {
				DEBUG_PRINT("send response: \"%s\"\n", file_name);
			}
		}
	}
FILE_OPEN_FAILURE_EXIT:
FILE_NAME_SCAN_FAILURE_EXIT:
USER_ID_PASS_SCAN_FAILURE_EXIT:
DIR_NAME_TOO_LONG_EXIT:
	espress_stream_disconnect(*sid);
	espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);

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
	struct file_rx_attr **attr = NULL;
	struct espress_stream_session_ex_param ex_param[1] = {{
		send_connect_payload, NULL,
		recv_connect_payload, NULL,
		NULL, NULL,
		NULL, NULL
	}};

	for (*sid = NULL ; *is_running ; ) {
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
			p = realloc(attr, sizeof(struct file_rx_attr *) * num_thread);
			q = malloc(sizeof(struct file_rx_attr));
			if ((p == NULL) || (q == NULL)) {
				if (p != NULL) free(p);
				if (q != NULL) free(q);
				num_thread--;
			} else {
				attr = (struct file_rx_attr **)p;
				attr[t] = (struct file_rx_attr *)q;
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
					if (pthread_create(&(attr[t]->thread), NULL, file_rx, attr[t])) {
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
	printf("receiving file overwrite: %s\n", arg_is_overwrite ? "available" : "unavailable");
	printf("receiving file store dir: \"%s\"\n", arg_dir_name);

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

	sige = eventfd(0, 0);
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

	for (u = 0 ; u < NUM_STANDBY ; u++) {
		if (standby_attr[u].is_attached) {
			pthread_join(standby_attr[u].thread, NULL);
			standby_attr[u].is_attached = 0;
		}
	}

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
