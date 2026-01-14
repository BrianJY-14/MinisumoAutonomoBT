#include <DriverSonic.h>

namespace DriverSonic {

    double* distances = nullptr;

    void start()   {
        byte* echo_pins = new byte[PIN::ECHO_N] {PIN::ECHO_IZQUIERDO, PIN::ECHO_CENTRAL, PIN::ECHO_DERECHO};
        HCSR04.begin(PIN::TRIGGER, echo_pins, PIN::ECHO_N);
    }

    void readSonic() {
        DriverSonic::distances = HCSR04.measureDistanceCm();
    }
}
