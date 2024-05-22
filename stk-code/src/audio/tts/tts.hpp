#pragma once
#include <string>
#include <thread>
#include "main_loop.hpp"

class Tts {
    public:
    void speak(const std::string& text) {
        // if (MainLoop::already_speaking) {
        //     return;
        // }

        //MainLoop::already_speaking = true;
        std::thread t(&Tts::runEspeak, this, text);
        t.detach(); // Detach the thread to run independently
        //MainLoop::already_speaking = false;
    }

    protected:
    void runEspeak(const std::string& text) {
        std::string command = "espeak -vfr \"" + text + "\"";
        system(command.c_str());
    }
};