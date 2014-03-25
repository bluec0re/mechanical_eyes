#include "servoblaster.h"

std::ostream& operator<<(std::ostream& os, const Servo& s) {
    os << "Servo " << s.servo << " [" << s.min << "," << s.max << "," << s.scale << "]";
    return os; 
}

