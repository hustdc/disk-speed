# paths
PREFIX = /usr/local

# cc options
CFLAGS += -std=c11 -Wall -Wextra -Wtype-limits -Wconversion -Ofast
CC = musl-gcc -static
