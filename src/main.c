#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser/parse_result.h"
#include "parser/parser.h"
#include "parser/command_registry.h"
#include "parser/subcommand.h"

int main(int argc, char **argv) {
	int r = 1;
	setup_registry();
	
	struct ParseCommandLineResult *pclr = parse_command_line(argc, argv);	
	if (pclr == NULL) {
		goto err_1;
	}

	struct Command *cmd = registry_get(pclr->sub);
	if (cmd == NULL) {
		goto err_1;
	}

	struct ParseSubcommandResult *pcr = parse_subcommand(cmd, argc, argv);
	if (pcr == NULL) {
		goto err_1;
	}

	if (cmd->handler == NULL) {
		goto err_0;
	}

	cmd->handler(pcr);
	r = 0;

err_0:
	free_subcommand_result(pcr);
err_1:
	free(pclr);
	free_registry();

	return r;
}

