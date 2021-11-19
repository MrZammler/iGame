##########################################################################
# object files (030)
##########################################################################

src/funcs_030.o: src/funcs.c src/iGame_cat.h src/strfuncs.h src/fsfuncs.h
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/funcs.c

src/iGameGUI_030.o: src/iGameGUI.c src/iGameGUI.h src/iGame_cat.h src/fsfuncs.h
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/iGameGUI.c

src/iGameMain_030.o: src/iGameMain.c
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/iGameMain.c

src/strfuncs_030.o: src/strfuncs.c src/strfuncs.h
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/strfuncs.c

src/fsfuncs_030.o: src/fsfuncs.c src/fsfuncs.h src/funcs.h src/iGameExtern.h
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/fsfuncs.c

src/iGame_cat_030.o: src/iGame_cat.c
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/iGame_cat.c
