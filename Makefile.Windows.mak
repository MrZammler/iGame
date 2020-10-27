##########################################################################
# Makefile for iGame on Windows using VBCC. Assumed VBCC installation directory is D:\vbcc.
#-------------------------------------------------------------------------
# To compile an iGame flat executable  using this makefile, run:
#  make -f Makefile.Windows.mak
#-------------------------------------------------------------------------
##########################################################################

##########################################################################
# Default: Build iGame with standard optimizations and 00 support
##########################################################################
all: iGame

##########################################################################
# Compiler settings
##########################################################################
CC			= vc
LINK		= vc
INCLUDES	= -I"D:\vbcc\MUI\Developer\C\Include" -I"D:\vbcc\MCC_Guigfx\Developer\C\Include" -I"D:\vbcc\MCC_TextEditor\Developer\C\include"
CFLAGS			= -c +aos68k -dontwarn=-1 -O2 -c99 -DCPU_VERS=68000
CFLAGS_030	= -c +aos68k -cpu=68030 -dontwarn=-1 -O2 -c99 -DCPU_VERS=68030
CFLAGS_040	= -c +aos68k -cpu=68040 -dontwarn=-1 -O2 -c99 -DCPU_VERS=68040
CFLAGS_060	= -c +aos68k -cpu=68060 -dontwarn=-1 -O2 -c99 -DCPU_VERS=68060
CFLAGS_MOS	= -c +morphos -dontwarn=-1 -O2 -c99 -DCPU_VERS=MorphOS
CFLAGS_AOS4	= -c +aosppc -dontwarn=-1 -O2 -c99 -DCPU_VERS=AmigaOS4

DATE = $(shell date --iso=date)

##########################################################################
# Library builder settings
##########################################################################
#MKLIB	 = join
LIBFLAGS		= +aos68k -lamiga -lauto -o
LIBFLAGS_MOS	= +morphos -lamiga -lauto -o
LIBFLAGS_AOS4	= +aosppc -lamiga -lauto -o

##########################################################################
# Object files which are part of the GLFW library
##########################################################################
OBJS		= src/funcs.o src/iGameGUI.o src/iGameMain.o src/strcasestr.o src/strdup.o src/iGame_cat.o
OBJS_030	= src/funcs_030.o src/iGameGUI_030.o src/iGameMain_030.o src/strcasestr_030.o src/strdup_030.o src/iGame_cat_030.o
OBJS_040	= src/funcs_040.o src/iGameGUI_040.o src/iGameMain_040.o src/strcasestr_040.o src/strdup_040.o src/iGame_cat_040.o
OBJS_060	= src/funcs_060.o src/iGameGUI_060.o src/iGameMain_060.o src/strcasestr_060.o src/strdup_060.o src/iGame_cat_060.o
OBJS_MOS	= src/funcs_MOS.o src/iGameGUI_MOS.o src/iGameMain_MOS.o src/strcasestr_MOS.o src/strdup_MOS.o src/iGame_cat_MOS.o
OBJS_AOS4	= src/funcs_AOS4.o src/iGameGUI_AOS4.o src/iGameMain_AOS4.o src/strcasestr_AOS4.o src/strdup_AOS4.o src/iGame_cat_AOS4.o

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

iGame.AOS4: $(OBJS_AOS4)
	$(LINK) $(OBJS_AOS4) $(LIBFLAGS_AOS4) $@

##########################################################################
# generated source files
##########################################################################
src/iGame_cat.c: src/iGame.cd src/C_c.sd
	cd src && flexcat iGame.cd iGame_cat.c=C_c.sd

src/iGame_cat.h: src/iGame.cd src/C_h.sd
	cd src && flexcat iGame.cd iGame_cat.h=C_h.sd

##########################################################################
# object files (generic 00)
##########################################################################
src/funcs.o: src/funcs.c src/iGame_cat.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/funcs.c

src/iGameGUI.o: src/iGameGUI.c src/iGameGUI.h src/version.h src/iGame_cat.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/iGameGUI.c

src/iGameMain.o: src/iGameMain.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/iGameMain.c

src/strcasestr.o: src/strcasestr.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/strcasestr.c

src/strdup.o: src/strdup.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/strdup.c

src/iGame_cat.o: src/iGame_cat.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ src/iGame_cat.c

##########################################################################
# object files (030)
##########################################################################

src/funcs_030.o: src/funcs.c src/iGame_cat.h
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/funcs.c

src/iGameGUI_030.o: src/iGameGUI.c src/iGameGUI.h src/iGame_cat.h
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/iGameGUI.c

