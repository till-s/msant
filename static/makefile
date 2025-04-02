FILES = qmom
FILES += d1mach

LIBS= lib$(LIBNAME)
FILES += drecur
FILES += dgauss
FILES += dpsi dcot

LIBNAME = qmomdrv

all:
	intersci-n $(LIBNAME) '$(FILES)' '$(LIBS)'
	scilab -nw -e 'exec("$(LIBNAME)_builder.sce");quit' -args -nouserstartup
	sed -e 's/loader.sce/$(LIBNAME).sce/g' < loader.sce > $(LIBNAME).sce

clean:
	$(RM) $(FILES:%=%.o) $(FILES:%=%.lo) $(LIBNAME).o $(LIBNAME).lo
	$(RM) $(filter-out lib$(LIBNAME).so,$(wildcard lib$(LIBNAME).*))
	$(RM) Makelib
	$(RM) $(LIBNAME)_builder.sce
	$(RM) loader.sce
	$(RM) $(LIBNAME).tmp
	$(RM) -r .libs

distclean: clean
	$(RM) $(LIBNAME).sce
	$(RM) lib$(LIBNAME).so
	@if [ -f $(LIBNAME).c ] ; then echo "$(LIBNAME).c is normally generated automatically and it should be ok to remove it"; $(RM) -i $(LIBNAME).c; fi
	$(RM) test*.out.local
