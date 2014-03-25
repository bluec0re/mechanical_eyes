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

    if(flip_vertical && flip_horizontal)
        flip(img, img, -1);
    if(flip_horizontal)
        flip(img, img, 1);
    else if(flip_vertical)
        flip(img, img, 0);

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

    if(flip_vertical && flip_horizontal)
        flip(img, img, -1);
    if(flip_horizontal)
        flip(img, img, 1);
    else if(flip_vertical)
        flip(img, img, 0);

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
    flip_vertical = getConfig().GetBoolean(PT_SECTION, "flip_vertical", false);
    flip_horizontal = getConfig().GetBoolean(PT_SECTION, "flip_horizontal", false);

    std::cout << "Resolution: " << width << "x" << height << std::endl
              << "Brightness: " << brightness << std::endl
              << "Contrast: " << contrast << std::endl
              << "Saturation: " << saturation << std::endl
              << "Gain: " << gain << std::endl
              << "Flip vertical: " << flip_vertical << std::endl
              << "Flip horizontal: " << flip_horizontal << std::endl;

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
