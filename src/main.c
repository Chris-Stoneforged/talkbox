#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser/parser.h"
#include "parser/command_registry.h"
#include "parser/subcommand.h"

int main(int argc, char **argv) {
	struct ParserResult *r = parse_command(argc, argv);	
	if (r == NULL)
		return 1;

	struct Command **reg = get_registry();
	struct Command **itr = reg;

	while (*itr != NULL) {
		struct Command *cmd = *itr;
		itr++;
		if (strcmp(r->sub, cmd->name) != 0)
			continue;

		if (cmd->handler == NULL) {
			fprintf(stderr, "Command has no handler");
			break;
		}

		parse_subcommand(cmd, argc, argv);
		cmd->handler(cmd);
	}

	free_registry(reg);
	return 0;
}

