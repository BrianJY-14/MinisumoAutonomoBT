#include <BTAutonomo.h>
#include <DriverDebug.h>

//#include <BTControl.h>
#define ENABLE_DEBUG false

void setup() {
    BTAutonomo::start();
    #if ENABLE_DEBUG
        // Inicializar monitor serial
        DriverDebug::start();
        
        // 🔥 ACTIVAR SOLO LOS SENSORES
        DriverDebug::setFlags(
            true,   // sensors ON
            false,  // control OFF
            false,  // motors OFF
            false,  // mode OFF
            false   // turbo OFF
        );

        // 🔥 DESACTIVAR SEPARATOR (barra final)
        DriverDebug::flags.separator = false;

        Serial.println("DEBUG: Solo sensores ultrasónicos activados");
        Serial.println();
        
        Serial.println("✓ Sistema iniciado con DEBUG habilitado");
        Serial.println("  Baudrate: 115200");
        Serial.println("  Intervalo: 100 ms (10 Hz)");
        Serial.println();
    #endif
}

void loop() {
    BTAutonomo::loop();
    #if ENABLE_DEBUG
        // Imprimir estado cada DEBUG_INTERVAL ms
        DriverDebug::printStatus();
    #endif
}

/*#include <BTAutonomo.h>
//#include <BTControl.h>

void setup() {
    BTAutonomo::start();
}

void loop() {
    BTAutonomo::loop();
}

*/