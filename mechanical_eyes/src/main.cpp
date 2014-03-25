#include <iostream>
#include <ctime>
#include <cmath>
#include <unistd.h>
#include <cstdlib>

#include <persontracker.h>
#include <utils.h>

#include "servomaster.h"

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

    std::vector<cv::Rect> faces;
    timespec last_face_found, current;
    bool posnull = true;
    while(1) {
        faces.clear();
//        std::cerr << "Searching...\n";
        if(pt.getFaces(faces) && faces.size()) {
            PersonRelativeLocator locator(pt.getLastImg().size());
            
            cv::Point2f loc = locator.locate(faces.at(0));
            std::cout << "Face found at " << loc.x << "," << loc.y << std::endl;

            sm.setHorizontal(loc.x);
            sm.setVertical(loc.y);
            clock_gettime(CLOCK_REALTIME, &last_face_found);
            posnull = false;
        } else if(!posnull) {
            // drive to pos null after 30 sec without a face
            clock_gettime(CLOCK_REALTIME, &current);
            if(current.tv_sec - last_face_found.tv_sec > 30) {
                posnull = true;
                sm.nullPositions();
            }
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
