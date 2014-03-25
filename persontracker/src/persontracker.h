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

    virtual std::vector<cv::Rect> getPersons();
    virtual bool getPersons(std::vector<cv::Rect>& persons);

    virtual std::vector<cv::Rect> getFaces();
    virtual bool getFaces(std::vector<cv::Rect>& faces);

    virtual bool setCascade(const std::string& filename) {
        return haarCascade.load(filename);
    }

    virtual void setSVMDetector(cv::InputArray svmdet) {
        hog.setSVMDetector(svmdet);
    }

    virtual const cv::Mat& getLastImg() const {
        return img;
    }

    virtual bool loadSettings();
    virtual bool open();
private:
    cv::VideoCapture* camera;
    cv::HOGDescriptor hog;
    cv::CascadeClassifier haarCascade;
    cv::Mat img;

    float face_scalefactor;
    int face_minneighbors;
    int face_flags;
    cv::Size face_minsize;
    cv::Size face_maxsize;
    bool flip_vertical;
    bool flip_horizontal;
};

#endif
