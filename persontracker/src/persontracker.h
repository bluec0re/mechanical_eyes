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
