MAP_GEN = generator
GAME = zelda_generator

all:
	$(MAKE) -C map_generator
	$(MAKE) -C game
	mv map_generator/$(MAP_GEN) .
	mv game/$(GAME) .

clean:
	$(MAKE) clean -C map_generator
	$(MAKE) clean -C game

fclean:	clean
	$(RM) $(MAP_GEN) $(GAME)

re:	fclean all

debug:	fclean
	$(MAKE) debug -C map_generator
	$(MAKE) debug -C game
	mv map_generator/$(MAP_GEN) .
	mv game/$(GAME) .