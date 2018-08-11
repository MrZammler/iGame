##########################################################################
# Makefile for iGame on Windows using VBCC. Assumed VBCC installation directory is D:\vbcc.
#-------------------------------------------------------------------------
# To compile an iGame flat executable  using this makefile, run:
#  make
#-------------------------------------------------------------------------
# To compile an iGame 
#-------------------------------------------------------------------------
# one line build:
# vc -dontwarn=-1 -O2 -o RAM:iGame/iGame -IStuff:Development/MUI/Developer/C/Include -IStuff:Development/MCC_Guigfx/Developer/C/Include -IStuff:Development/MCC_Texteditor/Developer/C/Include -c99 -lamiga -lauto src/funcs.c src/iGameGUI.c src/iGameMain.c src/Hook_utility.o src/strdup.c
##########################################################################

##########################################################################
# Default: Build iGame with standard optimizations and 00 support
##########################################################################
all: iGame

##########################################################################
# Compiler settings
##########################################################################
CC		= vc
LINK	= vc

CFLAGS		= -c +aos68k -dontwarn=-1 -O2 -c99 -I"D:\vbcc\MUI\Developer\C\Include" -I"D:\vbcc\MCC_Guigfx\Developer\C\Include" -I"D:\vbcc\MCC_TextEditor\Developer\C\include"
CFLAGS_030	= -c +aos68k -cpu=68030 -dontwarn=-1 -O2 -c99 -I"D:\vbcc\MUI\Developer\C\Include" -I"D:\vbcc\MCC_Guigfx\Developer\C\Include" -I"D:\vbcc\MCC_TextEditor\Developer\C\include"
CFLAGS_040	= -c +aos68k -cpu=68040 -fpu=68040 -dontwarn=-1 -O2 -c99 -I"D:\vbcc\MUI\Developer\C\Include" -I"D:\vbcc\MCC_Guigfx\Developer\C\Include" -I"D:\vbcc\MCC_TextEditor\Developer\C\include"
CFLAGS_060	= -c +aos68k -cpu=68060 -fpu=68060 -dontwarn=-1 -O2 -c99 -I"D:\vbcc\MUI\Developer\C\Include" -I"D:\vbcc\MCC_Guigfx\Developer\C\Include" -I"D:\vbcc\MCC_TextEditor\Developer\C\include"
CFLAGS_MOS	= -c +morphos -dontwarn=-1 -O2 -c99 I"D:\vbcc\MUI\Developer\C\Include" -I"D:\vbcc\MCC_Guigfx\Developer\C\Include" -I"D:\vbcc\MCC_TextEditor\Developer\C\include"

DATE = $(shell date --iso=date)

##########################################################################
# Library builder settings
##########################################################################
#MKLIB	 = join
LIBFLAGS = +aos68k -lamiga -lauto -lmieee -o
LIBFLAGS_MOS = +morphos -lamiga -lauto -o

##########################################################################
# Object files which are part of the GLFW library
##########################################################################
OBJS = src/funcs.o src/iGameGUI.o src/iGameMain.o src/Hook_utility.o src/strdup.o
OBJS_030 = src/funcs_030.o src/iGameGUI_030.o src/iGameMain_030.o src/Hook_utility.o src/strdup_030.o
OBJS_040 = src/funcs_040.o src/iGameGUI_040.o src/iGameMain_040.o src/Hook_utility.o src/strdup_040.o
OBJS_060 = src/funcs_060.o src/iGameGUI_060.o src/iGameMain_060.o src/Hook_utility.o src/strdup_060.o
OBJS_MOS = src/funcs_MOS.o src/iGameGUI_MOS.o src/iGameMain_MOS.o src/Hook_utility_MOS.o src/strdup_MOS.o

##########################################################################
# Rule for building library
##########################################################################
iGame: $(OBJS)
	$(LINK) $(OBJS) $(LIBFLAGS) $@

iGame.030: $(OBJS_030)
	$(LINK) $(OBJS_030) $(LIBFLAGS) $@

iGame.040: $(OBJS_040)
	$(LINK) $(OBJS_040) $(LIBFLAGS) $@
	
iGame.060: $(OBJS_060)
	$(LINK) $(OBJS_060) $(LIBFLAGS) $@

iGame.MOS: $(OBJS_MOS)
	$(LINK) $(OBJS_MOS) $(LIBFLAGS_MOS) $@

iGame.AOS4:

##########################################################################
# object files (generic 00)
##########################################################################
src/funcs.o: src/funcs.c
	$(CC) $(CFLAGS) -o $@ src/funcs.c

src/iGameGUI.o: src/iGameGUI.c src/iGameGUI.h src/version.h
	$(CC) $(CFLAGS) -o $@ src/iGameGUI.c

src/iGameMain.o: src/iGameMain.c
	$(CC) $(CFLAGS) -o $@ src/iGameMain.c

src/strdup.o: src/strdup.c
	$(CC) $(CFLAGS) -o $@ src/strdup.c

##########################################################################
# object files (030)
##########################################################################

src/funcs_030.o: src/funcs.c
	$(CC) $(CFLAGS_030) -o $@ src/funcs.c

src/iGameGUI_030.o: src/iGameGUI.c src/iGameGUI.h
	$(CC) $(CFLAGS_030) -o $@ src/iGameGUI.c

src/iGameMain_030.o: src/iGameMain.c
	$(CC) $(CFLAGS_030) -o $@ src/iGameMain.c

src/strdup_030.o: src/strdup.c
	$(CC) $(CFLAGS_030) -o $@ src/strdup.c

##########################################################################
# object files (040)
##########################################################################

src/funcs_040.o: src/funcs.c
	$(CC) $(CFLAGS_040) -o $@ src/funcs.c

src/iGameGUI_040.o: src/iGameGUI.c src/iGameGUI.h
	$(CC) $(CFLAGS_040) -o $@ src/iGameGUI.c

src/iGameMain_040.o: src/iGameMain.c
	$(CC) $(CFLAGS_040) -o $@ src/iGameMain.c

src/strdup_040.o: src/strdup.c
	$(CC) $(CFLAGS_040) -o $@ src/strdup.c
	
##########################################################################
# object files (060)
##########################################################################

src/funcs_060.o: src/funcs.c
	$(CC) $(CFLAGS_060) -o $@ src/funcs.c

src/iGameGUI_060.o: src/iGameGUI.c src/iGameGUI.h
	$(CC) $(CFLAGS_060) -o $@ src/iGameGUI.c

src/iGameMain_060.o: src/iGameMain.c
	$(CC) $(CFLAGS_060) -o $@ src/iGameMain.c

src/strdup_060.o: src/strdup.c
	$(CC) $(CFLAGS_060) -o $@ src/strdup.c

##########################################################################
# object files (MOS)
##########################################################################

src/funcs_MOS.o: src/funcs.c
	$(CC) $(CFLAGS_MOS) -o $@ src/funcs.c

src/iGameGUI_MOS.o: src/iGameGUI.c src/iGameGUI.h
	$(CC) $(CFLAGS_MOS) -o $@ src/iGameGUI.c

src/iGameMain_MOS.o: src/iGameMain.c
	$(CC) $(CFLAGS_MOS) -o $@ src/iGameMain.c

src/strdup_MOS.o: src/strdup.c
	$(CC) $(CFLAGS_MOS) -o $@ src/strdup.c

clean:
	del iGame iGame.* src\funcs*.o src\iGameGUI*.o src\iGameMain*.o src\strdup*.o


