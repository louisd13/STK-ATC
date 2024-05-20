#pragma once
#include <string>
#include <thread>

class Tts {
    public:
    void speak(const std::string& text) {
        std::thread t(&Tts::runEspeak, this, text);
        t.detach(); // Detach the thread to run independently
    }

    protected:
    void runEspeak(const std::string& text) {
        std::string command = "espeak -vfr \"" + text + "\"";
        system(command.c_str());
    }
};