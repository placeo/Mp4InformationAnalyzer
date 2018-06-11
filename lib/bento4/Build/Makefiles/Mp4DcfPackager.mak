##########################################################################
#
#    Mp4DcfPackager Program
#
#    (c) 2002-2008 Axiomatic Systems, LLC
#
##########################################################################
all: mp4dcfpackager

##########################################################################
# includes
##########################################################################
include $(BUILD_ROOT)/Makefiles/Lib.exp

##########################################################################
# targets
##########################################################################
TARGET_SOURCES = Mp4DcfPackager.cpp

##########################################################################
# make path
##########################################################################
VPATH += $(SOURCE_ROOT)/Apps/Mp4DcfPackager

##########################################################################
# includes
##########################################################################
include $(BUILD_ROOT)/Makefiles/Rules.mak

##########################################################################
# rules
##########################################################################
mp4dcfpackager: $(TARGET_OBJECTS) $(TARGET_LIBRARY_FILES)
	$(LINK) $(TARGET_OBJECTS) -o $@ $(LINK_LIBRARIES)


