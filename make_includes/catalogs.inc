##########################################################################
# catalog files
##########################################################################
src/iGame_strings.h: catalogs/iGame.cd catalogs/CatComp_h.sd
ifeq ($(shell uname), AmigaOS)
	catcomp catalogs/iGame.cd CFILE src/iGame_strings.h
else
	cd catalogs && \
		./cd2po.sh iGame.cd > po/iGame.pot && \
		flexcat iGame.cd ../src/iGame_strings.h=CatComp_h.sd
endif

catalog_files := $(patsubst catalogs/po/%.po,catalogs/%/iGame.catalog,$(wildcard catalogs/po/*.po))
catalog_dirs := $(dir $(catalog_files))

catalogs: $(catalog_files)
