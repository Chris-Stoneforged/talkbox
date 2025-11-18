#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include "subcommand.h"

char *get_getopt_string(struct Command *cmd);
void print_err(struct Opt *opts);

struct ParserResult *parse_command(int argc, char **argv) {
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

	struct ParserResult *r = (struct ParserResult *)malloc(sizeof(struct ParserResult));
	r->g_opts = gopts;

	if (optind < argc)
		r->sub = argv[optind];
	else
		r->sub = "";

	return r;
}

int parse_subcommand(struct Command *cmd, int argc, char **argv) {
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
