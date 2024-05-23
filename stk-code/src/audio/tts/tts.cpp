#include "audio/tts/tts.hpp"
#include <thread>
#include <vector>

Tts *the_voice = new Tts;

Tts::Tts() {
    //to_speak_q = new MutexQueue;
    m_in_use = new MutexBool();
}

void Tts::speak(const std::string& text, bool add_to_queue, bool empty_queue) {
    // try to acquire lock
    //std::unique_lock<std::mutex> lock(m_in_use_mutex, std::try_to_lock);

// do this first so that nothing else goes in
    if (empty_queue) {
        to_speak_q.clear();
    }

    if (m_in_use->tryLock()) {
        //printf("VOICE LOCKED\n");              TODO CHECK IF OKKKK
        std::vector<std::string> copied = to_speak_q;
        //printf("to_speak size: %d\ncopied size: %d\n\n", to_speak_q.size(), copied.size());
        //std::copy(to_speak_q.begin(), to_speak_q.end(), copied);

        // send to voice
        std::thread t(&Tts::runEspeak, this, text, copied);
        t.detach(); // Detach the thread to run independently

        to_speak_q.clear(); // clear for other incoming values
        return;
    } 

    // add even if there is already a priority in there, because it will go away anyway
    if (add_to_queue) {
        //toSpeak t = {text, empty_queue};
        to_speak_q.push_back(text);
        
        // std::thread t(&Tts::updateQueue, this);
        // t.detach(); // Detach the thread to run independently
    }
}


void Tts::updateQueue() {
    if (m_in_use->tryLock()) {
        //printf("VOICE LOCKED\n");              TODO CHECK IF OKKKK
        std::vector<std::string> copied = to_speak_q;
        //printf("to_speak size: %d\ncopied size: %d\n\n", to_speak_q.size(), copied.size());
        //std::copy(to_speak_q.begin(), to_speak_q.end(), copied);

        // send to voice
        std::thread t(&Tts::runEspeak, this, "", copied);
        t.detach(); // Detach the thread to run independently

        to_speak_q.clear(); // clear for other incoming values
        return;
    }
}



// void Tts::updateQueue() {
//     do {

//     } while (!m_in_use->tryLock());

//     //printf("VOICE LOCKED\n");              TODO CHECK IF OKKKK
//     std::vector<std::string> copied = to_speak_q;
//     printf("to_speak size: %d\ncopied size: %d\n\n", to_speak_q.size(), copied.size());
//     to_speak_q.clear(); // let's just hope for not so much of a mess here

//     // send to voice, but no need to create another thread since this is already another thread
//     runEspeak("", copied);
//     //t.detach(); // Detach the thread to run independently
// }

void Tts::runEspeak(const std::string& text, const std::vector<std::string>& queue) {
    std::string nextText;
    for (int i = 0; i < queue.size(); ++i) {
        nextText = queue[i];

        if (nextText != "") {
            std::string command = "espeak -ven+m1 -s160 \"" + nextText + "\"";
            system(command.c_str());
        }
    }

    //bool prioritizeText = false;
    if (text != "") {
        std::string command = "espeak -ven+m1 -s160 \"" + text + "\"";
        system(command.c_str());
    }

    // // do not speak other waiting text
    // if (prioritizeText) {
    //     return;
    // }

    // // else keep saying other text
    // for (int j = i; j < queue.size(); ++j) {
    //     nextText = queue[i].text;

    // std::string command = "espeak -vfr \"" + nextText + "\"";
    // system(command.c_str());
    // }

    m_in_use->unlock();
    //printf("VOICE UNLOCKED\n");
}