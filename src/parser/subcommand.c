#include <stdlib.h>
#include <stdio.h>
#include "subcommand.h"

struct Command *create_command(char *name, char *desc) {
	struct Command *cmd = (struct Command *)malloc(sizeof(struct Command));
	cmd->name = name;
	cmd->desc = desc;
	cmd->flags = NULL;
	cmd->opts = NULL;
	cmd->args = NULL;
	cmd->handler = NULL;
	cmd->num_flags = 0;
	cmd->num_opts = 0;
	cmd->num_args = 0;
	return cmd;
}

void add_opt(struct Command *cmd, char c, char *desc) {
	struct Opt *new_opt = (struct Opt *)malloc(sizeof(struct Opt));
	new_opt->opt_char = c;
	new_opt->desc = desc;
	new_opt->next = cmd->opts;

	cmd->opts = new_opt;
	cmd->num_opts++;
}

void add_flag(struct Command *cmd, char c, char *desc) {
	struct Flag *new_flag = (struct Flag *)malloc(sizeof(struct Flag));
	new_flag->flag_char = c;
	new_flag->desc = desc;
	new_flag->next = cmd->flags; 

	cmd->flags = new_flag;
	cmd->num_flags++;
}

void add_arg(struct Command *cmd, char *desc) {
	struct Arg *new_arg = (struct Arg *)malloc(sizeof(struct Arg));
	new_arg->desc = desc;
	new_arg->next = cmd->args;

	cmd->args = new_arg;
	cmd->num_args++;
}

void set_handler(struct Command *cmd, void(*handler)(struct ParseSubcommandResult *cmd_res)) {
	if (cmd->handler != NULL) {
		printf("%s command already has a handler assigned. Commands can only have one handler", cmd->name);
		return;
	}

	cmd->handler = handler;
}

void print_help(struct Command *cmd) {
	printf("Help for %s... coming soon", cmd->name);
}

void free_command(struct Command *cmd) {
	struct Arg *cur_arg = cmd->args;
	struct Arg *next_arg;

	while (cur_arg != NULL) {
		next_arg = cur_arg->next;
		free(cur_arg);
		cur_arg = next_arg;
	}

	struct Opt *cur_opt = cmd->opts;
	struct Opt *next_opt;

	while (cur_opt != NULL) {
		next_opt = cur_opt->next;
		free(cur_opt);
		cur_opt = next_opt;
	}

	struct Flag *cur_flg = cmd->flags;
	struct Flag *next_flg;

	while (cur_flg != NULL) {
		next_flg = cur_flg->next;
		free(cur_flg);
		cur_flg = next_flg;
	}

	free(cmd);
}
