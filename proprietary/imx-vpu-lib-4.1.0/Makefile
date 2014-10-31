	#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com)
##$L$

include ../../bsp/classes/rrsdk.class
include ../../bsp/classes/fetcher.defs

PKG_TARBALL=imx-vpu-lib-3.0.35-4.1.0.tar.gz
PKG_SHA1SUM=bf6e2510cb0c79393bfc240da09f8331c762d8dd
FETCHER_EXTRACT_DIRECTORY_NAME=src

LIBRARIES=

# Build libraries
INCLUDE="-I$(FSDEVROOT)/usr/include \
-I$(KERNELDIR)/drivers/mxc/security/rng/include \
-I$(KERNELDIR)/drivers/mxc/security/sahara2/include \
-I$(KERNEL_INCLUDE)"

PLATFORM=IMX6Q

DEST_DIR=$FSDEVROOT

LIBRARIES=/usr/lib/{libipu.so.0,libpxp.so.0,libfsl_shw.so.0,libvpu.so.4}
LINKS=/usr/lib/{libipu.so,libpxp.so,libfsl_shw.so,libvpu.so}

build:rrfetched
	 $(V)$(MAKE) -C src PLATFORM=$(PLATFORM) INCLUDE=$(INCLUDE)
	 @touch rrbuilt
	 
pre_install:
	 $(V)$(MAKE) -C src install PLATFORM=$(PLATFORM) INCLUDE=$(INCLUDE) DEST_DIR=$(FSDEVROOT)

install: pre_install rrsdk_install
	@touch rrinstalled

clean::
	$(V)if [ -e src ] ; then \
		$(MAKE) -C src clean; \
	fi;
	$(V)if [ -f rrbuilt ] ; then \
		rm rrbuilt; \
	fi;
	$(V)if [ -f rrinstalled ] ; then \
		rm rrinstalled; \
	fi;

distclean: clean
	$(V)if [ -f rrfetched ] ; then \
		rm rrfetched; \
	fi;
	$(V)if [ -e src ] ; then \
		rm -r src; \
	fi;


#include $(CLASSES)/gstreamer-plugin.class

