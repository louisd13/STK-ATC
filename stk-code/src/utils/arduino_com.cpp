#include "arduino_com.hpp"
#include <iostream>

// Initialize the global instance pointer
ArduinoCom* globalArduinoCom = nullptr;

ArduinoCom::ArduinoCom(std::string portName) : portName(portName), connected(false) {
    hSerial = CreateFileA(static_cast<LPCSTR>(portName.c_str()),
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error in opening serial port" << std::endl;
        return;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state" << std::endl;
        return;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state" << std::endl;
        return;
    }

    connected = true;
}

ArduinoCom::~ArduinoCom() {
    if (connected) {
        connected = false;
        CloseHandle(hSerial);
    }
}

bool ArduinoCom::isConnected() {
    return this->connected;
}

bool ArduinoCom::writeSerial(std::string data) {
    if (!connected) {
        //std::cerr << "Serial port not connected" << std::endl;
       return false;
    }

    DWORD bytesSent;
    bool success = WriteFile(hSerial, data.c_str(), data.size(), &bytesSent, NULL);
    return success;
}

void initializeGlobalArduinoCom(std::string portName) {
    if (globalArduinoCom == nullptr) {
        globalArduinoCom = new ArduinoCom(portName);
    }
}
