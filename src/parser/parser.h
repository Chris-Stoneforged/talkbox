#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include "subcommand.h"

struct GlobalOpts {
	int verbose;
};

struct ParserResult {
	char *sub;
	struct GlobalOpts g_opts;
};

struct ParserResult *parse_command(int argc, char **argv);
int parse_subcommand(struct Command *cmd, int argc, char **argv);

#endif
