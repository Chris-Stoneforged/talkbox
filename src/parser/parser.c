#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"

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

	struct ParserResult *r = (struct ParserResult *)malloc(sizeof(struct ParserResult));
	r->verbose = v;	

	if (optind < argc)
		r->sub = argv[optind];
	else
		r->sub = "";

	return r;
}
