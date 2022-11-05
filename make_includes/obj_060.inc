##########################################################################
# object files (060)
##########################################################################

src/funcs_060.o: src/funcs.c src/iGame_strings.h src/strfuncs.h src/fsfuncs.h src/iGameExtern.h
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/funcs.c

src/iGameGUI_060.o: src/iGameGUI.c src/iGameGUI.h src/iGame_strings.h src/fsfuncs.h src/iGameExtern.h
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/iGameGUI.c

src/iGameMain_060.o: src/iGameMain.c src/iGameExtern.h
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/iGameMain.c

src/strfuncs_060.o: src/strfuncs.c src/strfuncs.h
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/strfuncs.c

src/fsfuncs_060.o: src/fsfuncs.c src/fsfuncs.h src/funcs.h src/iGameExtern.h
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/fsfuncs.c
