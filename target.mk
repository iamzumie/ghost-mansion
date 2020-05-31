# Disable all the suffixes
.SUFFIXES:

Config ?= Debug

MAKETARGET = $(MAKE) \
	--no-print-directory \
	-C $@ \
	-f $(CURDIR)/Makefile \
	SRCDIR=$(CURDIR) \
	$(MAKECMDGOALS)

.PHONY: $(CONFIGURATIONS)
$(CONFIGURATIONS):
	+@[ -d $@ ] || mkdir -p $@
	+@$(MAKETARGET)

Makefile : ;
%.mk :: ;

% :: $(Config) ; :

.PHONY: clean
clean:
	rm -rf $(Config)
