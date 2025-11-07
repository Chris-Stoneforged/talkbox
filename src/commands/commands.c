#include <stdio.h>
#include <string.h>
#include "message_command.h"
#include "start_server_command.h"
#include "../parser/parser.h"

void handle_commands(struct ParserResult *r, int argc, char **argv) {
	#define sub_equals(str) (strcmp(r->sub, str) == 0)

	if (sub_equals("message"))
		handle_message_command(argc, argv);
	else if (sub_equals("start-server")) 
		handle_start_server_command(argc, argv);
	else
		printf("Nothing to do!\n");
}
