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
#include <sys/random.h>

#include "espress_stream/api.h"
#include "cipher_api.h"

#define IS_DEBUGGING (0)
#define TIMEOUT (30 * 1000 * 1000)
#define BUFFER_SIZE (4 *1024)
#define CONNECT_MESSAGE_SIZE (64)
#define USER_ID_SIZE (256)
#define USER_PASSWORD_SIZE (256)

static void *ifd;
static const char *arg_address;
static const char *arg_port;
static const char ** arg_file_name;
static unsigned arg_num_file;
static unsigned char arg_public_key[CIPHER_ED25519_PUBLIC_KEY_SIZE];
static char const *arg_user_id;
static char const *arg_user_password;

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
	int ret;
	char c = '\0';

	arg_address = "localhost";
	arg_port = "12345";
	arg_file_name = NULL;
	arg_num_file = 0;
	memset(arg_public_key, 0, CIPHER_ED25519_PUBLIC_KEY_SIZE);
	arg_user_id = NULL;
	arg_user_password = NULL;

	for (i = 1 ; i < argc ; i++) {
		if (argv[i][0] == '-') {
			c = argv[i][1];
			switch (c) {
			case 'a':
			case 'p':
			case 'k':
			case 'i':
			case 'w':
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
			for (j = 0 ; (j < (CIPHER_ED25519_PUBLIC_KEY_SIZE * 2)) && (argv[i][j] != '\0') ; j++) {
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
					arg_public_key[j >> 1] = (k << 4) & 0xf0;
				} else {
					arg_public_key[j >> 1] |= k & 0x0f;
				}
			}
			if (j < (CIPHER_ED25519_PUBLIC_KEY_SIZE * 2)) {
				memset(arg_public_key, 0, CIPHER_ED25519_PUBLIC_KEY_SIZE);
			}
			c = '\0';
			break;
		case 'i':
			arg_user_id = argv[i];
			c = '\0';
			break;
		case 'w':
			arg_user_password = argv[i];
			c = '\0';
			break;
		default:
			if (arg_num_file == 0) {
				arg_file_name = argv + i;
				arg_num_file = 1;
				ret = 0;
			} else if ((arg_file_name + arg_num_file) == (argv + i)) {
				arg_num_file++;
				ret = 0;
			} else {
				ret = 1;
			}
			break;
		}
		if (ret) {
			break;
		}
	}

	ret = ret || (arg_num_file == 0);

	if (ret) {
		p = strrchr(argv[0], '/');
		if (p == NULL) {
			p = argv[0];
		} else {
			p++;
		}
		printf("%s "
				"[-a <destination address>] "
				"[-p <destination port>] "
				"[-k <destination public key>] "
				"[-i <user id>] "
				"[-w <user password>] "
				"<file name> [<file name> [...]]\n",
			p);
		printf("%s (C) 2023 akinoyonika\n", p);
		printf("%s\n", espress_stream_copyright());
	}

	return ret;
}

struct file_tx_attr {
	unsigned index;
	int is_attached;
	pthread_t thread;
	void *sid;
	int is_running;
	struct sockaddr_storage const *address;
	char const *file_name;
	int ret;
	unsigned char message[CONNECT_MESSAGE_SIZE];
	unsigned message_size;
};

static struct file_tx_attr *file_tx_attr = NULL;

#if IS_DEBUGGING
	static void disp(unsigned index, char const *main_file_name, uint64_t file_size, uint64_t progress, int fin_state,
		unsigned bufdsz, unsigned rtt, unsigned rto, unsigned dac, unsigned cwnd, unsigned send_timeout_count, unsigned fast_retransmit_count)
	{
		unsigned u;
		static pthread_mutex_t disp_mutex[1] = {PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP};
#		define FILE_NAME_DISP_WIDTH (24)

		pthread_mutex_lock(disp_mutex);

		for (u = 0 ; u < index ; u++) {
			putchar('\n');
			putchar('\n');
		}

		if (main_file_name == NULL) {
			printf("------------------------\r");
		}
		else if (FILE_NAME_DISP_WIDTH < strlen(main_file_name)) {
			printf("%.*s...\r", FILE_NAME_DISP_WIDTH - 3, main_file_name);
		} else {
			printf("%-*s\r", FILE_NAME_DISP_WIDTH, main_file_name);
		}

		printf("\x1b[24C");
		if (fin_state == -1) {
			printf("[XXXXXXXXXXXXXXXX(XXXXXXXX)/%016llu]\n", file_size);
		} else if (fin_state == 1) {
			printf("[OOOOOOOOOOOOOOOO(OOOOOOOO)/%016llu]\n", file_size);
		} else {
			printf("[%016llu(%08u)/%016llu]\n", progress, bufdsz, file_size);
		}
		if (fin_state == 0) {
			printf("rtt=%08u, ", rtt);
			printf("rto=%08u, ", rto);
			printf("dac=%04u, ", dac);
			printf("cwnd=%04u, ", cwnd);
			printf("stoc=%08u, ", send_timeout_count);
			printf("frxc=%08u\r\x1b[A", fast_retransmit_count);
		} else {
			printf("\x1b[A");
		}

		if (0 < index) {
			printf("\x1b[%uA", index * 2);
		}

		fflush(stdout);

		pthread_mutex_unlock(disp_mutex);
	}

