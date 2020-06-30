export COMPONENT = libamxc

-include $(STAGINGDIR)/sahcommon.mk

VERSION ?= $(subst v,,$(strip $(if $(shell git describe --tags 2> /dev/null),$(shell git describe --tags),v0.0.0)))

# the processor architecture 
MACHINE = $(shell $(CC) -dumpmachine)

export INSTALL ?= install
export STRIP ?= strip

all:
	$(MAKE)

install:
	$(call install_so,output/$(MACHINE)/$(COMPONENT)-$(VERSION).so,/lib)

release:
	$(call install_so_release,$(COMPONENT).so,/lib)

clean:
	$(MAKE) clean

doc:
	$(INSTALL) -d $(STAGINGDIR)/doc/api/$(COMPONENT)
	-doxygen $(COMPONENT).doxy

build_package:
	make -f Component.mk release ROOT_FS_DIR=$(COMP_PACKAGE_DIR)

release_package:
	cp -af $(COMP_PACKAGE_DIR)/* $(ROOT_FS_DIR)

.PHONY: all clean install release doc build_package release_package
