#ifndef _servoblaster_header_
#define _servoblaster_header_


#ifdef __cplusplus
extern "C" {
#endif
int setServo(int servo, int position);
int getServoPosition(int servo);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
class Servo
{
public:
    Servo(int servo) : servo(servo) {}
    
    bool setPosition(int pos) const { 
        return setServo(servo, pos) != 0;
    }

    int getPosition() const {
        return getServoPosition(servo);
    }
private:
    int servo;
};
#endif

#endif
