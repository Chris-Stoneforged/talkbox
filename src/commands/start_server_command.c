#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "start_server_command.h"
#include "../network/server/server.h"

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

