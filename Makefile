MAP_GEN =    push_generator

MAP_GEN_SRC = map_generator/map_generator.c

MAP_GEN_OBJS = $(MAP_GEN_SRC:.c=.o)

CFLAGS    =    -Wall -Wextra -Werror

LDFLAGS    = -lraylib -lm

all:    $(MAP_GEN)

$(MAP_GEN):	$(MAP_GEN_OBJS)
	$(CC) -o $(MAP_GEN) $(MAP_GEN_OBJS) $(LDFLAGS)

clean:
	$(RM) $(MAP_GEN_OBJS)

fclean:	clean
	$(RM) $(MAP_GEN)

re:	fclean all

debug:	fclean
	$(CC) -o $(MAP_GEN) $(MAP_GEN_SRC) $(LDFLAGS)