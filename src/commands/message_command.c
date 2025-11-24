#include <stdio.h>
#include <stdlib.h>
#include "message_command.h"
#include "../test/test_message_command.h"

struct Command *get_message_command(void) {
	struct Command *cmd = create_command("message", "Sends a message to a client");

	add_arg(cmd, "The user to send the message to");
	add_opt(cmd, 'm', "The message body");
	add_flag(cmd, 'a', "Send message annonymously");
	set_handler(cmd, handle_message_command);

	return cmd;
}
