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
#ifndef __SERVOMASTER_HEADER__
#define __SERVOMASTER_HEADER__

#include <iostream>
#include "servoblaster.h"

#define ME_SECTION "mechanical_eyes"

//class ServoManager;
//std::ostream& operator<<(std::ostream& os, const ServoManager& sm);

class ServoManager
{
public:
    ServoManager() : left_v(0), right_v(0), left_h(0), right_h(0) {
    }

    virtual ~ServoManager();

    void loadSettings();

    void nullPositions();

    void nullPosition(Servo* servo);

    void setPosition(Servo* servo, float pos);

    void setHorizontal(float pos);

    void setVertical(float pos);

    Servo* getLeftHorizontalServo() const { return left_h;}
    Servo* getRightHorizontalServo() const { return right_h;}
    Servo* getLeftVerticalServo() const { return left_v;}
    Servo* getRightVerticalServo() const { return right_v;}

    friend std::ostream& operator<<(std::ostream& os, const ServoManager& sm);
private:
    void loadServo(Servo** servo, bool horizontal, bool left);
private:
    Servo *left_h, *right_h;
    Servo *left_v, *right_v;
};

#endif
