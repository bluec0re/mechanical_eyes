#include "utils.h"


cv::Point2f RelativeLocator::locate(const cv::Rect& frame) {
    cv::Point2f center(frame.x, frame.y);

    center.x += frame.width/2.f;
    center.y += frame.height/2.f;

    center.x /= image_size.width;
    center.y /= image_size.height;

    return center;
}

cv::Point2f PersonRelativeLocator::locate(const cv::Rect& frame) {
    float head_body_proportion = 1.f/8.f;
    cv::Point2f center(frame.x, frame.y);

    center.x += frame.width/2.f;
    center.y += frame.height/2.f * head_body_proportion;

    center.x /= image_size.width;
    center.y /= image_size.height;

    return center;
}

