MAP_GEN = ../generator

MAP_GEN_SRC = main_generator.c \
              map.c \
              map_generator.c \

MAP_GEN_OBJS = $(MAP_GEN_SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lraylib -lm

$(MAP_GEN):	$(MAP_GEN_OBJS)
	$(CC) -o $(MAP_GEN) $(MAP_GEN_OBJS) $(LDFLAGS)

all:	$(MAP_GEN)

clean:
	$(RM) $(MAP_GEN_OBJS)

debug:	fclean
	$(CC) -o $(MAP_GEN) $(MAP_GEN_SRC) $(LDFLAGS)