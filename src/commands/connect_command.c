#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include "network_utils.h"
#include "connect_command.h"

void handle_connect_command(int argc, char **argv) {
	int f;
	char *port = "12345";
	argc--;
	optind = 0;	// Set optind to 0 so it gets every arg after the subcommand

	char *address = *argv++;
	if (address == NULL || *address == '-') {
		printf("Missing positional argument 'Address'\n");
		return;
	}

	while ((f = getopt(argc, argv, "p:")) != -1) {
		switch (f)
		{
		case 'p':
			port = optarg;
			break;
		case '?':
			if (optopt == 'p')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else 
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return;
		}
	}

	if (optind < argc) {
		printf("Unexpected positional argument %s.\n", argv[optind]);
		return;
	}

	connect_client(address, port);
}

void connect_client(char *addr, char *port) {
	int sockfd, numbytes, rv;
	struct addrinfo hints, *servinfo, *p;
	char s[INET6_ADDRSTRLEN];
	char buf[100];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(addr, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "Incorrect address: %s\n", gai_strerror(rv));
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
			continue;

		inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));
		printf("Connecting to server at %s\n", s);

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "Connection failed\n");
		return;
	}

	printf("Connected!\n");
	freeaddrinfo(servinfo);

	if ((numbytes = recv(sockfd, buf, 99, 0)) == -1) {
		fprintf(stderr, "Failed to receive message\n");
		return;
	}

	buf[numbytes] = '\0';
	printf("Client got message: %s\n", buf);

	close(sockfd);
}
