#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "connect_command.h"
#include "../network/client/client.h"

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

