#pragma once
#include <PreCompile.h>
#include <HCSR04.h>

namespace DriverSonic {

    enum PIN {
        TRIGGER = 4,
        ECHO_DERECHO = 14,
        ECHO_CENTRAL = 13,
        ECHO_IZQUIERDO = 15,
        ECHO_N = 3,
    };

    // Variables globales
    extern double* distances;
    // Start Driver
    void start();
    // Messure Sonic
    void readSonic();

}