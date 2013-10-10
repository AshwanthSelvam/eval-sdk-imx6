#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
##$L$

PKG_TARBALL=libfslparser-3.0.8.tar.gz
PKG_SHA1SUM=ec4786fcb4777a8c4fd3f128eb8bc71e11d8fd62

include ../../bsp/classes/rrsdk.class
include $(CLASSES)/autotools.class

APPS_LDFLAGS+=-Wl,--rpath-link -Wl,$(FSDEVROOT)/usr/lib 

AUTOTOOLS_PARAMS=LDFLAGS="$(APPS_LDFLAGS)" CFLAGS="$(APPS_CFLAGS)"

LIBRARIES= \
		/usr/lib/imx-mm/parser/{lib_avi_parser_arm11_elinux.so.3.1,lib_avi_parser_arm9_elinux.so.3.1} \
		/usr/lib/imx-mm/parser/{lib_flv_parser_arm11_elinux.so.3.1,lib_flv_parser_arm9_elinux.so.3.1} \
		/usr/lib/imx-mm/parser/{lib_mkv_parser_arm11_elinux.so.3.1,lib_mkv_parser_arm9_elinux.so.3.1} \
		/usr/lib/imx-mm/parser/{lib_mp4_parser_arm11_elinux.so.3.1,lib_mp4_parser_arm9_elinux.so.3.1} \
		/usr/lib/imx-mm/parser/{lib_mpg2_parser_arm11_elinux.so.3.1,lib_mpg2_parser_arm9_elinux.so.3.1} \
		/usr/lib/imx-mm/parser/{lib_ogg_parser_arm11_elinux.so.3.1,lib_ogg_parser_arm9_elinux.so.3.1} \
		/usr/lib/{lib_mp3_parser_arm11_elinux.so.3.1,lib_mp3_parser_arm9_elinux.so.1}
		
LINKS=  \
	   /usr/lib/imx-mm/parser/{lib_avi_parser_arm11_elinux.so.3,lib_avi_parser_arm9_elinux.so,lib_avi_parser_arm9_elinux.so.3} \
	   /usr/lib/imx-mm/parser/{lib_flv_parser_arm11_elinux.so,lib_flv_parser_arm11_elinux.so.3,lib_flv_parser_arm9_elinux.so} \
	   /usr/lib/imx-mm/parser/{lib_flv_parser_arm9_elinux.so.3,lib_mkv_parser_arm11_elinux.so,lib_mkv_parser_arm11_elinux.so.3} \
	   /usr/lib/imx-mm/parser/{lib_mkv_parser_arm9_elinux.so,lib_mkv_parser_arm9_elinux.so.3,lib_mp4_parser_arm11_elinux.so} \
	   /usr/lib/imx-mm/parser/{lib_mp4_parser_arm11_elinux.so.3,lib_mp4_parser_arm9_elinux.so,lib_mp4_parser_arm9_elinux.so.3} \
	   /usr/lib/imx-mm/parser/{lib_mpg2_parser_arm11_elinux.so,lib_mpg2_parser_arm11_elinux.so.3,lib_mpg2_parser_arm9_elinux.so} \
	   /usr/lib/imx-mm/parser/{lib_mpg2_parser_arm9_elinux.so.3,lib_ogg_parser_arm11_elinux.so,lib_ogg_parser_arm11_elinux.so.3} \
	   /usr/lib/imx-mm/parser/{lib_ogg_parser_arm9_elinux.so,lib_ogg_parser_arm9_elinux.so.3} \
	   /usr/lib/{lib_mp3_parser_arm11_elinux.so,lib_mp3_parser_arm9_elinux.so} 

