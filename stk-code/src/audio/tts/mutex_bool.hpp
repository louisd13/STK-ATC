#ifndef HEADER_MUTEXBOOL_HPP
#define HEADER_MUTEXBOOL_HPP

#include <mutex>
#include <queue>

class MutexBool {
    protected:
    bool m_bool;
    std::mutex m_mutex;
    void setValue(bool b);
    void resetValue();

    public:
    MutexBool();

    //bool getValue();
    bool tryLock();
    void unlock();
};

#endif