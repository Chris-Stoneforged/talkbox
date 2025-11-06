#include <stdlib.h>
#include "command.h"

struct Command *create_command(char *name, char *desc) {
	struct Command *command = (struct Command *)malloc(sizeof(struct Command));
	command->name = name;
	command->desc = desc;
	command->flags = NULL;
	command->args = NULL;
	command->opts = NULL;
	command->num_flags = 0;
	command->num_opts = 0;
	command->num_args = 0;
	return command;
}

void add_flag(struct Command *command, char flag, char *desc) {
	struct Flag* new_flag = (struct Flag *)malloc(sizeof(struct Flag));	
	new_flag->flag_char = flag;
	new_flag->desc = desc;
	new_flag->next = command->flags;

	command->flags = new_flag;
	command->num_flags++;
}

void add_arg(struct Command *command, char *desc) {
	struct Argument *new_arg = (struct Argument *)malloc(sizeof(struct Argument));
	new_arg->desc = desc;
	new_arg->next = command->args;

	command->args = new_arg;
	command->num_args++;
}

void add_opt(struct Command *command, char flag, char *desc) {
	struct Option *new_opt = (struct Option *)malloc(sizeof(struct Option));
	new_opt->opt_char = flag;
	new_opt->desc = desc;
	new_opt->next = command->opts;

	command->opts = new_opt;
	command->num_opts++;
}

char *get_getopt_string(struct Command *command) {
	struct Flag *flag;
	struct Option *opt;

	int str_len = command->num_flags + (command->num_opts * 2) + 1;
	char *s = (char *)malloc(sizeof(char) * str_len);
	char *t = s;

	for (flag = command->flags; flag != NULL; flag = flag->next) {
		*t++ = flag->flag_char;
	}

	for (opt = command->opts; opt != NULL; opt = opt->next) {
		*t++ = opt->opt_char;
		*t++ = ':';
	}

	*t = '\0';
	return s;
}
