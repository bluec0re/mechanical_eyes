#include "persontracker.h"
#include "config.h"
#include <iostream>

#ifdef RASPBERRYPI
#include <unistd.h>
#include <raspicam/raspicam_cv.h>

using namespace raspicam;
#endif

#define PT_SECTION "persontracker"

PersonTracker::PersonTracker() : camera(NULL)
{
#ifdef RASPBERRYPI
    camera = new RaspiCam_Cv();
#else
    camera = new cv::VideoCapture();
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

bool PersonTracker::open() {
    if(getCamera().isOpened())
        return false;
#ifdef RASPBERRYPI
    return dynamic_cast<RaspiCam_Cv*>(camera)->open();
#else
    return camera->open(0);
#endif
}

cv::VideoCapture& PersonTracker::getCamera() const {
    return *camera;
}

std::vector<cv::Rect> PersonTracker::getPersons() {
    std::vector<cv::Rect> persons;
    getPersons(persons);
    return persons;
}

bool PersonTracker::getPersons(std::vector<cv::Rect>& persons) {
    camera->read(img);

    if(!img.data)
        return false;

    std::vector<cv::Rect> found;
    hog.detectMultiScale(img, found, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 2);

    // make unique
    for(size_t i = 0; i < found.size(); i++) {
        cv::Rect& r = found[i];
        size_t j = 0;
        for(; j < found.size(); j++)
            if( j != i && (r & found[j]) == r)
                break;
        if( j == found.size() )
            persons.push_back( r );
    }

    return true;
}

std::vector<cv::Rect> PersonTracker::getFaces() {
    std::vector<cv::Rect> faces;
    getFaces(faces);
    return faces;
}

bool PersonTracker::getFaces(std::vector<cv::Rect>& faces) {
    camera->read(img);

    if(!img.data)
        return false;

    cvtColor(img, img, cv::COLOR_BGR2GRAY);
    equalizeHist(img, img);
// scaleFactor=1.3, minNeighbors=4, minSize=(30, 30), flags = cv2.CASCADE_SCALE_IMAGE
    haarCascade.detectMultiScale(img, faces,
            face_scalefactor,
            face_minneighbors,
            face_flags,
            face_minsize,
            face_maxsize);

    return true;
}

bool PersonTracker::loadSettings() {
    int width = getConfig().GetInteger(PT_SECTION, "width", 640);
    int height = getConfig().GetInteger(PT_SECTION, "height", 480);
    int brightness = getConfig().GetInteger(PT_SECTION, "brightness", 50);
    int contrast = getConfig().GetInteger(PT_SECTION, "contrast", 50);
    int saturation = getConfig().GetInteger(PT_SECTION, "saturation", 50);
    int gain = getConfig().GetInteger(PT_SECTION, "gain", 50);

    std::cout << "Resolution: " << width << "x" << height << std::endl
              << "Brightness: " << brightness << std::endl
              << "Contrast: " << contrast << std::endl
              << "Saturation: " << saturation << std::endl
              << "Gain: " << gain << std::endl;

    getCamera().set( CV_CAP_PROP_FRAME_WIDTH, width);
    getCamera().set( CV_CAP_PROP_FRAME_HEIGHT, height);
    getCamera().set( CV_CAP_PROP_BRIGHTNESS, brightness);
    getCamera().set( CV_CAP_PROP_CONTRAST, contrast);
    getCamera().set( CV_CAP_PROP_SATURATION, saturation);
    getCamera().set( CV_CAP_PROP_GAIN, gain);

    face_scalefactor = getConfig().GetReal(PT_SECTION "-face", "scalefactor", 1.3);
    face_minneighbors = getConfig().GetInteger(PT_SECTION "-face", "minneighbors", 4);
    face_flags = getConfig().GetInteger(PT_SECTION "-face", "flags", cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT);
    face_minsize = cv::Size(
                getConfig().GetInteger(PT_SECTION "-face", "minsize_x", 30),
                getConfig().GetInteger(PT_SECTION "-face", "minsize_y", 30)
            );
    face_maxsize = cv::Size(
                getConfig().GetInteger(PT_SECTION "-face", "maxsize_x", 0),
                getConfig().GetInteger(PT_SECTION "-face", "maxsize_y", 0)
            );

    std::cout << "Face Settings:\n"
              << " Scalefactor: " << face_scalefactor << std::endl
              << " Min neighbors: " << face_minneighbors << std::endl
              << " Flags: " << face_flags << std::endl
              << " Min Size: " << face_minsize.width << "x" << face_minsize.height << std::endl
              << " Max Size: " << face_maxsize.width << "x" << face_maxsize.height << std::endl;
/*
    // restart camera
    if(getCamera().isOpened()) {
        std::cout << "Restarting camera\n";
        getCamera().release();
#ifdef RASPBERRYPI
        // wait 1 sec to disable
        sleep(2);
        return dynamic_cast<RaspiCam_Cv&>(getCamera()).open();
#else
        return getCamera().open(0);
#endif
    }*/
    return true;
}
