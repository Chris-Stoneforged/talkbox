#include "cmd_parser/cmd_parser.h"
#include "network.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	Command *command = parse_args(argv, argc);
	if (command == NULL) {
		return 1;
	}

	if (command->cmd_name == START_SERVER) {
		StartServer *start_server = (StartServer *)command->cmd_data;
		printf("Starting server on port %s\n", start_server->port);
	}	

	return 0;
}
