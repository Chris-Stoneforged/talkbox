#include <stdio.h>
#include <stdlib.h>
#include "message_command.h"
#include "subcommand.h"

struct Command *get_message_command(void) {
	struct Command *cmd = create_subcommand("message", "Sends a message to a client");

	set_handler(cmd, handle_message_command);
	add_arg(cmd, "The user to send the message to");
	add_opt(cmd, 'm', "The message body");
	add_flag(cmd, 'a', "Send message annonymously");

	return cmd;
}

void handle_message_command(struct Command *cmd) {
	char *usr = get_arg_value(cmd, 0);
	char *msg = get_opt_value(cmd, 'm');
	int a = get_flag_value(cmd, 'a');

	if (usr == NULL)
		return;

	printf("Sent message \"%s\" to %s%s", msg, usr, a == 1 ? " (annonymously)\n" : "\n");
}
