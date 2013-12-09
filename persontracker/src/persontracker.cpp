#include "persontracker.h"

#ifdef RASPBERRYPI
#include <raspicam/raspicam_cv.h>

using namespace raspicam;
#endif

PersonTracker::PersonTracker() : camera(NULL)
{
#ifdef RASPBERRYPI
    camera = new RaspiCam_Cv();
    dynamic_cast<RaspiCam_Cv*>(camera)->open();
#else
    camera = new cv::VideoCapture();
    camera->open(0);
#endif
}

PersonTracker::PersonTracker(int device) : camera(NULL)
{
    camera = new cv::VideoCapture();
    camera->open(device);
}

PersonTracker::PersonTracker(const std::string& filename) : camera(NULL)
{
    camera = new cv::VideoCapture();
    camera->open(filename);
}

PersonTracker::~PersonTracker()
{
    if(camera) {
        camera->release();
        delete camera;
    }
}

cv::VideoCapture& PersonTracker::getCamera() const {
    return *camera;
}
