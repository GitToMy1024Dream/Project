.PHONY:all
all:udp_client udp_server

udp_client:udp_client.c
	gcc -o $@ $^
udp_server:udp_server.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f udp_client udp_server
