CC		 ?= /usr/bin/gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
	-Wshadow -Wpointer-arith -O3 -fomit-frame-pointer

libpmu.so: pmu.c
	$(CC) $(CFLAGS) -fPIC -shared $^ -o $@