#	define disp_fin(count) do{unsigned u; unsigned v = (count); for (u = 0 ; u < v ; u++) {putchar('\n'); putchar('\n');}}while (0)

#	define disp(index, main_file_name, file_size, progress, fin_state, bufdsz, rtt, rto, dac, cwnd, send_timeout_count, fast_retransmit_count) \
		disp(index, main_file_name, file_size, progress, fin_state, bufdsz, rtt, rto, dac, cwnd, send_timeout_count, fast_retransmit_count)
#else
	static void disp(unsigned index, char const *main_file_name, uint64_t file_size, uint64_t progress, int fin_state)
	{
		unsigned u;
		unsigned ratio;
		static pthread_mutex_t disp_mutex[1] = {PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP};
#		define FILE_NAME_DISP_WIDTH (24)
#		define disp_num(n) do               \
		{                                   \
			unsigned num;                   \
			char *au;                       \
			if (10000000000000 < (n)) {     \
				num = (n) / 1000000000000;  \
				au = "T";                   \
			} else if (10000000000 < (n)) { \
				num = (n) / 1000000000;     \
				au = "G";                   \
			} else if (10000000 < (n)) {    \
				num = (n) / 1000000;        \
				au = "M";                   \
			} else if (10000 < (n)) {       \
				num = (n) / 1000;           \
				au = "K";                   \
			} else {                        \
				num = (n);                  \
				au = "";                    \
			}                               \
			printf("%u%sB", num, au);       \
		} while(0)

		if (file_size == 0) {
			ratio = 0;
		} else {
			ratio = progress * 32 / file_size;
		}

		pthread_mutex_lock(disp_mutex);

		for (u = 0 ; u < index ; u++) {
			putchar('\n');
		}

		if (main_file_name == NULL) {
			printf("------------------------\r");
		}
		else if (FILE_NAME_DISP_WIDTH < strlen(main_file_name)) {
			printf("%.*s...\r", FILE_NAME_DISP_WIDTH - 3, main_file_name);
		} else {
			printf("%-*s\r", FILE_NAME_DISP_WIDTH, main_file_name);
		}

		printf("\x1b[24C");
		if (file_size == 0) {
			if (fin_state == -1) {
				printf("[XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX] -/-");
			} else if (fin_state == 1) {
				printf("[OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO] -/-");
			} else {
				printf("[--------------------------------] -/-");
			}
		} else {
			if (fin_state == -1) {
				printf("[XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX] -/");
				disp_num(file_size);
			} else if (fin_state == 1) {
				printf("[OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO] ");
				disp_num(progress);
				putchar('/');
				disp_num(file_size);
			} else {
				printf("[%.*s%.*s] ",
					ratio, "================================",
					32 - ratio, "--------------------------------");
				disp_num(progress);
				putchar('/');
				disp_num(file_size);
			}
		}
		printf("\x1b[K\r");

		if (0 < index) {
			printf("\x1b[%uA", index);
		}

		fflush(stdout);

		pthread_mutex_unlock(disp_mutex);
	}

#	define disp_fin(count) do{unsigned u; unsigned v = (count); for (u = 0 ; u < v ; u++) {putchar('\n');}}while (0)

#	define disp(index, main_file_name, file_size, progress, fin_state, bufdsz, rtt, rto, dac, cwnd, send_timeout_count, fast_retransmit_count) \
		disp(index, main_file_name, file_size, progress, fin_state)
#endif

