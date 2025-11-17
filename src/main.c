#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser/parser.h"
#include "commands/commands.h"
#include "parser/command_test.h"

int main(int argc, char **argv) {
	struct ParserResult *r = parse_command(argc, argv);	
	if (r == NULL)
		return 1;

	test_commands(argc, argv);
	return 0;

	do {
		argc--;
	} while (strcmp(*argv++, r->sub) != 0);

	handle_commands(r, argc, argv);
	free(r);

	return 0;
}
