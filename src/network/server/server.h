#ifndef SERVER_H
#define SERVER_H

#include "../../parser/parse_result.h"

void start_server(struct ParseSubcommandResult *res);
void kill_server(void);

#endif
