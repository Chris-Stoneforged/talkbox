#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "message_command.h"

void handle_message_command(int argc, char **argv) {
	int a = 0, f;
	argc -= 2;	// 2 Positional arguments required below, reduce argc to match
	optind = 0;	// Set optind to 0 so it gets every arg after the last positional arg

	char *user = *argv++;
	if (user == NULL || *user == '-') {
		printf("Missing positional argument 'User'\n");
		return;
	}

	char *message = *argv++;
	if (message == NULL || *message == '-') {
		printf("Missing positional argument 'Message'\n");
		return;
	}


	while ((f = getopt(argc, argv, "a")) != -1) {
		switch (f)
		{
		case 'a':
			a = 1;
			break;
		case '?':
			if (isprint(optopt))
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

	message_user(user, message, a);
}

void message_user(char *user, char *message, int anonymous) {
	// TODO: Send message via TCP
	printf("Sent message '%s' to %s!%s", message, user, anonymous == 1 ? " (anonymously)\n" : "\n");
}
