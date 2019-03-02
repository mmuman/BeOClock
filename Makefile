SUBDIRS := src

default .DEFAULT:
	@for f in $(SUBDIRS) ; do \
		$(MAKE) -C $$f $@ ; \
	done