static int send_connect_payload(void *sid, int (*callback)(void *sid, void const *payload, unsigned payload_size, int is_rejected), void *param)
{
	unsigned char *message = ((struct file_tx_attr *)param)->message;
	unsigned *message_size = &(((struct file_tx_attr *)param)->message_size);
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
	*message_size = CONNECT_MESSAGE_SIZE;
	get_random(message, *message_size);
	return callback(sid, message, *message_size, 0);
}

static int recv_connect_payload(void *sid, int (*callback)(void *sid, void *payload, unsigned *payload_size), void *param)
{
	int ret;
	unsigned char *message = ((struct file_tx_attr *)param)->message;
	unsigned *message_size = &(((struct file_tx_attr *)param)->message_size);
	unsigned char sign_message[CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE];
	unsigned sign_message_size = CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE;
	ret = callback(sid, sign_message, &sign_message_size);
	if ((ret != 0) ||
		(sign_message_size != CIPHER_ED25519_SIGNATURE_SIZE + CONNECT_MESSAGE_SIZE) ||
		(memcmp(message, sign_message + CIPHER_ED25519_SIGNATURE_SIZE, CONNECT_MESSAGE_SIZE) != 0) ||
		(cipher_ed25519_auth(sign_message, arg_public_key, sign_message + CIPHER_ED25519_SIGNATURE_SIZE, sign_message_size - CIPHER_ED25519_SIGNATURE_SIZE) != 0)) {
		*message_size = 0;
	}
	return ret;
}

static int conn(struct file_tx_attr *attr,
	struct sockaddr_storage const *address,
	unsigned timeout, int *is_running)
{
	int i;
	unsigned t;
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
		for (i = -1 ;
			*is_running &&
				(i == -1) &&
				(t = (espress_stream_time() - start_time) & 0xffffffff) < timeout ;
			) {
			t = timeout - t;
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
		}
	}

	return i;
}

static int send_recv(struct file_tx_attr *attr,
	void const *send_data, unsigned *send_data_size,
	void *recv_data, unsigned *recv_data_size,
	unsigned timeout, int *is_running)
{
	int i;
	int j;
	unsigned t;
	void *sid = attr->sid;
	unsigned const start_time = espress_stream_time();

	for (i = -1, j = -1 ;
		*is_running &&
			(i == -1) && (j == -1) &&
			((t = (espress_stream_time() - start_time) & 0xffffffff) < timeout) ;
		) {
		t = timeout - t;
		espress_stream_wait_any(sid, t);
		i = espress_stream_send(sid, send_data, send_data_size, 0);
		j = espress_stream_recv(sid, recv_data, recv_data_size, 0);
	}

	i = (i < 0) ? -1 : ((0 < i) ? 1 : 0);
	j = (j < 0) ? -1 : ((0 < j) ? 1 : 0);
	return (i & 0x03) | ((j & 0x03) << 2);
}

static int flush_recv(struct file_tx_attr *attr,
	void *recv_data, unsigned *recv_data_size,
	unsigned timeout, int *is_running)
{
	int i;
	int j;
	unsigned t;
	void *sid = attr->sid;
	unsigned const start_time = espress_stream_time();

	for (i = -1, j = -1 ;
		*is_running &&
			(i == -1) && (j == -1) &&
			((t = (espress_stream_time() - start_time) & 0xffffffff) < timeout) ;
		) {
		t = timeout - t;
		espress_stream_wait_any(sid, t);
		i = espress_stream_flush(sid, 0);
		j = espress_stream_recv(sid, recv_data, recv_data_size, 0);
	}

	i = (i < 0) ? -1 : ((0 < i) ? 1 : 0);
	j = (j < 0) ? -1 : ((0 < j) ? 1 : 0);
	return (i & 0x03) | ((j & 0x03) << 2);
}

