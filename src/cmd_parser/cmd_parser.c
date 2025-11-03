#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmd_parser.h"

Command *parse_args(char *args[], int arg_c) {
	if (arg_c < 3) {
		perror("Not enough arguments!");
		return NULL;
	}

	if (strcmp(args[1], "start-server") != 0) {
		perror("Unrecognized command!");
		return NULL;
	}
	
	StartServer *data = malloc(sizeof(StartServer));
	data->port = args[2];
	Command *cmd = malloc(sizeof(Command));
	cmd->cmd_name = START_SERVER;
	cmd->cmd_data = (CommandData *)data;
	return cmd;
}
