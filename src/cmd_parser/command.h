#ifndef COMMAND_H
#define COMMAND_H

struct Flag {
	char flag_char;
	char *desc;
	struct Flag *next;
};

struct Option {
	char opt_char;
	char *desc;
	struct Option *next;
};

struct Argument {
	char *desc;
	struct Argument *next;
};

struct Command {
	char *name;
	char * desc;
	int num_args;
	int num_opts;
	int num_flags;
	struct Flag *flags;
	struct Option *opts;
	struct Argument *args;
};

struct Command *create_command(char *name, char *desc);
void add_flag(struct Command *command, char flag, char *desc);
void add_arg(struct Command *command, char *desc); 
void add_opt(struct Command *command, char flag, char *desc);
char *get_getopt_string(struct Command *command);

#endif
