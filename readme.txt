To compile udp_server:
	udp_server.c is on eros.cs.txstate.edu
	open a window on eros.
	compile and run the server program first:
	to compile: gcc -O3 -g -std=c99 udp_server.c -o udp_server
	to run: ./udp_server

To compile udp_client:
	udp_client.c is on zeus.cs.txstate.edu
	open a window on zeus.
	to compile: gcc -o udp_client udp_client.c
	to run: ./udp_client eros.cs.txstate.edu