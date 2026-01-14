#include <BTAutonomo.h>

namespace BTAutonomo {
    String Name = "Neurobot";
    BluetoothSerial BT = BluetoothSerial();
    bool active = false;
    int current_mode = 0;  // Variable para almacenar el modo actual
}

void ejecutarGiroInicial(int modo) {
    switch (modo) {
        case 1: 
            // Modo 1: Giro inicial hacia la derecha
            DriverMotor::writeMotor(0.88, -0.88); 
            delay(200);
            break;
        case 2: 
            // Modo 2: Giro inicial hacia la izquierda  
            DriverMotor::writeMotor(-0.88, 0.88); 
            delay(200);
            break;
        case 3: 
            // Modo 3: Giro inicial de 180° (hacia la derecha)
            DriverMotor::writeMotor(0.90, -0.90); 
            delay(350);
            DriverMotor::writeMotor(0.90, 0.90);
            delay(350);
            break;
        default: 
            break;
    }
    // Parar después del giro inicial
    DriverMotor::writeMotor(0, 0);
    delay(100);
    DriverAutonomus::previous_error = 0;
    DriverAutonomus::previous_output = 0;
}

void configurarModo(int modo) {
    // Configurar variables según el modo seleccionado
    switch(modo) {
        case 1:
            // Modo 1: sesgo hacia la derecha
            DriverAutonomus::previous_error = 0.75;
            DriverAutonomus::init_backward = false;
            break;
        case 2:
            // Modo 2: sesgo hacia la izquierda
            DriverAutonomus::previous_error = -0.75;
            DriverAutonomus::init_backward = false;
            break;
        case 3:
            // Modo 3: búsqueda inicial 180°
            DriverAutonomus::init_backward = true;
            DriverAutonomus::previous_error = 0.75;
            break;
        default:
            // Modo por defecto (modo 1)
            DriverAutonomus::previous_error = 0.75;
            DriverAutonomus::init_backward = false;
            break;
    }
    
    // Resetear estado de combate
    DriverAutonomus::combat_started = false;
    DriverAutonomus::previous_output = 0;
}

void BTAutonomo::start() {
    BT.begin(Name);
    DriverMotor::start();
    DriverSonic::start();
    DriverAutonomus::start();
}

void BTAutonomo::loop() {
    if (BT.available()) {
        char command = BT.read();
        
        if (command == '1') {
            // MODO 1: Activar con giro hacia la derecha
            current_mode = 1;
            active = true;
            configurarModo(1);
            ejecutarGiroInicial(1);
            
        } else if (command == '2') {
            // MODO 2: Activar con giro hacia la izquierda
            current_mode = 2;
            active = true;
            configurarModo(2);
            ejecutarGiroInicial(2);
            
        } else if (command == '3') {
            // MODO 3: Activar con giro de 180°
            current_mode = 3;
            active = true;
            configurarModo(3);
            ejecutarGiroInicial(3);
            
        } else if (command == '0') {
            // DESACTIVAR ROBOT
            active = false;
            current_mode = 0;
            DriverMotor::stopMotor();
            
            // Resetear variables para el próximo round
            DriverAutonomus::combat_started = false;
            DriverAutonomus::previous_error = 0;
            DriverAutonomus::previous_output = 0;
            DriverAutonomus::init_backward = false;
            // Resetear modo turbo
            DriverAutonomus::turbo_mode = false;
            DriverAutonomus::close_contact_start = 0;
        }
    }
    
    // Ejecutar control autónomo si está activo
    if(active) {
        DriverAutonomus::controlLoop();
    }
}