#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

#include "subcommand.h"

struct Command **get_registry(void);
void free_registry(struct Command **registry);

#endif
