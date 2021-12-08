##########################################################################
# catalog files
##########################################################################
src/iGame_strings.h: catalogs/iGame.cd catalogs/CatComp_h.sd
	cd catalogs && flexcat iGame.cd ../src/iGame_strings.h=CatComp_h.sd

catalog_files := $(patsubst %/iGame.ct,%/iGame.catalog,$(wildcard catalogs/*/iGame.ct))
catalog_dirs := $(dir $(catalog_files))

catalogs: $(catalog_files)
