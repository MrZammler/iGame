##########################################################################
# object files (AOS4)
##########################################################################

src/funcs_OS4.o: src/funcs.c src/iGame_strings.h src/strfuncs.h src/fsfuncs.h
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/funcs.c

src/iGameGUI_OS4.o: src/iGameGUI.c src/iGameGUI.h src/iGame_strings.h src/fsfuncs.h
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/iGameGUI.c

src/iGameMain_OS4.o: src/iGameMain.c
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/iGameMain.c

src/strfuncs_OS4.o: src/strfuncs.c src/strfuncs.h
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/strfuncs.c

src/fsfuncs_OS4.o: src/fsfuncs.c src/fsfuncs.h
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/fsfuncs.c
