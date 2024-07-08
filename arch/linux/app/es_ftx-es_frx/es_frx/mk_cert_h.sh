#!/bin/bash

user_id_pass=""
no=1
for i in `seq 999`
do
	read -p "${no}) user id: " user_id
	if [ "${user_id}" == "" ]
	then
		break
	fi
	read -p "${no}) user password: " -s user_password
	echo
	if [ "${user_password}" == "" ]
	then
		break
	fi
	if [ "`echo ${user_id} | cut -d ':' -s -f 1-`" != "" ]
	then
		echo "invalid user id"
		continue
	fi
	if [ "`echo ${user_id} | cut -d ' ' -s -f 1-`" != "" ]
	then
		echo "invalid user id"
		continue
	fi
	if [ "`echo ${user_password} | cut -d ':' -s -f 1-`" != "" ]
	then
		echo "invalid user password"
		continue
	fi
	if [ "`echo ${user_password} | cut -d ' ' -s -f 1-`" != "" ]
	then
		echo "invalid user password"
		continue
	fi
	k=0
	for j in ${user_id_pass}
	do
		k=`expr \( ${k} + 1 \) % 2`
		if [ ${k} == 0 ]
		then
			continue
		fi
		if [ "${user_id}" == "`echo ${j} | cut -d ':' -f 1`" ]
		then
			break
		fi
	done
	if [ ${k} != 0 ]
	then
		echo "existing user id"
		continue
	fi
	user_id_pass="${user_id_pass} ${user_id} ${user_password}"
	no=`expr ${no} + 1`
done

cat << "EOT" > mk_cert_h.c
#include <stdio.h>
#include <string.h>
#include "cipher_api.h"

int main()
{
	int i;
	unsigned char secret_key[CIPHER_ED25519_SECRET_KEY_SIZE];
	unsigned char public_key[CIPHER_ED25519_PUBLIC_KEY_SIZE];
	char *user_id = (char *)public_key;
#	define USER_ID_SIZE (CIPHER_ED25519_PUBLIC_KEY_SIZE)
	char *user_password = (char *)secret_key;
#	define USER_PASSWORD_SIZE (CIPHER_ED25519_SECRET_KEY_SIZE)

	cipher_ed25519_get_key(secret_key, public_key);

	printf("#define SECRET_KEY {\\\n");
	for (i = 0 ; i < CIPHER_ED25519_SECRET_KEY_SIZE ; i++) {
		printf("%s0x%02x,%s", (0 == (i % 8)) ? "\t" : "", secret_key[i], (7 == (i % 8)) ? "\\\n" : " ");
	}
	printf("}\n");

	printf("#define PUBLIC_KEY {\\\n");
	for (i = 0 ; i < CIPHER_ED25519_PUBLIC_KEY_SIZE ; i++) {
		printf("%s0x%02x,%s", (0 == (i % 8)) ? "\t" : "", public_key[i], (7 == (i % 8)) ? "\\\n" : " ");
	}
	printf("}\n");

	printf("#define USER_ID_PASS {\\\n");
	for( ; ; ) {
		//printf("user id: ");
		if ((NULL == fgets(user_id, USER_ID_SIZE, stdin)) ||
			(user_id[0] == '\n') ||
			(user_id[0] == '\0')) {
			break;
		}
		if (user_id[strlen(user_id) - 1] == '\n') {
			user_id[strlen(user_id) - 1] = '\0';
		}
		//printf("user password: ");
		if ((NULL == fgets(user_password, USER_PASSWORD_SIZE, stdin)) ||
			(user_password[0] == '\n') ||
			(user_password[0] == '\0')) {
			break;
		}
		if (user_password[strlen(user_password) - 1] == '\n') {
			user_password[strlen(user_password) - 1] = '\0';
		}
		printf("\t\"%s:%s\",\\\n", user_id, user_password);
	}
	printf("}\n");

	return 0;
}
EOT
gcc -O2 -I../cipher/include -Wall mk_cert_h.c ../cipher/src/*.c
(for l in ${user_id_pass}; do echo ${l}; done) | ./a.out > src/cert.h
rm -f src/mk_cert_h.o ../cipher/src/*.o a.out mk_cert_h.c
