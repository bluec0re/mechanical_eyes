#project( mechanical_eyes )

file( GLOB mechanical_eyes_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    )


include_directories( "${persontracker_SOURCE_DIR}/src" )

add_executable( mechanical_eyes ${mechanical_eyes_SRC} )
target_link_libraries( mechanical_eyes persontracker )

install( TARGETS mechanical_eyes DESTINATION "bin" )
