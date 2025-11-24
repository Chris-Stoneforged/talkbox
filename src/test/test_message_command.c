#include <stdio.h>
#include <stdlib.h>
#include "../parser/parse_result.h"

void handle_message_command(struct ParseSubcommandResult *res) {
	char *usr = get_arg_value(res, 0);
	char *msg = get_opt_value(res, 'm');
	int a = get_flag_value(res, 'a');

	if (usr == NULL)
		return;

	printf("Sent message \"%s\" to %s%s", msg == NULL ? "(TBD)" : msg, usr, a == 1 ? " (annonymously)\n" : "\n");
}
