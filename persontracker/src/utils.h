#ifndef _utils_header_
#define _utils_header_

#include <opencv2/opencv.hpp>

class RelativeLocator
{
public:
    RelativeLocator(const cv::Size& image_size) :
        image_size(image_size) {}

    virtual cv::Point2f locate(const cv::Rect& frame);

protected:
    cv::Size image_size;
};

class PersonRelativeLocator : public RelativeLocator
{
public:
    PersonRelativeLocator(const cv::Size& is) : RelativeLocator(is) {}
    virtual cv::Point2f locate(const cv::Rect& frame);
};

#endif
