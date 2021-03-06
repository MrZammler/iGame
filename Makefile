##########################################################################
# Makefile for iGame on Linux using GCC.
#-------------------------------------------------------------------------
# To compile an iGame flat executable  using this makefile, run:
#  make
#-------------------------------------------------------------------------
##########################################################################

##########################################################################
# Default: Build iGame with standard optimizations and 000 support
##########################################################################
all: iGame

##########################################################################
# Set up version and date properties
##########################################################################

DATE = $(shell date --iso=date)

##########################################################################
# Compiler settings
##########################################################################
CC			= m68k-amigaos-gcc
LINK		= m68k-amigaos-gcc
CC_PPC		= ppc-amigaos-gcc
LINK_PPC	= ppc-amigaos-gcc

INCLUDES	= -I$(NDK_INC) -I$(MUI38_INC)
INCLUDES_OS4= -I$(SDK_INC) -I$(MUI50_INC)
INCLUDES_MOS= -I$(NDK_INC) -I$(MUI50_INC)

CFLAGS		= -c -Os -fomit-frame-pointer -std=c99 -DCPU_VERS=68000 -DRELEASE_DATE=$(DATE)
CFLAGS_030	= -c -mcpu=68030 -Os -fomit-frame-pointer -std=c99 -DCPU_VERS=68030 -DRELEASE_DATE=$(DATE)
CFLAGS_040	= -c -mcpu=68040 -Os -fomit-frame-pointer -std=c99 -DCPU_VERS=68040 -DRELEASE_DATE=$(DATE)
CFLAGS_060	= -c -mcpu=68060 -Os -fomit-frame-pointer -std=c99 -DCPU_VERS=68060 -DRELEASE_DATE=$(DATE)
CFLAGS_MOS	= -c -Os -fomit-frame-pointer -std=c99 -DCPU_VERS=MorphOS -DRELEASE_DATE=$(DATE)
CFLAGS_OS4	= -c -Os -fomit-frame-pointer -std=c99 -D__USE_INLINE__ -DCPU_VERS=AmigaOS4 -DRELEASE_DATE=$(DATE)

##########################################################################
# Builder settings
##########################################################################
#MKLIB			= join
LIBFLAGS		= -v -lamiga -lstubs -o
LIBFLAGS_MOS	= -v -lamiga -lstubs -o
LIBFLAGS_OS4	= -v -lamiga -lstubs -o

##########################################################################
# Object files which are part of iGame
##########################################################################
OBJS		= src/funcs.o src/iGameGUI.o src/iGameMain.o src/strcasestr.o src/strdup.o src/iGame_cat.o
OBJS_030	= src/funcs_030.o src/iGameGUI_030.o src/iGameMain_030.o src/strcasestr_030.o src/strdup_030.o src/iGame_cat_030.o
OBJS_040	= src/funcs_040.o src/iGameGUI_040.o src/iGameMain_040.o src/strcasestr_040.o src/strdup_040.o src/iGame_cat_040.o
OBJS_060	= src/funcs_060.o src/iGameGUI_060.o src/iGameMain_060.o src/strcasestr_060.o src/strdup_060.o src/iGame_cat_060.o
OBJS_MOS	= src/funcs_MOS.o src/iGameGUI_MOS.o src/iGameMain_MOS.o src/strcasestr_MOS.o src/strdup_MOS.o src/iGame_cat_MOS.o
OBJS_OS4	= src/funcs_OS4.o src/iGameGUI_OS4.o src/iGameMain_OS4.o src/strcasestr_OS4.o src/strdup_OS4.o src/iGame_cat_OS4.o

##########################################################################
# Rule for building
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
	$(LINK_PPC) $(OBJS_MOS) $(LIBFLAGS_MOS) $@

iGame.OS4: $(OBJS_OS4)
	$(LINK_PPC) $(OBJS_OS4) $(LIBFLAGS_OS4) $@

##########################################################################
# catalog files
##########################################################################
src/iGame_cat.c: catalogs/iGame.cd catalogs/C_c.sd
	cd catalogs && flexcat iGame.cd ../src/iGame_cat.c=C_c.sd

src/iGame_cat.h: catalogs/iGame.cd catalogs/C_h.sd
	cd catalogs && flexcat iGame.cd ../src/iGame_cat.h=C_h.sd

