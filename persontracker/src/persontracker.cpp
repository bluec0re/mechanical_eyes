#include "persontracker.h"
#include "config.h"

#ifdef RASPBERRYPI
#include <raspicam/raspicam_cv.h>

using namespace raspicam;
#endif

#define PT_SECTION "persontracker"

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
            getConfig().GetReal(PT_SECTION "-face", "scalefactor", 1.3),
            getConfig().GetInteger(PT_SECTION "-face", "minneighbors", 4),
            getConfig().GetInteger(PT_SECTION "-face", "flags", cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT),
            cv::Size(
                getConfig().GetInteger(PT_SECTION "-face", "minsize_x", 30),
                getConfig().GetInteger(PT_SECTION "-face", "minsize_y", 30)
            ),
            cv::Size(
                getConfig().GetInteger(PT_SECTION "-face", "maxsize_x", 0),
                getConfig().GetInteger(PT_SECTION "-face", "maxsize_y", 0)
            ));

    return true;
}

void PersonTracker::loadSettings() {
    getCamera().set( CV_CAP_PROP_FRAME_WIDTH, getConfig().GetInteger(
                PT_SECTION, "width", 640));
    getCamera().set( CV_CAP_PROP_FRAME_HEIGHT, getConfig().GetInteger(
                PT_SECTION, "height", 480));
    getCamera().set( CV_CAP_PROP_BRIGHTNESS, getConfig().GetInteger(
                PT_SECTION, "brightness", 50));
    getCamera().set( CV_CAP_PROP_CONTRAST, getConfig().GetInteger(
                PT_SECTION, "contrast", 50));
    getCamera().set( CV_CAP_PROP_SATURATION, getConfig().GetInteger(
                PT_SECTION, "saturation", 50));
    getCamera().set( CV_CAP_PROP_GAIN, getConfig().GetInteger(
                PT_SECTION, "gain", 50));
}
