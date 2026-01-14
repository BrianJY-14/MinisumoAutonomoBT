#include <DriverMotor.h>

namespace DriverMotor {
    STATE state = STATE_STOP;
    float maxPWM = 200;
    const float TURBO_MAX_PWM = 255;  // PWM máximo para modo turbo

    
    STATE mapState(float valueLeft, float valueRight)
    {
        STATE state_return;
        if(valueLeft >= 0 && valueRight >= 0) state_return = STATE_FORWARD_FORWARD;
        else if(valueLeft >= 0 && valueRight < 0) state_return = STATE_FORWARD_BACKWARD;
        else if(valueLeft < 0 && valueRight >= 0) state_return = STATE_BACKWARD_FORWARD;
        else if(valueLeft < 0 && valueRight < 0) state_return = STATE_BACKWARD_BACKWARD;
        return state_return;
    }

    void configState(STATE state)
    {
        switch (state)
        {
            case STATE_FORWARD_FORWARD:
                digitalWrite(PIN::LEFT_FORWARD, HIGH);
                digitalWrite(PIN::LEFT_BACKWARD, LOW);
                digitalWrite(PIN::RIGHT_FORWARD, HIGH);
                digitalWrite(PIN::RIGHT_BACKWARD, LOW);
                break;
            case STATE_BACKWARD_BACKWARD:
                digitalWrite(PIN::LEFT_FORWARD, LOW);
                digitalWrite(PIN::LEFT_BACKWARD, HIGH);
                digitalWrite(PIN::RIGHT_FORWARD, LOW);
                digitalWrite(PIN::RIGHT_BACKWARD, HIGH);
                break;
            case STATE_FORWARD_BACKWARD:
                digitalWrite(PIN::LEFT_FORWARD, HIGH);
                digitalWrite(PIN::LEFT_BACKWARD, LOW);
                digitalWrite(PIN::RIGHT_FORWARD, LOW);
                digitalWrite(PIN::RIGHT_BACKWARD, HIGH);
                break;
            case STATE_BACKWARD_FORWARD:
                digitalWrite(PIN::LEFT_FORWARD, LOW);
                digitalWrite(PIN::LEFT_BACKWARD, HIGH);
                digitalWrite(PIN::RIGHT_FORWARD, HIGH);
                digitalWrite(PIN::RIGHT_BACKWARD, LOW);
                break;
        
            default:
                break;
        }
    }
    
    void start()   {
        pinMode(PIN::LEFT_PWM, OUTPUT);   
        pinMode(PIN::LEFT_FORWARD, OUTPUT);
        pinMode(PIN::LEFT_BACKWARD, OUTPUT);
        pinMode(PIN::RIGHT_FORWARD, OUTPUT);
        pinMode(PIN::RIGHT_BACKWARD, OUTPUT);
        pinMode(PIN::RIGHT_PWM, OUTPUT);
    }

    void freeMotor()   {
        digitalWrite(PIN::LEFT_FORWARD, LOW);
        digitalWrite(PIN::LEFT_BACKWARD, LOW);
        digitalWrite(PIN::RIGHT_FORWARD, LOW);
        digitalWrite(PIN::RIGHT_BACKWARD, LOW);
        analogWrite(PIN::LEFT_PWM, 0);
        analogWrite(PIN::RIGHT_PWM, 0);
        state = STATE_FREE;
    }
    
    void stopMotor()   {
        digitalWrite(PIN::LEFT_FORWARD, HIGH);
        digitalWrite(PIN::LEFT_BACKWARD, HIGH);
        digitalWrite(PIN::RIGHT_FORWARD, HIGH);
        digitalWrite(PIN::RIGHT_BACKWARD, HIGH);
        analogWrite(PIN::LEFT_PWM, 0);
        analogWrite(PIN::RIGHT_PWM, 0);
        state = STATE_STOP;
    }

    void writeMotor(float valueLeft, float valueRight)
    {
        // Escribir los valores (0 - 1.0) a PWM y al estado
        STATE new_state = mapState(valueLeft, valueRight);
        if(DriverMotor::state != new_state) {
            // Si el estado es distinto al anterior, cambiar la configuracion de pines
            DriverMotor::state = new_state;
            configState(DriverMotor::state);
        }
        // Escribir y corregir el factor de velocidad
        int pwmLeft = abs(valueLeft * maxPWM);
        int pwmRight = abs(valueRight * maxPWM);
        analogWrite(LEFT_PWM, pwmLeft);
        analogWrite(RIGHT_PWM, pwmRight);
    }

    void writeMotorTurbo(float valueLeft, float valueRight)
    {
        // Escribir los valores (0 - 1.0) a PWM y al estado (MODO TURBO)
        STATE new_state = mapState(valueLeft, valueRight);
        if(DriverMotor::state != new_state) {
            // Si el estado es distinto al anterior, cambiar la configuracion de pines
            DriverMotor::state = new_state;
            configState(DriverMotor::state);
        }
        // Escribir con PWM turbo (255 máximo)
        int pwmLeft = abs(valueLeft * TURBO_MAX_PWM);
        int pwmRight = abs(valueRight * TURBO_MAX_PWM);
        analogWrite(LEFT_PWM, pwmLeft);
        analogWrite(RIGHT_PWM, pwmRight);
    }

}
