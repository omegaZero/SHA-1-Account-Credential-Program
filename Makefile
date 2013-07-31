all:
	gcc auth.c login.c -lssl -lcrypto -o login

clean:
	rm -f login *.a *.o .userdata
