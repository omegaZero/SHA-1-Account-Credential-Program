all:
	gcc auth.c login.c -lssl -lcrypto -o login

clean:
	rm login *.a *.o
