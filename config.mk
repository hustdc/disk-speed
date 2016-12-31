# paths
PREFIX = /usr/local

# cc options
CFLAGS += -std=std11 -Wall -Wextra -Wtype-limits -Wconversion -Ofast
CC = musl-gcc -static
