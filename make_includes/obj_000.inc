##########################################################################
# object files (generic 000)
##########################################################################

src/funcs.o: src/funcs.c src/iGame_strings.h src/strfuncs.h src/fsfuncs.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/funcs.c

src/iGameGUI.o: src/iGameGUI.c src/iGameGUI.h src/iGame_strings.h src/fsfuncs.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/iGameGUI.c

src/iGameMain.o: src/iGameMain.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/iGameMain.c

src/strfuncs.o: src/strfuncs.c src/strfuncs.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/strfuncs.c

src/fsfuncs.o: src/fsfuncs.c src/fsfuncs.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/fsfuncs.c
