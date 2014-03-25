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

#include <iostream>
#include <ctime>
#include <cmath>
#include <unistd.h>
#include <cstdlib>

#include <persontracker.h>
#include <utils.h>

#include "servomaster.h"

#include "GPIOClass.h"

void lookAround(ServoManager& sm, float time) {
    float cycleTime = 20.f;
    float stepTime = fmod(time, cycleTime);
    float pos;
    if(stepTime > cycleTime/2.f) {
        pos = (stepTime-cycleTime/2.f) / (cycleTime/2.f);
    } else {
        pos = 1.f - stepTime / (cycleTime/2.f);
    }
    sm.setHorizontal(pos);
    sm.setVertical(pos);
    std::cerr << time << " " << stepTime << " Pos = " << 100*pos << std::endl;
}

void demoMode(ServoManager& sm, int sleeptime) {
    timespec start, current;
    clock_gettime(CLOCK_REALTIME, &start);
    while(1) {
        sleep(sleeptime);
        clock_gettime(CLOCK_REALTIME, &current);
        float time = static_cast<float>(current.tv_sec - start.tv_sec) + static_cast<float>(current.tv_nsec - start.tv_nsec) / 1e6f;

        lookAround(sm, time);
    }
}

void trackMode(ServoManager& sm) {
    PersonTracker pt;
    std::cout << "[*] Loading camera settings\n";
    if(!pt.loadSettings()) {
        std::cerr << "Error on loading settings\n";
        return;
    }
    if(!pt.open()) {
        std::cerr << "Error on open camera\n";
        return;
    }
    pt.setCascade("/etc/mechanical_eyes/faces.xml");

    GPIOClass ledGPIO("5");
    ledGPIO.export_gpio();
    ledGPIO.setdir_gpio("out");
    ledGPIO.setval_gpio("0");

    std::vector<cv::Rect> faces;
    timespec last_face_found, current, start;
    bool posnull = true;
    clock_gettime(CLOCK_REALTIME, &start);
    while(1) {
        faces.clear();
//        std::cerr << "Searching...\n";
        if(pt.getFaces(faces) && faces.size()) {
            ledGPIO.setval_gpio("1");
            PersonRelativeLocator locator(pt.getLastImg().size());
            
            cv::Point2f loc = locator.locate(faces.at(0));
            std::cout << "Face found at " << loc.x << "," << loc.y << std::endl;

            sm.setHorizontal(loc.x);
            sm.setVertical(loc.y);
            clock_gettime(CLOCK_REALTIME, &last_face_found);
            posnull = false;
        } else if(!posnull) {
            ledGPIO.setval_gpio("0");
            // drive to pos null after 30 sec without a face
            clock_gettime(CLOCK_REALTIME, &current);
            if(current.tv_sec - last_face_found.tv_sec > 30) {
                posnull = true;
                sm.nullPositions();
                clock_gettime(CLOCK_REALTIME, &start);
            }
        } else if(posnull) {
            clock_gettime(CLOCK_REALTIME, &current);
            float time = current.tv_sec - start.tv_sec + (current.tv_nsec - start.tv_nsec) / 1e9f;
            lookAround(sm, time);
        }
    }
}

void usage(const char* exec) {
    std::cout << "usage: "<< exec << " (-d) (-s <sec>)\n";
}

int main(int argc, char** argv) {
    ServoManager sm;
    bool demoMode = false;
    int sleeptime = 5;

    int opt;
    while((opt = getopt(argc, argv, "ds:h")) != -1) {
        switch(opt) {
            case 'd':
                demoMode = true;
                break;
            case 's':
                sleeptime = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                return 1;
        }
    }

    std::cout << "[*] Loading servo settings...";
    sm.loadSettings();
    std::cout << "DONE\n";
    std::cout << "[*] Driving to initial positions...";
    sm.nullPositions();
    std::cout << "DONE\n";
    std::cout << "Status:\n" << sm << std::endl;


    if(demoMode) {
        std::cout << "Going into demo mode\n";
        ::demoMode(sm, sleeptime);
    } else {
        std::cout << "Going into track mode\n";
        std::cout.flush();
        ::trackMode(sm);
    }

    return 0;
}
