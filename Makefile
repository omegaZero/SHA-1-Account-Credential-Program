all:
	gcc passwordSha1.c -lssl -lcrypto -o SHAauth
