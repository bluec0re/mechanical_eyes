#ifndef RASPICAM_YUVCONVERSION_
#define RASPICAM_YUVCONVERSION_ 
namespace raspicam {
    namespace _private{
    void yuv420_2_rgb888(unsigned char  *dst_ptr,
               const unsigned char  *y_ptr,
               const unsigned char  *u_ptr,
               const unsigned char  *v_ptr,
                     int   width,
                     int   height  );
    void yuv420_2_bgr888(unsigned char  *dst_ptr,
               const unsigned char  *y_ptr,
               const unsigned char  *u_ptr,
               const unsigned char  *v_ptr,
                     int   width,
                     int   height  );
  };
}
#endif

