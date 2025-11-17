#include <stdlib.h>
#include <stdio.h>
#include "subcommand.h"

int get_flag_value(struct Command *cmd, char c) {
	for (struct Flag *f = cmd->flags; f != NULL; f = f->next) {
		if (f->flag_char == c)
			return f->val;
	}

	fprintf(stderr, "No flag exists with character %c\n", c);
	return 0;
}

char *get_opt_value(struct Command *cmd, char c) {
	for (struct Opt *o = cmd->opts; o != NULL; o = o->next) {
		if (o->opt_char == c)
			return o->val;
	}

	fprintf(stderr, "No opt exists with character %c\n", c);
	return NULL;
}

char *get_arg_value(struct Command *cmd, int index) {
	int c = 0;
	for (struct Arg *a = cmd->args; a != NULL; a = a->next) {
		if (c == index)
			return a->val;
		c++;
	}

	fprintf(stderr, "No arg exists at index %d\n", index);
	return NULL;
}

struct Command *create_subcommand(char *name, char *desc) {
	struct Command *cmd = (struct Command *)malloc(sizeof(struct Command));
	cmd->name = name;
	cmd->desc = desc;
	cmd->handler = NULL;
	cmd->flags = NULL;
	cmd->opts = NULL;
	cmd->args = NULL;
	return cmd;
}

void add_opt(struct Command *cmd, char c, char *desc) {
	struct Opt *new_opt = (struct Opt *)malloc(sizeof(struct Opt));
	new_opt->opt_char = c;
	new_opt->desc = desc;
	new_opt->next = cmd->opts;

	cmd->opts = new_opt;
}

void add_flag(struct Command *cmd, char c, char *desc) {
	struct Flag *new_flag = (struct Flag *)malloc(sizeof(struct Flag));
	new_flag->flag_char = c;
	new_flag->desc = desc;
	new_flag->next = cmd->flags; 

	cmd->flags = new_flag;
}

void add_arg(struct Command *cmd, char *desc) {
	struct Arg *new_arg = (struct Arg *)malloc(sizeof(struct Arg));
	new_arg->desc = desc;
	new_arg->next = cmd->args;

	cmd->args = new_arg;
}

void set_handler(struct Command *cmd, void (*handler)(struct Command *cmd)) {
	cmd->handler = handler;
}

void print_help(struct Command *cmd) {
	printf("Help... coming soon");
}
