#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>

#define LISTENER_QUEUE_LIMIT 10

const char *inet_ntopg(void *addr, char *buf, size_t size) {
	struct sockaddr_storage *sas = addr;
	struct sockaddr_in *sa4;
	struct sockaddr_in6 *sa6;
	void *src;

	switch (sas->ss_family) {
		case AF_INET:
			sa4 = addr;
			src = &(sa4->sin_addr);
			break;
		case AF_INET6:
			sa6 = addr;
			src = &(sa6->sin6_addr);
			break;
		default:
			return NULL;
	}
	
	return inet_ntop(sas->ss_family, src, buf, size);
}

int get_listener_socket(char *port) {
	int listener;
	int yes = 1;
	int rv;

	struct addrinfo hints, *ai, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, port, &hints, &ai)) != 0) {
		fprintf(stderr, "Could not start server: %s\n", gai_strerror(rv));
		exit(1);
	}

	for (p = ai; p != NULL; p = p->ai_next) {
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0) {
			continue;
		}

		// Free port if it's in use
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(listener);
			continue;
		}

		break;
	}

	if (p == NULL) {
		return -1;
	}

	freeaddrinfo(ai);

	if (listen(listener, LISTENER_QUEUE_LIMIT) < 0) {
		return -1;
	}

	return listener;
}

void handle_new_connection(int listener, int *fd_count, int *fd_size, struct pollfd **pfds) {
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;
	int newfd;
	char remoteIP[INET6_ADDRSTRLEN];
	
	addrlen = sizeof remoteaddr;
	newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

	if (newfd == -1) {
		perror("Could not accept connection");
	} else {
		(*pfds)[*fd_count].fd = newfd;
		(*pfds)[*fd_count].events = POLLIN;
		(*pfds)[*fd_count].revents = 0;

		(*fd_count)++;

		printf("New connection from %s on socket %d\n", inet_ntopg(&remoteaddr, remoteIP, sizeof remoteIP), newfd);
	}
}

void handle_client_data(int listener, int *fd_count, struct pollfd *pfds, int *pfd_i) {
	char buf[256];

	int nbytes = recv(pfds[*pfd_i].fd, buf, sizeof buf, 0);
	int sender_fd = pfds[*pfd_i].fd;

	if (nbytes <= 0) {
		if (nbytes == 0) {
			printf("Socket %d hung up\n", sender_fd);
		} else {
			perror("Could not process client data");
		}

		close(pfds[*pfd_i].fd);
		pfds[*pfd_i] = pfds[*fd_count - 1];
		(*fd_count)--;
		(*pfd_i)--;
	} else {
		printf("%d: %s", sender_fd, buf);

		for (int j = 0; j < *fd_count; j++) {
			int dest_fd = pfds[j].fd;
			
			if (dest_fd == listener || dest_fd == sender_fd) {
				continue;
			}

			if (send(dest_fd, buf, nbytes, 0) == -1) {
				perror("Count not send data");
			}
		}
	}
}

void process_connections(int listener, int *fd_count, int *fd_size, struct pollfd **pfds) {
	for (int i = 0; i < *fd_count; i++) {
		if (!((*pfds)[i].revents & (POLLIN | POLLHUP))) {
			continue;
		}
		
		if ((*pfds)[i].fd == listener) {
			handle_new_connection(listener, fd_count, fd_size, pfds);
		} else {
			handle_client_data(listener, fd_count, *pfds, &i);
		}
	}
}

int main(int argc, char *argv[]) {
	int listener;
	int fd_size = 5;
	int fd_count = 0;
	struct pollfd *pfds = malloc(sizeof *pfds * fd_size);

	if (argc < 2) {
		fprintf(stderr, "Not enough arguments\n");
		return 0;
	}

	listener = get_listener_socket(argv[1]);
	if (listener < 0) {
		fprintf(stderr, "Could not get listener socket\n");
		exit(1);
	}

	printf("Listening on port %s...\n", argv[1]);

	pfds[0].fd = listener;
	pfds[0].events = POLLIN;
	fd_count = 1;

	for (;;) {
		int poll_count = poll(pfds, fd_count, -1);

		if (poll_count == -1) {
			perror("Failed to poll for connections");
			exit(1);
		}

		process_connections(listener, &fd_count, &fd_size, &pfds);
	}

	free(pfds);
}
