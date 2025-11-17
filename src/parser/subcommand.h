#ifndef SUBCOMMAND_H
#define SUBCOMMAND_H

struct Flag {
	char flag_char;
	char *desc;
	int val;
	struct Flag *next;
};

struct Opt {
	char opt_char;
	char *desc;
	char *val;
	struct Opt *next;
};

struct Arg {
	char *desc;
	char *val;
	struct Arg *next;
};

struct Command {
	char *name;
	char *desc;
	struct Flag *flags;
	struct Opt *opts;
	struct Arg *args;
	void (*handler)(struct Command * cmd);
};

struct Command *create_subcommand(char *name, char *desc);
int get_flag_value(struct Command *cmd, char c);
char *get_opt_value(struct Command *cmd, char c);
char *get_arg_value(struct Command *cmd, int index);
void add_flag(struct Command *cmd, char c, char *desc);
void add_opt(struct Command *cmd, char c, char *desc);
void add_arg(struct Command *cmd, char *desc);
void set_handler(struct Command *cmd, void (*handler)(struct Command *cmd));
void print_help(struct Command *cmd);

#endif
