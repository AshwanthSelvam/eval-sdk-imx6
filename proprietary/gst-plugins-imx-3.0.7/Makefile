#$L$
# Copyright (C) 2013 Ridgerun (http://www.ridgerun.com). 
##$L$

PKG_TARBALL=gst-fsl-plugins-3.0.7.tar.gz
PKG_SHA1SUM=2930850c6ff22035db3dd12a0688410e18dfed97

include ../../bsp/classes/rrsdk.class

APPS_LDFLAGS+=-Wl,-rpath-link -Wl,$(FSDEVROOT)/usr/lib 
APPS_LDFLAGS+=-Wl,-rpath-link -Wl,$(FSDEVROOT)/usr/lib/imx-mm/audio-codec/
APPS_LDFLAGS+=-Wl,-rpath-link -Wl,$(FSDEVROOT)/usr/lib/imx-mm/video-codec/ 
APPS_LDFLAGS+=-Wl,-rpath-link -Wl,$(FSDEVROOT)/usr/lib/imx-mm/audio-codec/wrap/

APPS_LDFLAGS+=-Wl,-rpath=/usr/lib/imx-mm/audio-codec/
APPS_LDFLAGS+=-Wl,-rpath=/usr/lib/imx-mm/audio-codec/wrap/
APPS_LDFLAGS+=-Wl,-rpath=/usr/lib/imx-mm/video-codec/
APPS_LDFLAGS+=-Wl,-rpath=/usr/lib/imx-mm/parser/

#Header files search path needs to be corrected in order to compile
#the plugins in the arm directory
APPS_CFLAGS+=-I$(GST_PLUGINS_IMX)/src
APPS_CFLAGS+=-I$(GST_PLUGINS_IMX)/src/libs
APPS_CFLAGS+=-I$(GST_PLUGINS_IMX)/src/inc/{common,plugin}
APPS_CFLAGS+=-I$(GST_PLUGINS_IMX)/src/libs/{gstbufmeta,gstnext,gstsutils,me,sconf,vss}
APPS_CFLAGS+=-I$(KERNEL_INCLUDE)

AUTOTOOLS_PARAMS=LDFLAGS="$(APPS_LDFLAGS)" CFLAGS="$(APPS_CFLAGS) -lrt" PLATFORM=MX6 --disable-x11


BINARIES= /usr/bin/gplay

LIBRARIES= /usr/lib/{libgplaycore.so.0.0.1,libgstfsl-0.10.so.0.0.1}
LINKS= /usr/lib/{libgplaycore.so,libgplaycore.so.0,libgstfsl-0.10.so,libgstfsl-0.10.so.0}

OTHERS= /usr/share/{beep_registry.arm11.cf,beep_registry.arm12.cf,aiur_registry.arm11.cf}

build::
	$(V) $(MAKE) coprocessors.init

#============================================================================================
# coprocessor.init creates the init script that will be used to load all the
# necessary modules into the target board
#============================================================================================

.PHONY: coprocessors.init coprocessors.init.in
coprocessors.init: coprocessors.init.in
	$(V)sed -e 's?@KERNELVERSION@?$(KERNELVERSION)?g' \
	    $< > $@


#clean::
#	$(V)rm -Rf configured* autotools_extra_flags* sim
#	$(V)rm -rf $(AUTOTOOLS_BUILD_DIR)
#	$(V)rm -Rf $(BUILT_FLAG)* $(INSTALLED_FLAG)* $(FETCHED_FLAG)

include $(CLASSES)/gstreamer-plugin.class

INIT_SCRIPT=coprocessors.init
INIT_SCRIPT_LEVEL=35
