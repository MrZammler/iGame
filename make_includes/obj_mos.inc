##########################################################################
# object files (MOS)
##########################################################################

src/funcs_MOS.o: src/funcs.c src/iGame_cat.h src/strfuncs.h src/fsfuncs.h
	$(CC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/funcs.c

src/iGameGUI_MOS.o: src/iGameGUI.c src/iGameGUI.h src/iGame_cat.h src/fsfuncs.h
	$(CC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/iGameGUI.c

src/iGameMain_MOS.o: src/iGameMain.c
	$(CC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/iGameMain.c

src/strfuncs_MOS.o: src/strfuncs.c src/strfuncs.h
	$(CC_PPC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/strfuncs.c

src/fsfuncs_MOS.o: src/fsfuncs.c src/fsfuncs.h
	$(CC_PPC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/fsfuncs.c

src/iGame_cat_MOS.o: src/iGame_cat.c
	$(CC_PPC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/iGame_cat.c