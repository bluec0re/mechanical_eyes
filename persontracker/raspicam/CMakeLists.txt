
# raspistill/raspivid/raspiyuv

SET(COMPILE_DEFINITIONS -Werror)


# general
#include_directories( /root/raspi_userland/interface/vcos )
#include_directories( /root/raspi_userland )
#include_directories( /root/raspi_userland/interface/vcos/pthreads )
#include_directories( /root/raspi_userland/interface/vmcs_host/linux )
#include_directories( /root/raspi_userland/interface/vmcs_host/ )
#include_directories( /root/raspi_userland/interface/vmcs_host/khronos )
#include_directories( /root/raspi_userland/interface/khronos/include )
#
#
#include_directories( /root/raspi_userland/host_applications/linux/libs/bcm_host/include )
#include_directories(${CMAKE_SOURCE_DIR}/raspicam)

set (GL_SCENE_SOURCES
   gl_scenes/models.c
   gl_scenes/mirror.c
   gl_scenes/yuv.c
   gl_scenes/sobel.c
   gl_scenes/square.c
   gl_scenes/teapot.c)

set (COMMON_SOURCES
   RaspiCamControl.c
   RaspiCLI.c
   RaspiPreview.c)

add_library(raspistill ${COMMON_SOURCES} RaspiStill.c  RaspiTex.c RaspiTexUtil.c tga.c ${GL_SCENE_SOURCES})
#add_executable(raspiyuv   ${COMMON_SOURCES} RaspiStillYUV.c)
#add_executable(raspivid   ${COMMON_SOURCES} RaspiVid.c)

set (MMAL_LIBS mmal_core mmal_util mmal_vc_client)

target_link_libraries(raspistill ${MMAL_LIBS} vcos bcm_host GLESv2 EGL m)
#target_link_libraries(raspiyuv   ${MMAL_LIBS} vcos bcm_host)
#target_link_libraries(raspivid   ${MMAL_LIBS} vcos bcm_host)

#install(TARGETS raspistill raspiyuv raspivid RUNTIME DESTINATION bin)
install(TARGETS raspistill ARCHIVE DESTINATION lib)
