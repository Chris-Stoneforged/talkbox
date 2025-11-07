#ifndef CMD_PARSER_H
#define CMD_PARSER_H

struct ParserResult {
	char *sub;
	int verbose;
};

struct ParserResult *parse_command(int argc, char **argv);

#endif
