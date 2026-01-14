#include <DriverAutonomus.h>
#include <BluetoothSerial.h>

namespace BTAutonomo {
    extern String Name;
    extern BluetoothSerial BT;
    extern bool active;
    extern int current_mode;  // Nueva variable para el modo actual


    // Start BT CONTROL;
    void start();

    void loop();
}