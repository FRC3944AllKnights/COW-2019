#ifndef PUSHER_H
#define PUSHER_H

#include "frc/Talon.h"
#include "frc/DigitalInput.h"

class Pusher{
    public:
        Pusher();
        // Hold button to move Pusher
        void manualPush(bool pushButton);
        void manualRetract(bool retractButton);
   
    private: //assigning motor of Pusher
        frc::Talon pusherMotor{6};
        frc::DigitalInput limitExtend{0};
        frc::DigitalInput limitRetract{5};
};

#endif

    