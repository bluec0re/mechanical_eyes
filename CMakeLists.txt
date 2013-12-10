cmake_minimum_required( VERSION 2.8 )

project( mechanical_eyes )

execute_process(COMMAND grep -o BCM2708 /proc/cpuinfo
    OUTPUT_VARIABLE SOC_NAME)

if( "${SOC_NAME}" STREQUAL "" )
    set( RASPBERRYPI false )
else()
    set( RASPBERRYPI true )
endif()

add_subdirectory( persontracker )
add_subdirectory( mechanical_eyes )
