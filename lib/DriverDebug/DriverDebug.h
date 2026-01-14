#pragma once
#include <PreCompile.h>
#include <DriverSonic.h>
#include <DriverAutonomus.h>
#include <BTAutonomo.h>

// Sistema de debugging por monitor serial
namespace DriverDebug {

    // CONFIGURACIÓN GLOBAL
    enum CONFIG {
        SERIAL_BAUDRATE = 9600,
        DEBUG_INTERVAL = 100,  // Intervalo de impresión en ms (10 Hz)
    };

    // FLAGS DE DEBUGGING (activa/desactiva secciones)
    struct DebugFlags {
        bool sensors = true;           // Distancias de sensores
        bool control = true;           // Variables PID y objetivo
        bool motors = true;            // Estado de motores
        bool mode = true;              // Modo actual del robot
        bool turbo = true;             // Estado modo turbo
        bool separator = true;         // Separador visual
    };

    extern DebugFlags flags;
    extern unsigned long last_print_time;

    // FUNCIONES PÚBLICAS
    
    // Inicializar serial
    void start();
    
    // Imprimir todo el estado (llamar en loop)
    void printStatus();
    
    // Funciones individuales para debugging selectivo
    void printSensorDistances();
    void printControlVariables();
    void printMotorState();
    void printModeInfo();
    void printTurboStatus();
    void printSeparator();
    
    // Utilidades
    void setDebugInterval(unsigned long interval_ms);
    void enableAll();
    void disableAll();
    void setFlags(bool sensors, bool control, bool motors, bool mode, bool turbo);
}