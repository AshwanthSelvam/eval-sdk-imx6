#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

PKG_URL=http://sourceforge.net/projects/libpng/files/zlib/1.2.5/
PKG_TARBALL=zlib-1.2.5.tar.gz
PKG_SHA1SUM=8e8b93fa5eb80df1afe5422309dca42964562d7e

LIBRARIES=/usr/lib/libz.so.1.2.5

CFLAGS += -fPIC
# We include this definitions here instead of the bottom of the Makefile
# because we need to get the $(ARCH) variable definition
include ../../../bsp/classes/rrsdk.class

FETCHER_EXTRACT_DIRECTORY_NAME=src
build: rrfetched built

built: configured
	$(V) $(MAKE) -j $(BSP_NCPU) -C $(ARCH) $(QOUT)

configured:
	$(V) $(RM) -rf $(ARCH) $(QOUT)
	$(V) cp -r src $(ARCH) $(QOUT)
	$(V) cd $(ARCH) && \
	$(RM) -rf config.cache && \
	export $(EXPORT_TOOLCHAIN_ENVIRONMENT) && \
	./configure \
	--shared \
	--prefix=$(FSDEVROOT)/usr $(QOUT)
	$(V) touch $@ 

installed:
	$(V)$(MAKE) -C $(ARCH) install $(QOUT)
	$(V) touch $@ $(QOUT)

install: installed rrsdk_install

uninstall:
	$(V)$(MAKE) -C $(ARCH) uninstall $(QOUT)
	$(V) touch $@

clean:
	$(V) rm -Rf installed built configured $(ARCH)

distclean: clean rrfetched_clean

include $(CLASSES)/fetcher.defs
