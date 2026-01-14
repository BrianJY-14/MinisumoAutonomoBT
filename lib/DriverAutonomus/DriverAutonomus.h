#pragma once
#include <DriverMotor.h>
#include <DriverSonic.h>

// Implementación de control discreto
namespace DriverAutonomus {

    enum CONFIG {
        RANGE = 60,  // Cambiado de 45 a 60 como Neurobot
        MAX_FOLLOW_TIME = 600,  // Tiempo máximo siguiendo sin objetivo (ms)

    };

    // Variables globales
    extern float K_E, K_PE, K_PO;
    extern float previous_error, previous_output;
    extern float setpoint;
    extern bool init_backward;
    extern bool combat_started;  // Nueva variable

    // Variables para modo turbo
    extern bool turbo_mode;
    extern unsigned long close_contact_start;
    extern unsigned long TURBO_THRESHOLD;
    extern float CLOSE_DISTANCE_THRESHOLD;

    // Variables para control de tiempo de seguimiento
    extern unsigned long target_lost_time;
    extern bool target_recently_lost;

    void start();
    float calculateDistanceToOpponent();
    float readObjetive();
    float getSpeedFromLUT(float distance);   // <-- Nueva LUT
    void executeOutput(float output);
    void executeSearch();
    void controlLoop();
}