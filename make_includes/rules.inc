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
