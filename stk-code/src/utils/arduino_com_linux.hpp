#ifndef ARDUINO_COM_H
#define ARDUINO_COM_H

#include <string>

class ArduinoCom {
private:
    int hSerial;
    bool connected;
    std::string portName;

public:
    ArduinoCom(std::string portName);
    ~ArduinoCom();

    bool isConnected();
    bool writeSerial(std::string data);
};

#endif
