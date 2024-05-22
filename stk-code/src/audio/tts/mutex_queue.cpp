#include "audio/tts/mutex_queue.hpp"

// bool MutexQueue::isEmpty() {
//     printf("QUEUE LOCKED\n");
//     m_queue_mutex.lock();
//     bool result = m_to_speak_queue.empty();
//     m_queue_mutex.unlock();
//     printf("QUEUE UNLOCKED\n");

//     return result;
// }
    
// returns "" if nothing in queue
std::string MutexQueue::getFirstValue() {
    printf("QUEUE LOCKED\n");
    // acquire lock 
    m_queue_mutex.lock(); 

    // wait until queue is not empty 
    if (m_to_speak_queue.empty()) {
        printf("no other value in queue\n");
        m_queue_mutex.unlock();
        printf("QUEUE UNLOCKED\n");
        return "";
    }

    // retrieve item 
    std::string item = m_to_speak_queue.front(); 
    m_to_speak_queue.pop(); 
    printf("first value in queue: %s\n", item);

    m_queue_mutex.unlock();
    printf("QUEUE UNLOCKED\n");

    // return item 
    return item; 
}
    
void MutexQueue::addValue(std::string text) {
    printf("QUEUE LOCKED\n");

    // Acquire lock 
    m_queue_mutex.lock();

    // Add item 
    m_to_speak_queue.push(text); 
    printf("added to queue: %s\n", text);

    // Release lock
    m_queue_mutex.unlock(); 
    printf("QUEUE UNLOCKED\n");
}

void MutexQueue::emptyQueue() {
    printf("QUEUE LOCKED\n");
    // Acquire lock 
    m_queue_mutex.lock();

    // Add item 
    std::queue<std::string> empty;
    std::swap(m_to_speak_queue, empty);

    // Release lock
    m_queue_mutex.unlock(); 
    printf("QUEUE UNLOCKED\n");
}