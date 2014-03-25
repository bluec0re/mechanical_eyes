/*
 * Copyright (c) 2014, Timo Schmid, Kevin Schaller
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the university nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <persontracker.h>
#include <utils.h>
#include <iostream>
#include <time.h>

int main(int argc, char **argv) {

    timespec start,end;
    std::cout << "Starting\n";
    PersonTracker pt;
    pt.loadSettings();
    if(!pt.open()) {
        std::cerr << "Error opening camera\n";
        return -1;
    }

    pt.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    pt.setCascade("faces.xml");
    float time;
    cv::Mat img;
/*
    std::cout << "Finding persons\n";
    clock_gettime(CLOCK_REALTIME, &start);
    std::vector<cv::Rect> persons = pt.getPersons();
    clock_gettime(CLOCK_REALTIME, &end);
    time = (end.tv_sec + end.tv_nsec / 1e9) - (start.tv_sec + start.tv_nsec / 1e9);
    std::cout << "Time: " << time << "s\n";
    img = pt.getLastImg();
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
    cv::imwrite("persons.png", img);*/

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
