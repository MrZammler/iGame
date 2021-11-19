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

include make_includes/obj_files.inc

##########################################################################
# Rule for building
##########################################################################

include make_includes/rules.inc

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

include make_includes/obj_000.inc

##########################################################################
# object files (030)
##########################################################################

include make_includes/obj_030.inc

##########################################################################
# object files (040)
##########################################################################

include make_includes/obj_040.inc

##########################################################################
# object files (060)
##########################################################################

include make_includes/obj_060.inc

##########################################################################
# object files (MOS)
##########################################################################

include make_includes/obj_mos.inc

##########################################################################
# object files (AOS4)
##########################################################################

include make_includes/obj_os4.inc

##########################################################################
# generic build options
##########################################################################

clean:
	rm iGame iGame.* src/funcs*.o src/iGameGUI*.o src/iGameMain*.o src/strfuncs*.o src/iGame_cat*.o $(catalog_files)

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
