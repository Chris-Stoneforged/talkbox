#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cmd_parser.h"
#include "command.h"

void parse_args(int argc, char **argv) {
	if (argc < 2) {
		printf("Expected command argument.\n");
		return;
	}

	struct Command *command = create_command("message", "Send a message to a contact");
	add_arg(command, "Name of the contact");
	add_flag(command, 'a', "Send message annonymously");
	add_flag(command, 'i', "Send instantly");

	char *getopt_str = get_getopt_string(command);
	printf("getopt string is: %s\n", getopt_str);

	// char *usr_cmd = argv[1];
	argv++;
	argc -= (command->num_args + 1);	

	for (int i = 0; i < command->num_args; i++) {
		argv++;
		printf("Positional Argument: %s\n", *argv);
	}

	int c;
	while((c = getopt(argc, argv, getopt_str)) != -1) {
		printf("optopt = %c\n", optopt);
	}
}	
