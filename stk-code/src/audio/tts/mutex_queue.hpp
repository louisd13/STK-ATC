#ifndef HEADER_MUTEXQUEUE_HPP
#define HEADER_MUTEXQUEUE_HPP

#include <mutex>
#include <queue>

class MutexQueue {
    protected:
    std::queue<std::string> m_to_speak_queue;
    std::mutex m_queue_mutex;
    std::atomic<bool> count;

    public:
    //bool isEmpty();
    std::string getFirstValue();
    void addValue(std::string text);

    void emptyQueue();
};

#endif