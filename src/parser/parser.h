#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include "subcommand.h"
#include "parse_result.h"

struct GlobalOpts {
	int verbose;
};

struct ParseCommandLineResult {
	char *sub;
	struct GlobalOpts g_opts;
};

struct ParseCommandLineResult *parse_command_line(int argc, char **argv);
struct ParseSubcommandResult *parse_subcommand(struct Command *cmd, int argc, char **argv);

#endif
