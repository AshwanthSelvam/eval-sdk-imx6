#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

#PKG_URL=http://gstreamer.freedesktop.org/src/gst-plugins-good/
PKG_TARBALL=libfslvpuwrap-1.0.28.tar.gz
PKG_SHA1SUM=32cc080e594ab82607d27694f1d032e9089cf639

include ../../bsp/classes/rrsdk.class

APPS_LDFLAGS+=-Wl,--rpath-link -Wl,$(FSDEVROOT)/usr/lib 

AUTOTOOLS_PARAMS=LDFLAGS="$(APPS_LDFLAGS)" CFLAGS="$(APPS_CFLAGS)"


LIBRARIES= /usr/lib/libfslvpuwrap.so.3.0.0
LINKS= /usr/lib/{libfslvpuwrap.so,libfslvpuwrap.so.3}

#FETCHER_NO_DOWNLOAD=yes

#APPS_CFLAGS+=-I$(DEVDIR)/kernel/$(KERNEL)/include/

#ifeq ($(CONFIG_ARCH_DAVINCI_DM365),y)
#    APPS_CFLAGS+=-DPlatform_dm365
#endif

#ifeq ($(CONFIG_ARCH_DAVINCI),y)
#    APPS_CFLAGS+=-DDAVINCI_LSP_WORKAROUND
#endif

#ifeq ($(CONFIG_FS_APPS_GSTREAMER_PLUGINS_GOOD_V4L2),y)
#    APPS_CFLAGS+=-I$(DEVDIR)/kernel/$(KERNEL)/include/
#    AUTOTOOLS_PARAMS+=--enable-experimental --enable-gst_v4l2 
#endif
#ifeq ($(CONFIG_FS_APPS_GSTREAMER_PLUGINS_GOOD_FLAC),y)
#    AUTOTOOLS_PARAMS+=--enable-flac
#else
#    AUTOTOOLS_PARAMS+=--disable-flac
#endif

#include $(CLASSES)/gstreamer-plugin.class
include $(CLASSES)/autotools.class

