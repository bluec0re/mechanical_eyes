#include <iostream>
#include <ctime>
#include <cmath>


#include "servoblaster.h"
#include <config.h>

#define ME_SECTION "mechanical_eyes"

class ServoManager;
std::ostream& operator<<(std::ostream& os, const ServoManager& sm);

class ServoManager
{
public:
    ServoManager() : left_v(0), right_v(0), left_h(0), right_h(0) {
    }

    virtual ~ServoManager() {
        if(left_v)
            delete left_v;
        if(right_v)
            delete right_v;
        if(left_h)
            delete left_h;
        if(right_h)
            delete right_h;
    }

    void loadSettings() {
        loadServo(&left_h, true, true);
        loadServo(&right_h, true, false);
        loadServo(&left_v, false, true);
        loadServo(&right_v, false, false);
    }

    void nullPositions() {
        nullPosition(left_h);
        nullPosition(right_h);
        nullPosition(left_v);
        nullPosition(right_v);
    }

    void nullPosition(Servo* servo) {
        setPosition(servo, 0.5f);
    }

    void setPosition(Servo* servo, float pos) {
        if(servo) {
            servo->setPosition(pos);
        }
    }

    void setHorizontal(float pos) {
        setPosition(left_h, pos);
        setPosition(right_h, pos);
    }

    void setVertical(float pos) {
        setPosition(left_v, pos);
        setPosition(right_v, pos);
    }

    Servo* getLeftHorizontalServo() const { return left_h;}
    Servo* getRightHorizontalServo() const { return right_h;}
    Servo* getLeftVerticalServo() const { return left_v;}
    Servo* getRightVerticalServo() const { return right_v;}

    friend std::ostream& operator<<(std::ostream& os, const ServoManager& sm);
private:
    void loadServo(Servo** servo, bool horizontal, bool left) {
        const char *sname;
        const char *maxname, *minname;
        if(horizontal) {
            if(left) {
                sname = "servo_horizontal_left";
                maxname = "servo_horizontal_left_max";
                minname = "servo_horizontal_left_min";
            } else {
                sname = "servo_horizontal_right";
                maxname = "servo_horizontal_right_max";
                minname = "servo_horizontal_right_min";
            }
        } else {
            if(left) {
                sname = "servo_vertical_left";
                maxname = "servo_vertical_left_max";
                minname = "servo_vertical_left_min";
            } else {
                sname = "servo_vertical_right";
                maxname = "servo_vertical_right_max";
                minname = "servo_vertical_right_min";
            }
        }

        int servonum = getConfig().GetInteger(ME_SECTION, sname, -1);
        if(servonum >= 0)
            *servo = new Servo(servonum);
        else {
            *servo = 0;
            return;
        }

        (*servo)->setMin(getConfig().GetInteger(ME_SECTION, minname, 0));
        (*servo)->setMax(getConfig().GetInteger(ME_SECTION, maxname, 359));
    }
private:
    Servo *left_h, *right_h;
    Servo *left_v, *right_v;
};


std::ostream& operator<<(std::ostream& os, const ServoManager& sm) {
    if(sm.getLeftHorizontalServo())
        os << "LH: " << *sm.getLeftHorizontalServo() << std::endl;
    if(sm.getRightHorizontalServo())
        os << "RH: " << *sm.getRightHorizontalServo() << std::endl;
    if(sm.getLeftVerticalServo())
        os << "LV: " << *sm.getLeftVerticalServo() << std::endl;
    if(sm.getRightVerticalServo())
        os << "RV: " << *sm.getRightVerticalServo() << std::endl;
    return os;
}


void lookAround(ServoManager& sm, float time) {
    float stepTime = fmod(time, 10.f);

    if(stepTime > 5.f) {
        sm.setHorizontal((stepTime-5.f) / 5.f);
        sm.setVertical((stepTime-5.f) / 5.f);
    } else {
        sm.setHorizontal(1.f - stepTime / 5.f);
        sm.setVertical(1.f - stepTime / 5.f);
    }
}

int main(int argc, char** argv) {
    ServoManager sm;

    std::cout << "[*] Loading servo settings...";
    sm.loadSettings();
    std::cout << "DONE\n";
    std::cout << "[*] Driving to initial positions...";
    sm.nullPositions();
    std::cout << "DONE\n";
    std::cout << "Status:\n" << sm << std::endl;

    timespec start, current;
    clock_gettime(CLOCK_REALTIME, &start);
    while(1) {
        clock_gettime(CLOCK_REALTIME, &current);
        float time = static_cast<float>(current.tv_sec - start.tv_sec) + static_cast<float>(current.tv_nsec - start.tv_nsec) / 1e6f;

        lookAround(sm, time);
    }

    return 0;
}
