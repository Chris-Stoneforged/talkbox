#include <signal.h>
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
#include "start_server_command.h"

void handle_start_server_command(int argc, char **argv) {
	int f;
	char *port = "12345";
	int num_clients = 10;

	optind = 0;	// Set optind to 0 so it gets every arg after the subcommand

	while ((f = getopt(argc, argv, "p:c:")) != -1) {
		switch (f)
		{
		case 'p':
			port = optarg;
			break;
		case 'c':
			num_clients = atoi(optarg);
			if (num_clients <= 0 || num_clients > 100) {	// TODO: Magic number
				printf("-c requires a number between 1 and %d\n", 100);
				return;
			}
			break;
		case '?':
			if (optopt == 'p')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (optopt == 'c')
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

	start_server(port, num_clients);
}

void start_server(char *port, int max_clients) {
	int sockfd, newfd, rv, yes = 1;
	struct addrinfo hints, *servinfo, *p;
	struct sigaction sa;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "Could not get address info: %s\n", gai_strerror(rv));
		return;
	}	

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;
		
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			continue;

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL) {
		fprintf(stderr, "Server failed to bind to address\n");
		return;
	}

	if (listen(sockfd, max_clients) == -1) {
		fprintf(stderr, "Server failed to listen\n");
		return;
	}

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		fprintf(stderr, "Sigaction failure\n");
		return;
	}

	printf("Starting server on port %s. Up to %d clients can connect.\n", port, max_clients);

	while (1) {
		sin_size = sizeof their_addr;
		newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (newfd == -1)
			continue;

		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
		printf("Got connection from %s\n", s);

		if (!fork()) {
			close(sockfd);
			if (send(newfd, "I see you!", 10, 0) == -1) {
				fprintf(stderr, "Send failed\n");
			}
			close(newfd);
			exit(0);
		}
		close(newfd);
	}
}
