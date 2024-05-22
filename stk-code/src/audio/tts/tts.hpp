#ifndef HEADER_TTS_HPP
#define HEADER_TTS_HPP

#include <string>
#include <mutex>
#include "audio/tts/mutex_queue.hpp"
#include "audio/tts/mutex_bool.hpp"

class Tts {
    public:
    void speak(const std::string& text, bool add_to_queue, bool empty_queue);

    Tts();

    protected:
    MutexBool *m_in_use;
    // bool m_in_use; // assuming false by default
    // std::mutex m_in_use_mutex;   //CHECK WHETHER MUTEX IS NEEDED SINCE TTS IS THE ONLY CLASS TO LAUNCH THREADS AND THERE IS ONLY ONE INSTANCE OF IT
    
    MutexQueue *to_speak_q;

    void runEspeak(const std::string& text);
};


extern Tts *the_voice;

#endif