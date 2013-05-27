#$L$
# Copyright (C) 2013 Ridgerun (http://www.ridgerun.com)
##$L$

PKG_URL=http://ftp.gnome.org/pub/gnome/sources/glib/2.34/
PKG_TARBALL=glib-2.34.3.tar.xz
PKG_SHA1SUM=f2b94ca757191dddba686e54b32b3dfc5ad5d8fb

include ../../../bsp/classes/rrsdk.defs

# The gvariant on ARM has a bug with double floating values if the compiler has optimizations
CFLAGS:=$(patsubst -O%,,$(APPS_CFLAGS))

# Prevent autogen.sh from running configure script
export NOCONFIGURE=yes

APPS_LDFLAGS+=-Wl,--rpath-link -Wl,$(FSDEVROOT)/usr/lib

AUTOTOOLS_PARAMS= glib_cv_stack_grows=no --with-gio-module-dir=$(DEVDIR)/bsp/local/lib/gio/modules \
glib_cv_uscore=no ac_cv_func_posix_getpwuid_r=yes GLIB_GENMARSHAL=$(LOCALDIR)/bin/glib-genmarshal \
ac_cv_func_posix_getgrgid_r=yes --disable-rpath --enable-xattr \
--disable-fam --with-threads=posix CFLAGS="$(CFLAGS)" CPPFLAGS="$(CFLAGS)" LDFLAGS="$(APPS_LDFLAGS)"

BINARIES = /usr/bin/{gsettings,gdbus}

LIBRARIES=  /usr/lib/libgio-2.0.so.0.3400.3 \
	/usr/lib/libglib-2.0.so.0.3400.3 \
	/usr/lib/libgmodule-2.0.so.0.3400.3 \
	/usr/lib/libgobject-2.0.so.0.3400.3 \
	/usr/lib/libgthread-2.0.so.0.3400.3

include $(CLASSES)/autotools.class

