#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

#include "subcommand.h"

void setup_registry(void);
struct Command *registry_get(char *cmd_name);
void free_registry(void);

#endif
