#include <DriverDebug.h>

namespace DriverDebug {
    
    DebugFlags flags;
    unsigned long last_print_time = 0;

    void start() {
        Serial.begin(CONFIG::SERIAL_BAUDRATE);
        delay(1000); // Esperar estabilización
        
        Serial.println("\n\n");
        Serial.println("╔════════════════════════════════════════╗");
        Serial.println("║   ROZZUM 7134 - DEBUG MONITOR v1.0    ║");
        Serial.println("╚════════════════════════════════════════╝");
        Serial.println();
        
        // Configuración por defecto
        flags.sensors = true;
        flags.control = true;
        flags.motors = true;
        flags.mode = true;
        flags.turbo = true;
        flags.separator = true;
    }

    void printSensorDistances() {
        if (!flags.sensors) return;
        
        Serial.println("┌─ SENSORES ─────────────────────────────");
        Serial.print("│ IZQ: ");
        if (DriverSonic::distances[0] >= 0 && DriverSonic::distances[0] < 100) {
            Serial.print(DriverSonic::distances[0], 1);
            Serial.println(" cm");
        } else {
            Serial.println("---");
        }
        
        Serial.print("│ CEN: ");
        if (DriverSonic::distances[1] >= 0 && DriverSonic::distances[1] < 100) {
            Serial.print(DriverSonic::distances[1], 1);
            Serial.println(" cm");
        } else {
            Serial.println("---");
        }
        
        Serial.print("│ DER: ");
        if (DriverSonic::distances[2] >= 0 && DriverSonic::distances[2] < 100) {
            Serial.print(DriverSonic::distances[2], 1);
            Serial.println(" cm");
        } else {
            Serial.println("---");
        }
        
        // Distancia calculada al oponente
        float dist_opponent = DriverAutonomus::calculateDistanceToOpponent();
        Serial.print("│ OPONENTE: ");
        if (dist_opponent < DriverAutonomus::CONFIG::RANGE) {
            Serial.print(dist_opponent, 1);
            Serial.println(" cm");
        } else {
            Serial.println("NO DETECTADO");
        }
    }

    void printControlVariables() {
        if (!flags.control) return;
        
        Serial.println("┌─ CONTROL PID ──────────────────────────");
        
        // Objetivo normalizado
        float objective = DriverAutonomus::readObjetive();
        Serial.print("│ Objetivo: ");
        if (fabs(objective - 7.0) < 1e-3) {
            Serial.println("SIN OBJETIVO");
        } else {
            Serial.print(objective, 3);
            if (objective < -0.5) Serial.println(" (← IZQ)");
            else if (objective > 0.5) Serial.println(" (DER →)");
            else Serial.println(" (↑ CENTRO)");
        }
        
        // Variables PID
        Serial.print("│ Error Prev: ");
        Serial.println(DriverAutonomus::previous_error, 3);
        
        Serial.print("│ Output Prev: ");
        Serial.println(DriverAutonomus::previous_output, 3);
        
        // Estado de combate
        Serial.print("│ Combate: ");
        Serial.println(DriverAutonomus::combat_started ? "ACTIVO" : "INACTIVO");
        
        Serial.print("│ Init Backward: ");
        Serial.println(DriverAutonomus::init_backward ? "SÍ" : "NO");
        
        // Velocidad de LUT
        float dist = DriverAutonomus::calculateDistanceToOpponent();
        if (dist < DriverAutonomus::CONFIG::RANGE) {
            float speed = DriverAutonomus::getSpeedFromLUT(dist);
            Serial.print("│ Velocidad LUT: ");
            Serial.print(speed, 3);
            Serial.print(" (");
            Serial.print((int)(speed * 100));
            Serial.println("%)");
        }
    }

    void printMotorState() {
        if (!flags.motors) return;
        
        Serial.println("┌─ MOTORES ──────────────────────────────");
        
        // Estado actual
        Serial.print("│ Estado: ");
        switch(DriverMotor::state) {
            case DriverMotor::STATE_FORWARD_FORWARD:
                Serial.println("AVANCE");
                break;
            case DriverMotor::STATE_BACKWARD_BACKWARD:
                Serial.println("RETROCESO");
                break;
            case DriverMotor::STATE_FORWARD_BACKWARD:
                Serial.println("GIRO DERECHA");
                break;
            case DriverMotor::STATE_BACKWARD_FORWARD:
                Serial.println("GIRO IZQUIERDA");
                break;
            case DriverMotor::STATE_STOP:
                Serial.println("FRENADO");
                break;
            case DriverMotor::STATE_FREE:
                Serial.println("LIBRE");
                break;
            default:
                Serial.println("DESCONOCIDO");
        }
        
        // PWM máximo actual
        Serial.print("│ PWM Max: ");
        Serial.print((int)DriverMotor::maxPWM);
        Serial.println("/255");
    }

