#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "start_server_command.h"
#include "../network/server/server.h"

struct Command *get_start_server_command(void) {
	struct Command *cmd = create_command("start-server", "Starts the server");

	add_opt(cmd, 'p', "The port to connect on");
	add_opt(cmd, 'c', "The maximum number of clients the server can permit");
	set_handler(cmd, start_server);

	return cmd;
}
