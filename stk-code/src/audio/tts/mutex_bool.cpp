#include "audio/tts/mutex_bool.hpp"

MutexBool::MutexBool() {
    m_bool = false;
}

// bool MutexBool::getValue() {
//     m_mutex.lock();
//     bool result = m_bool;
//     m_mutex.unlock();
    
//     return result;
// }

void MutexBool::setValue(bool b) {
    m_mutex.lock();
    m_bool = b;
    m_mutex.unlock();
}

void MutexBool::resetValue() {
    setValue(false);
}

bool MutexBool::tryLock() {
    m_mutex.lock();
    bool result;

    if (m_bool == true) {
        printf("voice is locked, no lock possible\n");
        result = false;
    } else {
        printf("voice is free, acquire lock\n");
        m_bool = true;
        result = true;
    }

    m_mutex.unlock();
    return result;
}

void MutexBool::unlock() {
    resetValue();
    printf("voice lock released\n");
}