cmake_minimum_required( VERSION 2.8 )

project( persontracker )

find_package( OpenCV REQUIRED )

set( standalone_SRC  "${persontracker_SOURCE_DIR}/standalone/main.cpp" )

file( GLOB persontracker_SRC 
      "${persontracker_SOURCE_DIR}/src/*.cpp" 
      "${persontracker_SOURCE_DIR}/src/*.h" 
      "${persontracker_SOURCE_DIR}/src/*.c"
    )


# raspicam
#set( MMAL_LIBS 
#     /opt/vc/lib/libmmal_core.so
#     /opt/vc/lib/libmmal_util.so
#     /opt/vc/lib/libmmal_vc_client.so
#    )
#set( VCOS_LIBS /opt/vc/lib/libvcos.so )
#set( BCM_HOST_LIBS /opt/vc/lib/libbcm_host.so )


# general
include_directories( /root/raspi_userland/interface/vcos )
include_directories( /root/raspi_userland )
include_directories( /root/raspi_userland/interface/vcos/pthreads )
include_directories( /root/raspi_userland/interface/vmcs_host/linux )
include_directories( /root/raspi_userland/interface/vmcs_host/ )
include_directories( /root/raspi_userland/interface/vmcs_host/khronos )
include_directories( /root/raspi_userland/interface/khronos/include )


include_directories( /root/raspi_userland/host_applications/linux/libs/bcm_host/include )
include_directories(${CMAKE_SOURCE_DIR}/raspicam)

add_subdirectory( raspicam )

link_directories( ${persontracker_BINARY_DIR}/raspicam )
link_directories( /opt/vc/lib )

# end raspicam

include_directories( "${persontracker_SOURCE_DIR}/src" )

add_library( persontracker SHARED ${persontracker_SRC} )
target_link_libraries( persontracker ${OpenCV_LIBS} raspistill )

add_executable( tracker_standalone ${standalone_SRC} )
target_link_libraries( tracker_standalone persontracker ${OpenCV_LIBS} )
