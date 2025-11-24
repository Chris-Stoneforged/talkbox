#include <stdlib.h>
#include <string.h>
#include "command_registry.h"
#include "../commands/message_command.h"
#include "../commands/start_server_command.h"
#include "subcommand.h"

#define MAX_COMMANDS 10

static struct Command **registry = NULL;

void setup_registry(void) {
	registry = (struct Command **)malloc(sizeof(struct Command *) * MAX_COMMANDS);

	registry[0] = get_message_command();
	registry[1] = get_start_server_command();
}

struct Command *registry_get(char *cmd_name) {
	for (int i = 0; i < MAX_COMMANDS; i++) {
		struct Command *cmd = registry[i];
		if (cmd == NULL)
			continue;

		if (strcmp(cmd->name, cmd_name) == 0) 
			return cmd;
	}

	return NULL;
}

void free_registry(void) {
	for (int i = 0; i < MAX_COMMANDS; i++) {
		if (registry[i] == NULL)
			continue;

		free_command(registry[i]);
	}

	free(registry);
}
