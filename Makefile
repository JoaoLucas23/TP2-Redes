all:
	gcc -Wall -c common.c
	gcc -Wall -c topico.c
	gcc -Wall -c blog.c
	gcc -Wall client.c common.o blog.o topico.o -o bin/client
	gcc -Wall server.c common.o blog.o topico.o -o bin/server
clean:
	rm -f ./client ./server common.o blog.o topico.o