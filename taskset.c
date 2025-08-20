
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <err.h>

static void __attribute__((__noreturn__)) usage(char const *name)
{
    fprintf(stderr, "Usage:\n"
                    "    %s 0xf ./test\n", name);
    exit(1);
}

void set_my_affinity(unsigned int aff)
{
	cpu_set_t mask;

	CPU_ZERO(&mask);

	for (int i = 0; i < 32; ++i) {
		if (aff & (1u<<i)) {
			CPU_SET(i, &mask);
		}
	}

	sched_setaffinity(0, sizeof(mask), &mask);
}

int main(int argc, char const *argv[])
{
	char cmd[512];
	unsigned int aff;

	if (argc < 3) {
		fprintf(stderr, "Too few parameters!\n\n");
		usage(argv[0]);
	}

	aff = strtol(argv[1], NULL, 16);

	sprintf(cmd, "%s", argv[2]);
	for (int i = 3; i < argc; ++i) {
		strncat(cmd, " ", sizeof(cmd)-1);
		strncat(cmd, argv[i], sizeof(cmd)-1);
	}

	set_my_affinity(aff);

	return system(cmd);
}
