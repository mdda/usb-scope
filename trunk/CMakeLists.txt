cmake_minimum_required(VERSION 2.6.2)

PROJECT(USBprogrammer)

FIND_PACKAGE(wxWidgets COMPONENTS base core REQUIRED)

include_directories(${wxWidgets_INCLUDE_DIRS})
add_definitions(${wxWidgets_DEFINITIONS})

IF(wxWidgets_FOUND)
  INCLUDE(${wxWidgets_USE_FILE} )
ELSE(wxWidgets_FOUND)
  # For convenience. When we cannot continue, inform the user
  MESSAGE(FATAL_ERROR "wxWidgets not found!")
ENDIF(wxWidgets_FOUND)




# This is to set up the libusb variables 
# From : http://gitorious.org/android-obex/openobex/blobs/raw/master/CMakeModules/FindLibUSB.cmake
INCLUDE( build/FindLibUSB.cmake )



# For convenience, define the sources as a variable.
set(CONFIG_SRCS src/USBconfig.cpp src/USBconfig.h src/usb_utils.cpp src/usb_utils.h src/usbprogrammer.h )
set(SCOPE_SRCS src/SLOscope.cpp src/myLongScope.cpp src/myDetailScope.cpp src/myReadScope.cpp src/SLOscope.h src/usb_utils.cpp src/usb_utils.h src/usbprogrammer.h )

# If we build for windows systems, we also include the resource file
# containing the manifest, icon and other resources
if(WIN32)
  set(CONFIG_SRCS ${CONFIG_SRCS} src/USBconfig.rc)
  set(SCOPE_SRCS ${CONFIG_SRCS} src/SLOscope.rc)
endif(WIN32)
 

# Here we define the executable minimal.exe or minimal on other systems
# the above paths and defines will be used in this build

ADD_EXECUTABLE(usb-programmer-config WIN32 ${CONFIG_SRCS})
ADD_EXECUTABLE(SLOscope WIN32 ${SCOPE_SRCS})

# We add to our target the wxWidgets libraries. These are
# set for us by the find script. If you need other libraries, you
# can add them here as well. 
TARGET_LINK_LIBRARIES(usb-programmer-config ${wxWidgets_LIBRARIES} ${LibUSB_LIBRARIES} )
TARGET_LINK_LIBRARIES(SLOscope ${wxWidgets_LIBRARIES} ${LibUSB_LIBRARIES} )
