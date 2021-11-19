##########################################################################
# object files (generic 000)
##########################################################################

src/funcs.o: src/funcs.c src/iGame_cat.h src/strfuncs.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/funcs.c

src/iGameGUI.o: src/iGameGUI.c src/iGameGUI.h src/version.h src/iGame_cat.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/iGameGUI.c

src/iGameMain.o: src/iGameMain.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/iGameMain.c

src/strfuncs.o: src/strfuncs.c src/strfuncs.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/strfuncs.c

src/iGame_cat.o: src/iGame_cat.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/iGame_cat.c
