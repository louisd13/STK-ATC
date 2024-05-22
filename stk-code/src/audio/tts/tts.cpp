#include "audio/tts/tts.hpp"
#include <thread>

Tts *the_voice = new Tts;

Tts::Tts() {
    to_speak_q = new MutexQueue;
    m_in_use = new MutexBool();
}

void Tts::speak(const std::string& text, bool add_to_queue, bool empty_queue) {
    // try to acquire lock
    //std::unique_lock<std::mutex> lock(m_in_use_mutex, std::try_to_lock);

// do this first so that nothing else goes in
    if (empty_queue) {
        to_speak_q->emptyQueue();
    }

    if (m_in_use->tryLock()) {
        printf("VOICE LOCKED\n");

        std::thread t(&Tts::runEspeak, this, text);
        t.detach(); // Detach the thread to run independently
        return;
    } 
    
    if (add_to_queue) {
        printf("add to queue: %s\n", text);
        // acquire lock on queue and add
        to_speak_q->addValue(text);
    }
}

void Tts::runEspeak(const std::string& text) {
    std::string nextText = text;

    while (nextText != "") {
        std::string command = "espeak -vfr \"" + text + "\"";
        system(command.c_str());

        nextText = to_speak_q->getFirstValue();
    }

    m_in_use->unlock();
    printf("VOICE UNLOCKED\n");
}