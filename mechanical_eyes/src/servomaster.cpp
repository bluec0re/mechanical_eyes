#include "servomaster.h"
#include <config.h>

#define ME_SECTION "mechanical_eyes"


ServoManager::~ServoManager() {
    if(left_v)
        delete left_v;
    if(right_v)
        delete right_v;
    if(left_h)
        delete left_h;
    if(right_h)
        delete right_h;
}

void ServoManager::loadSettings() {
    loadServo(&left_h, true, true);
    loadServo(&right_h, true, false);
    loadServo(&left_v, false, true);
    loadServo(&right_v, false, false);
}

void ServoManager::nullPositions() {
    nullPosition(left_h);
    nullPosition(right_h);
    nullPosition(left_v);
    nullPosition(right_v);
}

void ServoManager::nullPosition(Servo* servo) {
    setPosition(servo, 0.5f);
}

void ServoManager::setPosition(Servo* servo, float pos) {
    if(servo) {
        servo->setRelPosition(pos);
    }
}

void ServoManager::setHorizontal(float pos) {
    setPosition(left_h, pos);
    setPosition(right_h, pos);
}

void ServoManager::setVertical(float pos) {
    setPosition(left_v, pos);
    setPosition(right_v, pos);
}

void ServoManager::loadServo(Servo** servo, bool horizontal, bool left) {
    const char *sname;
    const char *maxname, *minname, *scale;
    if(horizontal) {
        if(left) {
            sname = "servo_horizontal_left";
            maxname = "servo_horizontal_left_max";
            minname = "servo_horizontal_left_min";
            scale = "servo_horizontal_left_scale";
        } else {
            sname = "servo_horizontal_right";
            maxname = "servo_horizontal_right_max";
            minname = "servo_horizontal_right_min";
            scale = "servo_horizontal_right_scale";
        }
    } else {
        if(left) {
            sname = "servo_vertical_left";
            maxname = "servo_vertical_left_max";
            minname = "servo_vertical_left_min";
            scale = "servo_vertical_left_scale";
        } else {
            sname = "servo_vertical_right";
            maxname = "servo_vertical_right_max";
            minname = "servo_vertical_right_min";
            scale = "servo_vertical_right_scale";
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
    (*servo)->setScale(getConfig().GetReal(ME_SECTION, scale, 1.0));
}

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
