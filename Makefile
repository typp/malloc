##
## Makefile for Malloc in /home/la-val_c/rendu/PSU_2013_malloc
##
## Made by Cyril La Valle
## Login   <la-val_c@epitech.net>
##
## Started on  Tue Feb  3 10:42:39 2015 Cyril La Valle
## Last update Mon Feb 16 12:55:27 2015 Zadkiel Aslafy-Aharonian
##

CC	:= gcc
CFLAGS	:= -W -Wall -Wextra -O3 -rdynamic
LDFLAGS	:= -pthread

SRCS	= \
	malloc.c \
	realloc.c \
	arena.c \
	bins.c \
	bins_utils.c \
	fast.c \
	wide.c \
	wide_utils.c \
	wide_algos.c \
	pearson.c \
	malign.c \
	dbg_mem_map.c \
	dbg_heap_map.c \
	fatal.c \
	backtrace.c

OBJS	= $(SRCS:.c=.o)

LIB	= libmy_malloc_$(HOSTTYPE).so

all: $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) -c -fPIC $< -o $@

$(LIB): $(OBJS)
	$(CC) $^ -shared -fPIC -o $(LIB) $(LDFLAGS)
	ln -fs $(LIB) libmy_malloc.so

test: $(SRCS) test.c
	$(CC) $(CFLAGS) $^ -o test.out -pthread -rdynamic

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f libmy_malloc.so
	rm -f $(LIB)

re: fclean all

.PHONY: all clean fclean re
