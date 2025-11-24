#ifndef PARSE_RESULT_H
#define PARSE_RESULT_H

struct OptResult {
	char opt;
	char *val;
};

struct ParseSubcommandResult {
	char *flg_res;
	struct OptResult** opt_res;
	char** arg_res;
	int arg_len;
	int opt_len;
	int flg_len;
};

int get_flag_value(struct ParseSubcommandResult *cmd_res, char c);
char *get_opt_value(struct ParseSubcommandResult *cmd_res, char c);
char *get_arg_value(struct ParseSubcommandResult *cmd_res, int index);
void free_subcommand_result(struct ParseSubcommandResult *cmd_res);

#endif
