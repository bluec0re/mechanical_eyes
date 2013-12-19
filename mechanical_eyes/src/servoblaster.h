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
    Servo(int servo) : servo(servo),min(0),max(359) {}
    
    bool setPosition(int pos) const { 
        return setServoPosition(servo, pos) != 0;
    }

    int getPosition() const {
        return getServoPosition(servo);
    }

    void setMin(int m) { min = m;}
    void setMax(int m) { max = m;}

    bool setPosition(float relpos) const {
        return setServoPosition(servo, min + (max-min)*relpos);
    }

    float getRelPosition() const {
        return static_cast<float>(getServoPosition(servo)) / static_cast<float>(max-min);
    }


    friend std::ostream& operator<<(std::ostream& os, const Servo& s);
private:
    int servo;
    int min, max;
};

std::ostream& operator<<(std::ostream& os, const Servo& s) {
    os << "Servo " << s.servo << " [" << s.min << "," << s.max << "]" << " @ " << s.getPosition();
    return os;
}
#endif

#endif
