#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "kill_server_command.h"
#include "../network/server/server.h"

void handle_kill_server_command(int argc, char **argv) {
	if (argc > 0) {
		printf("Unexpected positional argument %s.\n", argv[0]);
		return;
	}

	kill_server();
}