    void printModeInfo() {
        if (!flags.mode) return;
        
        Serial.println("┌─ MODO ─────────────────────────────────");
        Serial.print("│ Robot: ");
        Serial.println(BTAutonomo::active ? "ACTIVO ✓" : "DESACTIVADO");
        
        Serial.print("│ Modo Actual: ");
        switch(BTAutonomo::current_mode) {
            case 0:
                Serial.println("0 (Desactivado)");
                break;
            case 1:
                Serial.println("1 (Giro DER + Sesgo DER)");
                break;
            case 2:
                Serial.println("2 (Giro IZQ + Sesgo IZQ)");
                break;
            case 3:
                Serial.println("3 (Giro 180° + Búsqueda)");
                break;
            default:
                Serial.println("Desconocido");
        }
    }

    void printTurboStatus() {
        if (!flags.turbo) return;
        
        Serial.println("┌─ MODO TURBO ───────────────────────────");
        Serial.print("│ Estado: ");
        Serial.println(DriverAutonomus::turbo_mode ? "ACTIVADO 🔥" : "Normal");
        
        if (DriverAutonomus::close_contact_start > 0) {
            unsigned long contact_time = millis() - DriverAutonomus::close_contact_start;
            Serial.print("│ Contacto Cercano: ");
            Serial.print(contact_time);
            Serial.print(" ms / ");
            Serial.print(DriverAutonomus::TURBO_THRESHOLD);
            Serial.println(" ms");
            
            // Barra de progreso
            int progress = (contact_time * 20) / DriverAutonomus::TURBO_THRESHOLD;
            if (progress > 20) progress = 20;
            Serial.print("│ [");
            for (int i = 0; i < 20; i++) {
                Serial.print(i < progress ? "█" : "░");
            }
            Serial.println("]");
        } else {
            Serial.println("│ Sin contacto cercano");
        }
    }

    void printSeparator() {
        if (!flags.separator) return;
        Serial.println("└────────────────────────────────────────");
        Serial.println();
    }

    void printStatus() {
        // Control de intervalo de impresión
        unsigned long current_time = millis();
        if (current_time - last_print_time < CONFIG::DEBUG_INTERVAL) {
            return;
        }
        last_print_time = current_time;
        
        // Limpiar pantalla (opcional - comentar si no deseas)
        // Serial.write(27);       // ESC
        // Serial.print("[2J");    // Clear screen
        // Serial.write(27);       // ESC
        // Serial.print("[H");     // Home cursor
            // 🔥 Asegurar que distances NO sea NULL
        if (!BTAutonomo::active) {
        // Si el robot está detenido, debemos leer sensores manualmente
            DriverSonic::readSonic();
        }

        Serial.print("═══ TIMESTAMP: ");
        Serial.print(current_time);
        Serial.println(" ms ═══");
        Serial.println();
        
        // Imprimir todas las secciones habilitadas
        printModeInfo();
        printSensorDistances();
        printControlVariables();
        printMotorState();
        printTurboStatus();
        printSeparator();
    }

    void setDebugInterval(unsigned long interval_ms) {
        // CONFIG::DEBUG_INTERVAL = interval_ms; // No se puede modificar enum
        // Alternativa: usar variable externa
        Serial.print("Debug interval solicitado: ");
        Serial.print(interval_ms);
        Serial.println(" ms (modifica DEBUG_INTERVAL en código)");
    }

    void enableAll() {
        flags.sensors = true;
        flags.control = true;
        flags.motors = true;
        flags.mode = true;
        flags.turbo = true;
        flags.separator = true;
        Serial.println("✓ Todas las secciones de debug habilitadas");
    }

    void disableAll() {
        flags.sensors = false;
        flags.control = false;
        flags.motors = false;
        flags.mode = false;
        flags.turbo = false;
        flags.separator = false;
        Serial.println("✗ Todas las secciones de debug deshabilitadas");
    }

    void setFlags(bool sensors, bool control, bool motors, bool mode, bool turbo) {
        flags.sensors = sensors;
        flags.control = control;
        flags.motors = motors;
        flags.mode = mode;
        flags.turbo = turbo;
        
        Serial.println("Flags de debug actualizados:");
        Serial.print("  Sensors: "); Serial.println(sensors ? "ON" : "OFF");
        Serial.print("  Control: "); Serial.println(control ? "ON" : "OFF");
        Serial.print("  Motors: "); Serial.println(motors ? "ON" : "OFF");
        Serial.print("  Mode: "); Serial.println(mode ? "ON" : "OFF");
        Serial.print("  Turbo: "); Serial.println(turbo ? "ON" : "OFF");
    }
}