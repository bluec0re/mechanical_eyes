#include <persontracker.h>
#include <utils.h>
#include <iostream>
#include <time.h>

int main(int argc, char **argv) {

    timespec start,end;
    std::cout << "Starting\n";
    PersonTracker pt;
    pt.loadSettings();
    pt.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    pt.setCascade("faces.xml");

    std::cout << "Finding persons\n";
    clock_gettime(CLOCK_REALTIME, &start);
    std::vector<cv::Rect> persons = pt.getPersons();
    clock_gettime(CLOCK_REALTIME, &end);
    float time = (end.tv_sec + end.tv_nsec / 1e9) - (start.tv_sec + start.tv_nsec / 1e9);
    std::cout << "Time: " << time << "s\n";
    cv::Mat img = pt.getLastImg();
    PersonRelativeLocator prl(img.size());
    for(size_t i = 0; i < persons.size(); i++) {
        cv::Rect r = persons[i];
        r.x += ::cvRound(r.width*0.1);
	r.width = ::cvRound(r.width*0.8);
	r.y += ::cvRound(r.height*0.06);
	r.height = ::cvRound(r.height*0.9);
        cv::rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
        cv::Point2f center = prl.locate(r);
        std::cout << "Person " << i << ":"
                  << " " << r
                  << " center: " << center << std::endl;
        center.x *= img.size().width;
        center.y *= img.size().height;
        cv::circle( img,
                center,
                16.0,
                cv::Scalar( 0, 0, 255 ),
                -1,
                8 );


    }
    cv::imwrite("persons.png", img);

    std::cout << "Finding faces\n";
    clock_gettime(CLOCK_REALTIME, &start);
    std::vector<cv::Rect> faces = pt.getFaces();
    clock_gettime(CLOCK_REALTIME, &end);
    time = (end.tv_sec + end.tv_nsec / 1e9) - (start.tv_sec + start.tv_nsec / 1e9);
    std::cout << "Time: " << time << "s\n";
    img = pt.getLastImg();
    RelativeLocator rl(img.size());
    for(size_t i = 0; i < faces.size(); i++) {
        cv::Rect r = faces[i];
        r.x += ::cvRound(r.width*0.1);
	r.width = ::cvRound(r.width*0.8);
	r.y += ::cvRound(r.height*0.06);
	r.height = ::cvRound(r.height*0.9);
        cv::rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
        cv::Point2f center = rl.locate(r);
        std::cout << "Face " << i << ":"
                  << " " << r
                  << " center: " << center<<std::endl;
        center.x *= img.size().width;
        center.y *= img.size().height;
        cv::circle( img,
                center,
                16.0,
                cv::Scalar( 0, 0, 255 ),
                -1,
                8 );


    }
    cv::imwrite("faces.png", img);


    return 0;
}
