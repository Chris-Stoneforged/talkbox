#include "server.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../network_utils.h"
#include "../../utils/daemonize.h"

#define MAX_CLIENTS 100

static struct sockaddr_storage their_addr;
static int sockfd;

void server_loop(void) {
	struct sigaction sa;
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		fprintf(stderr, "Sigaction failure\n");
		return;
	}

	while (1) {
		sin_size = sizeof their_addr;
		int newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
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

void start_server(struct ParseSubcommandResult *res) {
	puts("I'm here");
	char *port = get_opt_value(res, 'p');
	char *tmp = get_opt_value(res, 'm');

	int num_clients = atoi(tmp);
	if (num_clients <= 0 || num_clients > MAX_CLIENTS) {
		printf("-c requires a number between 1 and %d\n", MAX_CLIENTS);
		return;
	}

	int sockfd, rv, yes = 1;
	struct addrinfo hints, *servinfo, *p;

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

	if (listen(sockfd, num_clients) == -1) {
		fprintf(stderr, "Server failed to listen\n");
		return;
	}

	daemonize(server_loop, "/tmp/talkbox.pid", "/tmp/talkbox.log");
}


void kill_server(void) {
	pid_t pid = get_daemon_pid("/tmp/talkbox.pid");
	if (pid < 0) 
		return;
	
	if (kill(pid, SIGTERM) != 0) {
		perror("Failed to kill daemon");
	}

	puts("Successfully terminated the server");
}
