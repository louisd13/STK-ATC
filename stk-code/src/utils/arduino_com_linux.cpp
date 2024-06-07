#include "arduino_com_linux.hpp"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>

// Initialize the global instance pointer
ArduinoCom* globalArduinoCom = nullptr;

ArduinoCom::ArduinoCom(std::string portName) : portName(portName), connected(false) {
    hSerial = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (hSerial == -1) {
        std::cerr << "Error in opening serial port" << std::endl;
        return;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(hSerial, &tty) != 0) {
        std::cerr << "Error getting serial port attributes" << std::endl;
        close(hSerial);
        return;
    }

    tty.c_cflag &= ~PARENB; // No parity
    tty.c_cflag &= ~CSTOPB; // One stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS; // No hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable reading and ignore control lines

    // Set baud rate
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    // Set input mode (non-canonical, no echo,...)
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Set output mode
    tty.c_oflag = 0;

    // Set timeout parameters
    tty.c_cc[VTIME] = 0; // Timeout in tenths of a second
    tty.c_cc[VMIN] = 0;  // Minimum number of characters to read

    if (tcsetattr(hSerial, TCSANOW, &tty) != 0) {
        std::cerr << "Error setting serial port attributes" << std::endl;
        close(hSerial);
        return;
    }

    connected = true;
}

ArduinoCom::~ArduinoCom() {
    if (connected) {
        connected = false;
        close(hSerial);
    }
}

bool ArduinoCom::isConnected() {
    return connected;
}

bool ArduinoCom::writeSerial(std::string data) {
    if (!connected) {
        //std::cerr << "Serial port not connected" << std::endl;
        return false;
    }

    ssize_t bytesSent = write(hSerial, data.c_str(), data.size());
    if (bytesSent == -1) {
        std::cerr << "Error writing to serial port" << std::endl;
        return false;
    }

    return true;
}


void initializeGlobalArduinoCom(std::string portName) {
    if (globalArduinoCom == nullptr) {
        globalArduinoCom = new ArduinoCom(portName);
    }
}