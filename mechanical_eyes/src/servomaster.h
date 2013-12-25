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
