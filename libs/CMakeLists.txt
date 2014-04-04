set( SERVOD_SRC "PiBits/ServoBlaster/user/servod.c" )
set( SERVOD_SRC_PATCHED "PiBits/ServoBlaster/user/servod.patched.c" )
add_executable( servod "${SERVOD_SRC_PATCHED}" )
target_link_libraries( servod m )

add_custom_command( OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${SERVOD_SRC_PATCHED}"
                    COMMAND patch -s -N -r - -p0 -o "${CMAKE_CURRENT_SOURCE_DIR}/${SERVOD_SRC_PATCHED}" "${CMAKE_CURRENT_SOURCE_DIR}/${SERVOD_SRC}" "${CMAKE_CURRENT_SOURCE_DIR}/servod.patch"
                    )

install( TARGETS servod DESTINATION "bin" )
install( FILES "${CMAKE_CURRENT_SOURCE_DIR}/servod.service" DESTINATION "/usr/lib/systemd/system" )
