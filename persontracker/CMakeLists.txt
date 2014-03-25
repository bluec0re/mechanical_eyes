project( persontracker )

find_package( OpenCV REQUIRED )

if( RASPBERRYPI )
find_package( raspicam REQUIRED )
add_definitions( -DRASPBERRYPI=1 )
endif()

set( standalone_SRC  "${persontracker_SOURCE_DIR}/standalone/main.cpp" )

file( GLOB persontracker_SRC 
      "${persontracker_SOURCE_DIR}/src/*.cpp" 
      "${persontracker_SOURCE_DIR}/src/*.h" 
      "${persontracker_SOURCE_DIR}/src/*.c"
    )


include_directories( "${persontracker_SOURCE_DIR}/src" )

add_library( persontracker SHARED ${persontracker_SRC} )

if( RASPBERRYPI )
    target_link_libraries( persontracker ${OpenCV_LIBS} ${raspicam_CV_LIBS} )
else()
    target_link_libraries( persontracker ${OpenCV_LIBS} )
endif()

add_executable( tracker_standalone ${standalone_SRC} )
target_link_libraries( tracker_standalone persontracker ${OpenCV_LIBS} )

install( TARGETS persontracker DESTINATION "lib" )
