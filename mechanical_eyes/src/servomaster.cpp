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