static void *file_tx(void *param)
{
	int i;
	int j;
	size_t s;
	unsigned u;
	unsigned v;
	unsigned w;
	unsigned char b;
	void *ret = param;
	struct sockaddr_storage const *address = ((struct file_tx_attr *)param)->address;
	unsigned index = ((struct file_tx_attr *)param)->index;
	int *is_running = &(((struct file_tx_attr *)param)->is_running);
	char const *file_name = ((struct file_tx_attr *)param)->file_name;
	char const *main_file_name;
	FILE *file;
	uint64_t file_size = 0;
	void **sid = &(((struct file_tx_attr *)param)->sid);
	unsigned char buf[BUFFER_SIZE];
	uint64_t progress;

	main_file_name = strrchr(file_name, '/');
	if (main_file_name == NULL) {
		main_file_name = file_name;
	} else {
		if (*(++main_file_name) == '\0') {
			main_file_name = NULL;
			ret = NULL;
			goto FILE_NAME_INVALID_EXIT;
		}
	}

	file = fopen(file_name, "rb");
	if (file == NULL) {
		ret = NULL;
		goto FILE_OPEN_FAILURE_EXIT;
	}

	disp(index, main_file_name, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	i = conn((struct file_tx_attr *)param, address, *is_running ? TIMEOUT : 0, is_running);
	if (i != 0) {
		ret = NULL;
		goto CONNECT_FAILURE_EXIT;
	}

	strcpy((char *)buf, arg_user_id);
	strcat((char *)buf, ":");
	strcat((char *)buf, arg_user_password);
	for (u = 0, s = strlen((char *)buf) + 1 ; u < s ; u += v) {
		v = s - u;
		w = 1;
		i = send_recv((struct file_tx_attr *)param, buf + u, &v, &b, &w, *is_running ? TIMEOUT : 0, is_running);
		j = (i >> 2) & 0x03; if (1 < j) j = -1;
		i = i & 0x03; if (1 < i) i = -1;
		if (i != 0) {
			i = 1;
			break;
		}
		if ((j == 0) && (w == 0)) {
			i = 1;
			break;
		}
		i = 0;
	}
	if (i != 0) {
		ret = NULL;
		goto USER_ID_PASS_SEND_FAILURE_EXIT;
	}
	i = espress_stream_flush(*sid, *is_running ? TIMEOUT : 0);
	if (i != 0) {
		ret = NULL;
		goto USER_ID_PASS_SEND_FAILURE_EXIT;
	}

	for (u = 0, s = strlen(main_file_name) + 1 ; u < s ; u += v) {
		v = s - u;
		w = 1;
		i = send_recv((struct file_tx_attr *)param, main_file_name + u, &v, &b, &w, *is_running ? TIMEOUT : 0, is_running);
		j = (i >> 2) & 0x03; if (1 < j) j = -1;
		i = i & 0x03; if (1 < i) i = -1;
		if (i != 0) {
			i = 1;
			break;
		}
		if ((j == 0) && (w == 0)) {
			i = 1;
			break;
		}
		i = 0;
	}
	if (i != 0) {
		ret = NULL;
		goto NAME_SEND_FAILURE_EXIT;
	}
	i = espress_stream_flush(*sid, *is_running ? TIMEOUT : 0);
	if (i != 0) {
		ret = NULL;
		goto NAME_SEND_FAILURE_EXIT;
	}

	for (progress = 0 ; *is_running ; ) {
		s = fread(buf, 1, sizeof(buf), file);
		for (u = 0 ; u < s ; u += v) {
			v = s - u;
			w = 1;
			i = send_recv((struct file_tx_attr *)param, buf + u, &v, &b, &w, *is_running ? TIMEOUT : 0, is_running);
			j = (i >> 2) & 0x03; if (1 < j) j = -1;
			i = i & 0x03; if (1 < i) i = -1;
			if (i != 0) {
				i = 1;
				break;
			}
			if ((j == 0) && (w == 0)) {
				i = 1;
				break;
			}
		}
		disp(index, main_file_name, file_size, (progress += u) - espress_stream_buffered_send_size(*sid, NULL), i ? -1 : 0,
			espress_stream_buffered_send_size(*sid, NULL),
			espress_stream_rtt(*sid),
			espress_stream_rto(*sid),
			espress_stream_dac(*sid),
			espress_stream_cwnd(*sid),
			espress_stream_send_timeout_count(*sid),
			espress_stream_fast_retransmit_count(*sid));
		if (i != 0) {
			break;
		}
		if (s < sizeof(buf)) {
			break;
		}
	}
	if (!*is_running || (i != 0)) {
		ret = NULL;
		goto DATA_SEND_BREAK_EXIT;
	}

	for (u = espress_stream_buffered_send_size(*sid, NULL) ; *is_running ; u = v) {
		disp(index, main_file_name, file_size, progress - espress_stream_buffered_send_size(*sid, NULL), 0,
			espress_stream_buffered_send_size(*sid, NULL),
			espress_stream_rtt(*sid),
			espress_stream_rto(*sid),
			espress_stream_dac(*sid),
			espress_stream_cwnd(*sid),
			espress_stream_send_timeout_count(*sid),
			espress_stream_fast_retransmit_count(*sid));
		w = 1;
		i = flush_recv((struct file_tx_attr *)param, &b, &w, *is_running ? TIMEOUT : 0, is_running);
		j = (i >> 2) & 0x03; if (1 < j) j = -1;
		i = i & 0x03; if (1 < i) i = -1;
		if ((0 <= i) || ((j == 0) && (w == 0))) {
			break;
		}
		v = espress_stream_buffered_send_size(*sid, NULL);
		if (u == v) {
			break;
		}
	}
	if (!*is_running || (i != 0) || ((j == 0) && (w == 0))) {
		ret = NULL;
		goto DATA_SEND_TERMINATE_FAILURE_EXIT;
	}

DATA_SEND_TERMINATE_FAILURE_EXIT:
DATA_SEND_BREAK_EXIT:
NAME_SEND_FAILURE_EXIT:
USER_ID_PASS_SEND_FAILURE_EXIT:
	espress_stream_disconnect(*sid);
	espress_stream_disconnect_result(*sid, *is_running ? TIMEOUT : 0);
	if (ret != NULL) {
		for (u = 0 ; u < sizeof(uint64_t) ; u += v) {
			v = sizeof(uint64_t) - u;
			i = espress_stream_recv(*sid, buf + u, &v, *is_running ? TIMEOUT : 0);
			if (i != 0) {
				break;
			}
			if (v == 0) {
				i = 1;
				break;
			}
		}
		if (!*is_running || (i != 0) || (file_size != be64toh(*((uint64_t *)buf)))) {
			ret = NULL;
		} else {
			w = 1;
			espress_stream_recv(*sid, &b, &w, *is_running ? TIMEOUT : 0);
		}
	}
CONNECT_FAILURE_EXIT:
	fclose(file);
FILE_OPEN_FAILURE_EXIT:
FILE_NAME_INVALID_EXIT:
	if (ret == NULL) {
		disp(index, main_file_name, file_size, 0, -1, 0, 0, 0, 0, 0, 0, 0);
	} else {
		disp(index, main_file_name, file_size, file_size, 1, 0, 0, 0, 0, 0, 0, 0);
	}

	*is_running = 0;
	return ret;
}

static void sigh(int signum)
{
	unsigned u;
	for (u = 0 ; u < arg_num_file ; u++) {
		if (file_tx_attr[u].is_attached) {
			file_tx_attr[u].is_running = 0;
		}
	}
}

int main(int argc, char const **argv)
{
	int ret = 0;
	int i;
	unsigned u;
	void *p;
	int is_stream_ng = 1;
	struct sockaddr_storage address[1] = {0};
	struct sockaddr_storage src_address[1] = {0};
	sighandler_t org_sigh;
	struct espress_stream_session_ex_param ex_param[1] = {{
		send_connect_payload, NULL,
		recv_connect_payload, NULL,
		NULL, NULL,
		NULL, NULL
	}};
	char user_id[USER_ID_SIZE];
	char user_password[USER_PASSWORD_SIZE];

	ifd = NULL;

	i = scan_arg(argc, argv);
	if (i) {
		ret = 1;
		//fprintf(stderr, "error: scan argument failure.\n");
		goto EXIT;
	}

	printf("destination address: \"%s\"\n", arg_address);
	printf("destination port: %s\n", arg_port);
	printf("destination public key: ");
	for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
		printf("%02x", arg_public_key[i]);
	}
	printf("\n");
	printf("number of sending files: %u\n", arg_num_file);
	printf("user id: ");
	if (NULL == arg_user_id) {
		if ((NULL == fgets(user_id, USER_ID_SIZE, stdin)) ||
			(user_id[0] == '\n') ||
			(user_id[0] == '\0')) {
			user_id[0] = '\0';
		} else if (user_id[strlen(user_id) - 1] == '\n') {
			user_id[strlen(user_id) - 1] = '\0';
		}
	} else {
		strncpy(user_id, arg_user_id, USER_ID_SIZE);
		user_id[USER_ID_SIZE - 1] = '\0';
		printf("%s\n", user_id);
	}
	arg_user_id = user_id;
	printf("user password: ");
	if (NULL == arg_user_password) {
#		pragma GCC diagnostic push
#		pragma GCC diagnostic ignored "-Wunused-result"
		system("stty -echo");
#		pragma GCC diagnostic pop
		if ((NULL == fgets(user_password, USER_PASSWORD_SIZE, stdin)) ||
			(user_password[0] == '\n') ||
			(user_password[0] == '\0')) {
			user_password[0] = '\0';
		} else if (user_password[strlen(user_password) - 1] == '\n') {
			user_password[strlen(user_password) - 1] = '\0';
		}
#		pragma GCC diagnostic push
#		pragma GCC diagnostic ignored "-Wunused-result"
		system("stty echo");
#		pragma GCC diagnostic pop
		printf("\n");
	} else {
		strncpy(user_password, arg_user_password, USER_PASSWORD_SIZE);
		user_password[USER_PASSWORD_SIZE - 1] = '\0';
		printf("\n");
	}
	arg_user_password = user_password;

	i = inet_naton(arg_address, arg_port, (struct sockaddr *)address);
	if (i) {
		ret = 1;
		fprintf(stderr, "error: invalid address/port\n");
		goto EXIT;
	}

	file_tx_attr = (struct file_tx_attr *)calloc(arg_num_file, sizeof(struct file_tx_attr));
	if (file_tx_attr == NULL) {
		ret = 1;
		fprintf(stderr, "error: heap allocate failure.\n");
		goto EXIT;
	}

	is_stream_ng = (espress_stream_begin() != 0);
	if (is_stream_ng) {
		ret = 1;
		fprintf(stderr, "error: can't begin stream.\n");
		goto EXIT;
	}

	((struct sockaddr_in *)src_address)->sin_family = AF_INET;
	((struct sockaddr_in *)src_address)->sin_port = htons(0);
	((struct sockaddr_in *)src_address)->sin_addr.s_addr = htonl(INADDR_ANY);
	i = espress_stream_begin_interface(&ifd, src_address);
	if (i != 0) {
		ifd = NULL;
		ret = 1;
		fprintf(stderr, "error: can't begin interface.\n");
		goto EXIT;
	}

	for (u = 0 ; u < arg_num_file ; u++) {
		file_tx_attr[u].index = u;
		file_tx_attr[u].is_attached = 1;
		file_tx_attr[u].is_running = 1;
		file_tx_attr[u].address = address;
		file_tx_attr[u].file_name = arg_file_name[u];
		file_tx_attr[u].ret = 0;
		ex_param->send_connect_payload_param = file_tx_attr + u;
		ex_param->recv_connect_payload_param = file_tx_attr + u;
		i = espress_stream_begin_session(&(file_tx_attr[u].sid), ifd, 0, ex_param);
		if ((i == 0) && pthread_create(&(file_tx_attr[u].thread), NULL, file_tx, file_tx_attr + u)) {
			file_tx_attr[u].is_attached = 0;
			file_tx_attr[u].is_running = 0;
		}
	}

	org_sigh = signal(SIGINT, sigh);

	for ( ; ; ) {
		sleep(1);
		for (u = 0 ; u < arg_num_file ; u++) {
			if (file_tx_attr[u].is_attached && file_tx_attr[u].is_running) {
				break;
			}
		}
		if (u == arg_num_file) {
			for (u = 0 ; u < arg_num_file ; u++) {
				if (file_tx_attr[u].is_attached) {
					espress_stream_wait_break(file_tx_attr[u].sid);
				}
			}
			break;
		}
	}

	signal(SIGINT, org_sigh);

	for (u = 0 ; u < arg_num_file ; u++) {
		if (file_tx_attr[u].is_attached) {
			i = pthread_join(file_tx_attr[u].thread, &p);
			if ((i != 0) || (p == NULL)) {
				file_tx_attr[u].ret = 1;
			}
			file_tx_attr[u].is_attached = 0;
			espress_stream_end_session(file_tx_attr[u].sid);
		} else {
			file_tx_attr[u].ret = 1;
		}
	}

	disp_fin(arg_num_file);

	for (u = 0 ; u < arg_num_file ; u++) {
		if (file_tx_attr[u].ret) {
			ret = 1;
			fprintf(stderr, "error: file \"%s\" transmit failure.\n", file_tx_attr[u].file_name);
		}
	}


EXIT:

	if (ifd != NULL) espress_stream_end_interface(ifd);
	if (!is_stream_ng) espress_stream_end();
	if (file_tx_attr != NULL) free(file_tx_attr);

	return ret;
}
