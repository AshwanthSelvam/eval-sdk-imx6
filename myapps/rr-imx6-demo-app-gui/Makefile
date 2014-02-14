#$L$
# Copyright (C) 2011 Ridgerun (http://www.ridgerun.com). 
#$L$

include ../../bsp/classes/rrsdk.class
include $(CLASSES)/qt.class

# We don't have to fetch the source code, is local
FETCHER_NO_DOWNLOAD=yes
PKG_TARBALL=imx6_demo_gui_videos.tar.gz
PKG_SHA1SUM=fbf18031debb9a729af940176ec570a621b454f1
FETCHER_EXTRACT_DIRECTORY_NAME=$(DEVDIR)/myapps/rr-imx6-demo-app-gui/src/media

QMAKE_OPTIONS+=QMAKE_CXXFLAGS+="-I$(KERNEL_INCLUDE)"

# Define the number of cameras to be used
ifeq ($(CONFIG_MACH_MX6Q_SABRESD),y)
QMAKE_OPTIONS+=QMAKE_CXXFLAGS+="-DENABLE_2CAMERAS" 
endif

#Enable/Disable Motion detection plugin
ifeq ($(CONFIG_PROPRIETARY_RR_MOTION_DETECT_PLUGIN),y)
QMAKE_OPTIONS+=QMAKE_CXXFLAGS+="-DENABLE_MOTION_DETECTION" 
endif

BINARIES=/usr/bin/RR-iMX6-Demo

OTHERS=/usr/share/rr-imx6-demo.config

build:
	$(V) cp ./src/rr-imx6-demo.config $(FSDEVROOT)/usr/share/

install:
	$(V) mkdir -p $(FSROOT)/opt/RR-Demo
	$(V) cp -r ./src/media $(FSROOT)/opt/RR-Demo


INIT_SCRIPT=imx6_demo_tscalibrate.init
INIT_SCRIPT_LEVEL=90
