#ifndef SUBCOMMAND_H
#define SUBCOMMAND_H

#include "parse_result.h"

struct Flag {
	char flag_char;
	char *desc;
	struct Flag *next;
};

struct Opt {
	char opt_char;
	char *desc;
	struct Opt *next;
};

struct Arg {
	char *desc;
	struct Arg *next;
};

struct Command {
	char *name;
	char *desc;
	int num_flags;
	int num_opts;
	int num_args;
	struct Flag *flags;
	struct Opt *opts;
	struct Arg *args;
	void(*handler)(struct ParseSubcommandResult *cmd_res);
};

struct Command *create_command(char *name, char *desc);
void add_flag(struct Command *cmd, char c, char *desc);
void add_opt(struct Command *cmd, char c, char *desc);
void add_arg(struct Command *cmd, char *desc);
void set_handler(struct Command *cmd, void(*handler)(struct ParseSubcommandResult *cmd_res));
void print_help(struct Command *cmd);
void free_command(struct Command *cmd);

#endif
