#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

#PKG_URL=http://gstreamer.freedesktop.org/src/gst-plugins-good/
#PKG_TARBALL=firmware-imx-1.1.0.tar.gz
#PKG_SHA1SUM=a18f0f5e8be54a965689be117448eeab631fa7dc

include ../../bsp/classes/rrsdk.class

#FETCHER_NO_DOWNLOAD=yes

build:
	@touch rrbuilt

clean:

install:
	@mkdir -p $(FSROOT)/lib/firmware/vpu
	@cp src/firmware/vpu/vpu_fw_imx6q.bin $(FSROOT)/lib/firmware/vpu


#include $(CLASSES)/gstreamer-plugin.class

