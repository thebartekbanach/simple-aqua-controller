#include <menu.h>

class JoystickControl {
    private:
        Menu::navRoot *nav;
        const byte xPin;
        const byte yPin;
        const byte btnPin;
        const unsigned short moveDelayTime;

    public:
        JoystickControl(Menu::navRoot &nav, const byte &xPin, const byte &yPin,
                        const byte &btnPin, const unsigned short &moveDelayTime = (unsigned short)0): 
            nav(&nav),
            xPin(xPin), 
            yPin(yPin), 
            btnPin(btnPin),
            moveDelayTime(moveDelayTime) {
                pinMode(xPin, INPUT);
                pinMode(yPin, INPUT);
                pinMode(btnPin, INPUT_PULLUP);
        }

        void pool() {
            int xVal = analogRead(this->xPin);
            int yVal = analogRead(this->yPin);
            
            if (xVal == 0 || yVal == 0) {
                this->nav->doNav(downCmd);
                delay(this->moveDelayTime);
            } else if (xVal > 1000 || yVal > 1000) {
                this->nav->doNav(upCmd);
                delay(this->moveDelayTime);
            } else if (digitalRead(this->btnPin) == 0) {
                this->nav->doNav(enterCmd);
            }
        }
};
