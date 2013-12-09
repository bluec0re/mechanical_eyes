#ifndef _persontracker_header_
#define _persontracker_header_

#include <string>
#include <opencv2/opencv.hpp>

class PersonTracker {
public:
    PersonTracker();
    PersonTracker(int device);
    PersonTracker(const std::string& filename);
    virtual ~PersonTracker();

    virtual cv::VideoCapture& getCamera() const;
private:
    cv::VideoCapture* camera;
};

#endif
