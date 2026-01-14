#pragma once
#include <PreCompile.h>
#include <BluetoothSerial.h>
#include <DriverControl.h>

namespace BTControl {
    extern String Name;
    extern BluetoothSerial BT;

    // Start BT CONTROL;
    void start();

    void loop();
}