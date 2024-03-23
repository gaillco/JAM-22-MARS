GAME = ../zelda_generator

GAME_SRC = main_game.c \
           ../map_generator/map.c \
           ../map_generator/map_generator.c \

GAME_OBJS = $(GAME_SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lraylib -lm

$(GAME):	$(GAME_OBJS)
	$(CC) -o $(GAME) $(GAME_OBJS) $(LDFLAGS)

all:	$(GAME)

clean:
	$(RM) $(GAME_OBJS)

debug:	fclean
	$(CC) -o $(GAME) $(GAME_SRC) $(LDFLAGS)