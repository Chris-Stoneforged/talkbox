#ifndef MESSAGE_COMMAND_H
#define MESSAGE_COMMAND_H

#include "subcommand.h"

struct Command *get_message_command(void);
void handle_message_command(struct Command *cmd);

#endif
