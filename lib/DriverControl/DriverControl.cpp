#include <DriverControl.h>

namespace DriverControl {
    STATE state = STATE_STOP;
    int times = 0;

    void updateState(char command)
    {
        // En caso se lea un comando que no cambie el estado, este contara como si se repitiese el estado
        STATE new_state = state;
        switch (command) {
            case 'F': 
                new_state = STATE_FORWARD;
                DriverAcelerator::registerFinal(1.0,1.0);
                break;
            case 'B': 
                new_state = STATE_BACKWARD;
                DriverAcelerator::registerFinal(-1.0,-1.0);
                break;
            case 'L':
                new_state = STATE_LEFT;
                DriverAcelerator::registerFinal(-0.5,0.5);
                break;
            case 'R':
                new_state = STATE_RIGHT;
                DriverAcelerator::registerFinal(0.5,-0.5);
                break;
            case 'G': 
                new_state = STATE_FORWARD_LEFT;
                DriverAcelerator::registerFinal(0.5,1.0);
                break;
            case 'I': 
                new_state = STATE_FORWARD_RIGHT;
                DriverAcelerator::registerFinal(1.0,0.5);
                break;
            case 'H':
                new_state = STATE_BACKWARD_LEFT;
                DriverAcelerator::registerFinal(-0.5,-1.0);
                break;
            case 'J':
                new_state = STATE_BACKWARD_RIGHT;
                DriverAcelerator::registerFinal(-1.0,-0.5);
                break;
            case 'S':
                new_state = STATE_STOP;
                DriverAcelerator::registerFinal(0,0);
                break;
            case 'W':
                DriverMotor::maxPWM = 255;
                break;
            case 'w':
                DriverMotor::maxPWM = 120;
                break;
            default:
                break;
        }
        // Reinicia el contador de los times en el mismo estado
        if(new_state != state) {
            state = new_state;
            times = 0;
        }
    }

    void updateLoop()
    {
        // Loop de Control constante, con espaciado de tiempo 
        int steps = CONFIG::TIME_UMBRAL - times;
        if(steps >= 0) {
            if(state != STATE_STOP) {
                // Iterando el modulo acelerator hasta llegar al times umbral
                // Al acabarse los steps mandara la velocidad maxima
                float newLeft; float newRight;
                DriverAcelerator::acelerate(&newLeft, &newRight, steps);
                DriverMotor::writeMotor(newLeft,newRight);
                DriverAcelerator::registerLast(newLeft, newRight);
            }
            else {
                // Los pasos para detenerse seran menores, debe tener reaccion mas rapida
                int steps_stop = CONFIG::STOP_UMBRAL - times;
                if(steps_stop >= 0) {
                    float newLeft; float newRight;
                    DriverAcelerator::acelerate(&newLeft, &newRight, steps_stop);
                    DriverMotor::writeMotor(newLeft,newRight);
                    DriverAcelerator::registerLast(newLeft, newRight);
                }
                else if(steps_stop == -1) {
                    DriverAcelerator::registerLast(0,0);
                    DriverMotor::stopMotor();
                }
            }
            // Solo aumenta times hasta umbral para evitar desbordar la memoria
            // y solo mete delay durante la aceleracion para  mejor reaccion al bluetoh
            times++;
            delay(CONFIG::TIME_DELAY);
        }
    }

}