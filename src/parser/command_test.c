#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "command_test.h"
#include "subcommand.h"

char *get_getopt_string(struct Command *cmd);
void print_err(struct Opt *opts);


void test_commands(int argc, char **argv) {
	struct Command *cmd = create_message_command();

	int result = parse_commands(cmd, argc, argv);
	if (result != 0)
		return;

	cmd->handler(cmd);
}

struct Command *create_message_command(void) {
	struct Command *cmd = create_subcommand("message", "Send a message to another user");

	add_arg(cmd, "User to send this message to");
	add_arg(cmd, "The message to send to the user");
	add_flag(cmd, 'a', "If this flag is set, send the message annonymously");
	add_flag(cmd, 'u', "If this flag is set, notify the user that this message is urgent");
	add_opt(cmd, 'p', "Prefix to add to this message");
	set_handler(cmd, handle_message_cmd);

	return cmd;
}

void handle_message_cmd(struct Command *cmd) {
	int a_val = get_flag_value(cmd, 'a');
	int u_val = get_flag_value(cmd, 'u');
	char *pref = get_opt_value(cmd, 'p');
	char *user = get_arg_value(cmd, 0);
	char *msg = get_arg_value(cmd, 1);

	printf("user: %s, msg: %s, a: %d, u: %d, p: %s\n", user, msg, a_val, u_val, pref);
}

int parse_commands(struct Command *cmd, int argc, char **argv) {
	char f;

	// Increment argv until after the command name (and decrement argc to match)
	do 
		argc--;	
	while (strcmp(*argv++, cmd->name) != 0); 

	// Get positional arguments
	for (struct Arg *arg = cmd->args; arg != NULL; arg = arg->next) {
		char *value = *argv++;
		if (value == NULL || *value == '-') {
			fprintf(stderr, "Missing positional argument\n");
			return -1;
		}
		argc--;
		arg->val = value;
	}
	
	// Set optind to 0 so it gets every arg after the last positional arg
	optind = 0;
	char *str = get_getopt_string(cmd);
	
	while ((f = getopt(argc, argv, str)) != -1) {
		if (f == '?') {
			print_err(cmd->opts);
			free(str);
			return -1;
		}

		for (struct Flag *flg = cmd->flags; flg != NULL; flg = flg->next) {
			if (f == flg->flag_char)
				flg->val = 1;
		}
		for (struct Opt *opt = cmd->opts; opt != NULL; opt = opt->next) {
			if (f == opt->opt_char)
				opt->val = optarg;
		}
	}

	free(str);
	if (optind < argc) {
		printf("Unexpected positional argument '%s'.\n", argv[optind]);
		return -1;
	}

	return 0; // Success
}

char *get_getopt_string(struct Command *cmd) {
	int len = 0;
	struct Flag *f;
	struct Opt *o;
	char *str, *tmp;

	for (f = cmd->flags; f != NULL; f = f->next)
		len += 1;
	for (o = cmd->opts; o != NULL; o = o->next)
		len += 2;

	str = (char *)malloc(len * sizeof(char));
	tmp = str;
	
	for (f = cmd->flags; f != NULL; f = f->next)
		*tmp++ = f->flag_char;
	for (o = cmd->opts; o != NULL; o = o->next) {
		*tmp++ = o->opt_char;
		*tmp++ = ':';
	}

	return str;
}

void print_err(struct Opt *opts) {
	struct Opt *opt;
	for (opt = opts; opt != NULL; opt = opt->next) {
		if (opt->opt_char == optopt) {
			fprintf(stderr, "Option '-%c' requires an argument.\n", optopt);
			return;
		}
	}	
	
	if (isprint(optopt))
		fprintf(stderr, "Unknown option '-%c'\n", optopt);
	else 
		fprintf(stderr, "Unknown option character '%x'\n", optopt);
}
