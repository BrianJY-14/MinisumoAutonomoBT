#pragma once
#include <PreCompile.h>
#include <DriverMotor.h>
#include <DriverAcelerator.h>

// Controlador de movimiento en funcion a variable times que se altera de manera externa
// Segun el times y el TIME_UMBRAL, le indica al modulo acelerator cuantos steps le queda
// Ya no es necesario un time lapse
// Segun los times y el time umbral 
namespace DriverControl {
    // STATE
    enum STATE {
        STATE_FORWARD,
        STATE_BACKWARD,
        STATE_LEFT,
        STATE_RIGHT,
        STATE_FORWARD_LEFT,
        STATE_FORWARD_RIGHT,
        STATE_BACKWARD_LEFT,
        STATE_BACKWARD_RIGHT,
        STATE_STOP,
    };

    // CONFIG
    enum CONFIG {
        TIME_UMBRAL = 30,
        STOP_UMBRAL = 5,
        TIME_DELAY = 5,
    };

    // GLOBAL VARIABLES
    extern STATE state;
    extern int times;

    // Cambia el estado y maximas velocidades
    // Las maximas velocidades son definidas para cada estado, un modulo acelerator puede graduar el ascenso a ellas
    void updateState(char command);

    void updateLoop();
}