catalogs/%/iGame.catalog: catalogs/%/iGame.ct catalogs/iGame.cd
	flexcat catalogs/iGame.cd $< CATALOG $@

catalog_files := $(patsubst %/iGame.ct,%/iGame.catalog,$(wildcard catalogs/*/iGame.ct))
catalog_dirs := $(dir $(catalog_files))

catalogs: $(catalog_files)

##########################################################################
# object files (generic 000)
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
	$(CC_PPC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/funcs.c

src/iGameGUI_MOS.o: src/iGameGUI.c src/iGameGUI.h src/iGame_cat.h
	$(CC_PPC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/iGameGUI.c

src/iGameMain_MOS.o: src/iGameMain.c
	$(CC_PPC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/iGameMain.c

src/strcasestr_MOS.o: src/strcasestr.c
	$(CC_PPC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/strcasestr.c

src/strdup_MOS.o: src/strdup.c
	$(CC_PPC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/strdup.c

src/iGame_cat_MOS.o: src/iGame_cat.c
	$(CC_PPC) $(CFLAGS_MOS) $(INCLUDES_MOS) -o $@ src/iGame_cat.c

##########################################################################
# object files (AOS4)
##########################################################################

src/funcs_OS4.o: src/funcs.c src/iGame_cat.h
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/funcs.c

src/iGameGUI_OS4.o: src/iGameGUI.c src/iGameGUI.h src/iGame_cat.h
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/iGameGUI.c

src/iGameMain_OS4.o: src/iGameMain.c
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/iGameMain.c

src/strcasestr_OS4.o: src/strcasestr.c
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/strcasestr.c

src/strdup_OS4.o: src/strdup.c
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/strdup.c

src/iGame_cat_OS4.o: src/iGame_cat.c
	$(CC_PPC) $(CFLAGS_OS4) $(INCLUDES_OS4) -o $@ src/iGame_cat.c

##########################################################################
# generic build options
##########################################################################

clean:
	rm iGame iGame.* src/funcs*.o src/iGameGUI*.o src/iGameMain*.o src/strcasestr*.o src/strdup*.o src/iGame_cat*.o $(catalog_files)

# pack everything in a nice lha file
release: $(catalog_files)
	cp required_files iGame_rel/iGame-$(DATE) -r
	cp alt_icons iGame_rel/iGame-$(DATE)/Icons -r
	cp iGame_rel/iGame-$(DATE)/igame_drawer_3.0.info iGame_rel/iGame-$(DATE).info
	mv iGame_rel/iGame-$(DATE)/igame_drawer_3.0.info iGame_rel/iGame-$(DATE)/Icons/
	mv iGame_rel/iGame-$(DATE)/igame_drawer.info iGame_rel/iGame-$(DATE)/Icons/
	if [ -f "iGame" ]; then cp iGame iGame_rel/iGame-$(DATE)/; fi
	if [ -f "iGame.030" ]; then cp iGame.030 iGame_rel/iGame-$(DATE)/; fi
	if [ -f "iGame.040" ]; then cp iGame.040 iGame_rel/iGame-$(DATE)/; fi
	if [ -f "iGame.060" ]; then cp iGame.060 iGame_rel/iGame-$(DATE)/; fi
	if [ -f "iGame.MOS" ]; then cp iGame.MOS iGame_rel/iGame-$(DATE)/; fi
	if [ -f "iGame.OS4" ]; then cp iGame.OS4 iGame_rel/iGame-$(DATE)/; fi
	mkdir iGame_rel/iGame-$(DATE)/catalogs
	cp catalogs/iGame.cd iGame_rel/iGame-$(DATE)/catalogs/
	cd iGame_rel/iGame-$(DATE) && mkdir $(catalog_dirs)
	for c in $(catalog_files); do cp $$c iGame_rel/iGame-$(DATE)/$$(dirname $$c)/; done
	cp CHANGELOG.md iGame_rel/iGame=$(DATE)/
	cd iGame_rel && lha -aq2o6 iGame-$(DATE).lha iGame-$(DATE)/ iGame-$(DATE).info

clean-release:
	rm -rf iGame_rel/iGame-$(DATE)
	rm iGame_rel/iGame-$(DATE).lha
	rm iGame_rel/iGame-$(DATE).info
