all:
	gcc -Wall -c common.c
	gcc -Wall -c blog.c
	gcc -Wall client.c common.o blog.o -o client
	gcc -Wall server.c common.o blog.o -o server
clean:
	rm -f ./client ./server common.o blog.o