#include <BTControl.h>

namespace BTControl {
    String Name = "Marino Silver";
    BluetoothSerial BT = BluetoothSerial();

    void start() {
        DriverMotor::start();
      	BT.begin(Name); 	// Inicializa el Bluetooth , con este nombre aparecera en su celular
    }

    void loop() {
        // Por temas de buffer, el bluettoth responde mejor cuando es cambio por touch
        if (BT.available()) {   		// Verifica si hay datos desde Bluetooth
      	    char command = BT.read();  	// Lee el comando
	  	    DriverControl::updateState(command);
        }
        // Itera el loop de control
        DriverControl::updateLoop();
    }
}

