#include <DriverAcelerator.h>

namespace DriverAcelerator {
    // GLOBAL VARIABLES
    float finalLeft = 0;
    float finalRight = 0;
    float lastLeft = 0;
    float lastRight = 0;

    void registerLast(float lastLeft_, float lastRight_)
    {
        lastLeft = lastLeft_;
        lastRight = lastRight_;
    }

    void registerFinal(float finalLeft_, float finalRight_)
    {
        finalLeft = finalLeft_;
        finalRight = finalRight_;
    }

    void acelerate(float *newLeft, float *newRight, int steps)
    {
        // Versión simplificada: siempre retorna velocidades finales directamente
        *newLeft = finalLeft;
        *newRight = finalRight;
    }
}