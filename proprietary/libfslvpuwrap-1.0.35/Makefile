#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

PKG_TARBALL=libfslvpuwrap-1.0.35.tar.gz
PKG_SHA1SUM=22f9b846b4190881b7a67acb294de73c5f082a56

include ../../bsp/classes/rrsdk.class

APPS_LDFLAGS+=-Wl,--rpath-link -Wl,$(FSDEVROOT)/usr/lib 

AUTOTOOLS_PARAMS=LDFLAGS="$(APPS_LDFLAGS)" CFLAGS="$(APPS_CFLAGS)"


LIBRARIES= /usr/lib/libfslvpuwrap.so.3.0.0
LINKS= /usr/lib/{libfslvpuwrap.so,libfslvpuwrap.so.3}

include $(CLASSES)/autotools.class

