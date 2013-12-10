#include "servoblaster.h"

int main(int argc, char** argv) {
    Servo s(0);
    
    int pos = s.getPosition();
    if(pos == -1)
        return -1;

    return 0;
}
