##########################################################################
# catalog files
##########################################################################
src/iGame_strings.h: catalogs/iGame.cd catalogs/CatComp_h.sd
ifeq ($(shell uname), AmigaOS)
	catcomp catalogs/iGame.cd CFILE src/iGame_strings.h
else
	cd catalogs && flexcat iGame.cd ../src/iGame_strings.h=CatComp_h.sd
endif

catalog_files := $(patsubst %/iGame.ct,%/iGame.catalog,$(wildcard catalogs/*/iGame.ct))
catalog_dirs := $(dir $(catalog_files))

catalogs: $(catalog_files)

