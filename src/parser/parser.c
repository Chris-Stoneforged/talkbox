#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include "parse_result.h"
#include "subcommand.h"

// Forward declarations
char *get_getopt_string(struct Command *cmd);
void print_err(struct Opt *opts);
struct ParseSubcommandResult *create_result_object(struct Command *cmd);
void add_opt_to_result(struct ParseSubcommandResult *res, char c, char *val);
void add_flag_to_result(struct ParseSubcommandResult *res, char c);

struct ParseCommandLineResult *parse_command_line(int argc, char **argv) {
	int v = 0, f;
	opterr = 0;

	while ((f = getopt(argc, argv, "v")) != -1) {
		switch (f) 
		{
		case 'v':
			v = 1;
			break;
		case '?':
			if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else 
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return NULL;
		}
	}

	struct GlobalOpts gopts;
	gopts.verbose = v;

	struct ParseCommandLineResult *r = (struct ParseCommandLineResult *)malloc(sizeof(struct ParseCommandLineResult));
	r->g_opts = gopts;

	if (optind < argc)
		r->sub = argv[optind];
	else
		r->sub = "";

	return r;
}

struct ParseSubcommandResult *parse_subcommand(struct Command *cmd, int argc, char **argv) {
	struct ParseSubcommandResult *res = create_result_object(cmd);
	char f;

	// Increment argv until after the command name (and decrement argc to match)
	do 
		argc--;	
	while (strcmp(*argv++, cmd->name) != 0); 

	// Get positional arguments
	char **v = res->arg_res;
	for (struct Arg *arg = cmd->args; arg != NULL; arg = arg->next) {
		char *value = *argv++;
		if (value == NULL || *value == '-') {
			fprintf(stderr, "Missing positional argument\n");
			free_subcommand_result(res);
			return NULL;
		}
		argc--;
		*v++ = value;
	}

	// Set optind to 0 so it gets every arg after the last positional arg
	optind = 0;
	char *str = get_getopt_string(cmd);

	while ((f = getopt(argc, argv, str)) != -1) {
		if (f == '?') {
			print_err(cmd->opts);
			free(str);
			free_subcommand_result(res);
			return NULL;
		}

		for (struct Flag *flg = cmd->flags; flg != NULL; flg = flg->next) {
			if (f == flg->flag_char)
				add_flag_to_result(res, f);
		}
		for (struct Opt *opt = cmd->opts; opt != NULL; opt = opt->next) {
			if (f == opt->opt_char)
				add_opt_to_result(res, f, optarg);
		}
	}

	free(str);
	if (optind < argc) {
		printf("Unexpected positional argument '%s'.\n", argv[optind]);
		free_subcommand_result(res);
		return NULL;
	}

	return res;
}

struct ParseSubcommandResult *create_result_object(struct Command *cmd) {
	struct ParseSubcommandResult *res = (struct ParseSubcommandResult *)malloc(sizeof(struct ParseSubcommandResult));

	res->arg_len = cmd->num_args;
	res->flg_len = cmd->num_flags;
	res->opt_len= cmd->num_opts;
	res->arg_res = (char **)malloc(sizeof(char *) * cmd->num_args);
	res->flg_res = (char *)malloc(sizeof(char) * cmd->num_flags);
	res->opt_res = (struct OptResult **)malloc(sizeof(struct OptResult) * cmd->num_opts);

	return res;
}

void add_opt_to_result(struct ParseSubcommandResult *res, char c, char *val) {
	for (int i = 0; i < res->opt_len; i++) {
		if (res->opt_res[i] != NULL) 
			continue;
	
		struct OptResult *opt = (struct OptResult *)malloc(sizeof(struct OptResult));
		opt->opt = c;
		opt->val = val;
		res->opt_res[i] = opt;
		return;
	}
}

void add_flag_to_result(struct ParseSubcommandResult *res, char c) {
	for (int i = 0; i < res->flg_len; i++) {
		if (res->flg_res[i] == 0)
			res->flg_res[i] = c;

	}
}

char *get_getopt_string(struct Command *cmd) {
	int len = cmd->num_flags + (2 * cmd->num_opts);
	struct Flag *f;
	struct Opt *o;
	char *str, *tmp;

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
