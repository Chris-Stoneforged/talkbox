#include <stdlib.h>
#include <stdio.h>
#include "parse_result.h"

int get_flag_value(struct ParseSubcommandResult *res, char c) {
	for (int i = 0; i < res->flg_len; i++) {
		if (res->flg_res[i] == c)
			return 1;
	}

	return 0;
}

char *get_opt_value(struct ParseSubcommandResult *res, char c) {
	for (int i = 0; i < res->opt_len; i++) {
		struct OptResult *opt = res->opt_res[i];
		if (opt != NULL && opt->opt == c)
			return opt->val;
	}

	return NULL;
}

char *get_arg_value(struct ParseSubcommandResult *res, int index) {
	if (index < 0 || index >= res->arg_len) {
		return NULL;
	}

	return res->arg_res[index];
}

void free_subcommand_result(struct ParseSubcommandResult *res) {
	if (res == NULL)
		return;

	for (int i = 0; i < res->opt_len; i++) {
		if (res->opt_res[i])
			free(res->opt_res[i]);
	}

	free(res->opt_res);
	free(res->arg_res);
	free(res->flg_res);
	free(res);
}
