#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "start_server_command.h"

void handle_start_server_command(int argc, char **argv) {
	int f;
	char *port = "12345";
	optind = 0;	// Set optind to 0 so it gets every arg after the subcommand

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

	start_server(port);
}

void start_server(char *port) {
	// TODO:: start server!
	printf("Starting server on port %s\n", port);
}
