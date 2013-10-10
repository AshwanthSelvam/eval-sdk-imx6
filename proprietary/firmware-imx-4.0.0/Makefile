#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

PKG_TARBALL=firmware-imx-3.0.35-4.0.0.tar.gz
PKG_SHA1SUM=a2b36086a5d3f869ee65f28468a111354ad6701e
FETCHER_EXTRACT_DIRECTORY_NAME=src

include ../../bsp/classes/rrsdk.class
include ../../bsp/classes/fetcher.defs


build:rrfetched
	@touch rrbuilt

clean::
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

install::
	@mkdir -p $(FSROOT)/lib/firmware/vpu
	@cp src/firmware/vpu/vpu_fw_imx6q.bin $(FSROOT)/lib/firmware/vpu


#include $(CLASSES)/gstreamer-plugin.class