src/iGameMain_030.o: src/iGameMain.c
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/iGameMain.c

src/strcasestr_030.o: src/strcasestr.c
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/strcasestr.c

src/strdup_030.o: src/strdup.c
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/strdup.c

src/iGame_cat_030.o: src/iGame_cat.c
	$(CC) $(CFLAGS_030) $(INCLUDES) -o $@ src/iGame_cat.c

##########################################################################
# object files (040)
##########################################################################

src/funcs_040.o: src/funcs.c src/iGame_cat.h
	$(CC) $(CFLAGS_040) $(INCLUDES) -o $@ src/funcs.c

src/iGameGUI_040.o: src/iGameGUI.c src/iGameGUI.h src/iGame_cat.h
	$(CC) $(CFLAGS_040) $(INCLUDES) -o $@ src/iGameGUI.c

src/iGameMain_040.o: src/iGameMain.c
	$(CC) $(CFLAGS_040) $(INCLUDES) -o $@ src/iGameMain.c

src/strcasestr_040.o: src/strcasestr.c
	$(CC) $(CFLAGS_040) $(INCLUDES) -o $@ src/strcasestr.c

src/strdup_040.o: src/strdup.c
	$(CC) $(CFLAGS_040) $(INCLUDES) -o $@ src/strdup.c

src/iGame_cat_040.o: src/iGame_cat.c
	$(CC) $(CFLAGS_040) $(INCLUDES) -o $@ src/iGame_cat.c

##########################################################################
# object files (060)
##########################################################################

src/funcs_060.o: src/funcs.c src/iGame_cat.h
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/funcs.c

src/iGameGUI_060.o: src/iGameGUI.c src/iGameGUI.h src/iGame_cat.h
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/iGameGUI.c

src/iGameMain_060.o: src/iGameMain.c
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/iGameMain.c

src/strcasestr_060.o: src/strcasestr.c
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/strcasestr.c

src/strdup_060.o: src/strdup.c
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/strdup.c

src/iGame_cat_060.o: src/iGame_cat.c
	$(CC) $(CFLAGS_060) $(INCLUDES) -o $@ src/iGame_cat.c

##########################################################################
# object files (MOS)
##########################################################################

src/funcs_MOS.o: src/funcs.c src/iGame_cat.h
	$(CC) $(CFLAGS_MOS) $(INCLUDES) -o $@ src/funcs.c

src/iGameGUI_MOS.o: src/iGameGUI.c src/iGameGUI.h src/iGame_cat.h
	$(CC) $(CFLAGS_MOS) $(INCLUDES) -o $@ src/iGameGUI.c

src/iGameMain_MOS.o: src/iGameMain.c
	$(CC) $(CFLAGS_MOS) $(INCLUDES) -o $@ src/iGameMain.c

src/strcasestr_MOS.o: src/strcasestr.c
	$(CC) $(CFLAGS_MOS) $(INCLUDES) -o $@ src/strcasestr.c

src/strdup_MOS.o: src/strdup.c
	$(CC) $(CFLAGS_MOS) $(INCLUDES) -o $@ src/strdup.c

src/iGame_cat_MOS.o: src/iGame_cat.c
	$(CC) $(CFLAGS_MOS) $(INCLUDES) -o $@ src/iGame_cat.c

##########################################################################
# object files (AOS4)
##########################################################################

src/funcs_AOS4.o: src/funcs.c src/iGame_cat.h
	$(CC) $(CFLAGS_AOS4) -o $@ src/funcs.c

src/iGameGUI_AOS4.o: src/iGameGUI.c src/iGameGUI.h src/iGame_cat.h
	$(CC) $(CFLAGS_AOS4) -o $@ src/iGameGUI.c

src/iGameMain_AOS4.o: src/iGameMain.c
	$(CC) $(CFLAGS_AOS4) -o $@ src/iGameMain.c

src/strcasestr_AOS4.o: src/strcasestr.c
	$(CC) $(CFLAGS_AOS4) $(INCLUDES) -o $@ src/strcasestr.c

src/strdup_AOS4.o: src/strdup.c
	$(CC) $(CFLAGS_AOS4) -o $@ src/strdup.c

src/iGame_cat_AOS4.o: src/iGame_cat.c
	$(CC) $(CFLAGS_AOS4) $(INCLUDES) -o $@ src/iGame_cat.c

##########################################################################
# generic build options
##########################################################################

clean:
	del iGame iGame.* src\funcs*.o src\iGameGUI*.o src\iGameMain*.o src\strcasestr*.o src\strdup*.o src\iGame_cat*.o


