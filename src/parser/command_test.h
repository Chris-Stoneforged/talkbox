#include "subcommand.h"

void test_commands(int argc, char **argv);
int parse_commands(struct Command *cmd, int argc, char **argv);
void handle_message_cmd(struct Command *cmd);
struct Command *create_message_command(void);
