#pragma once
#include <PreCompile.h>

// Cambio gradual por diferencia de velocidades 
// En base a la cantidad de pasos de graduacion que le quedan para completar el movimiento

namespace DriverAcelerator {
    // GLOBAL VARIABLES
    extern float finalLeft;
    extern float finalRight;
    extern float lastLeft;
    extern float lastRight;

    // Guarda las ultimas velocidades
    void registerLast(float lastLeft_, float lastRight_);

    // Registrar las maximas velocidades por estado al cambiar de estado
    void registerFinal(float finalLeft_, float finalRight_);
    
    void acelerate(float* newLeft, float* newRight, int steps);
}