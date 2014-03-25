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
#ifndef _servoblaster_header_
#define _servoblaster_header_


#ifdef __cplusplus
extern "C" {
#endif
int setServoPosition(int servo, int position);
int getServoPosition(int servo);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include <iostream>
class Servo;
std::ostream& operator<<(std::ostream& os, const Servo& s);

class Servo
{
public:
    Servo(int servo) : servo(servo),min(0),max(359),scale(1.0) {}
    
    bool setPosition(int pos) const { 
        pos = pos + pos % 2;
        if(pos > max)
            pos = max;
        else if(pos < min)
            pos = min;
        return setServoPosition(servo, pos) != 0;
    }

    int getPosition() const {
        return getServoPosition(servo);
    }

    float getScale() const {
        return scale;
    }

    void setScale(float scale) {
        this->scale = scale;
    }

    void setMin(int m) { min = m;}
    void setMax(int m) { max = m;}

    bool setRelPosition(float relpos) const {
        if(scale < 0)
            return setPosition(max + scale*(max-min)*relpos);
        else
            return setPosition(min + scale*(max-min)*relpos);
    }

    float getRelPosition() const {
        return static_cast<float>(getServoPosition(servo)) / static_cast<float>(max-min);
    }


    friend std::ostream& operator<<(std::ostream& os, const Servo& s);
private:
    int servo;
    int min, max;
    float scale;
};

#endif

#endif
