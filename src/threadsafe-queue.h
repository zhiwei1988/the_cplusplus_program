//
// Created by ZhiWei Tan on 5/13/22.
//

#ifndef THE_CPLUSPLUS_PROGRAM_THREADSAFE_QUEUE_H
#define THE_CPLUSPLUS_PROGRAM_THREADSAFE_QUEUE_H

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue& other)
    {
        std::lock_guard<std::mutex> lk(m_mtx);
        m_dataQueue = other.m_dataQueue;
    }

    void push(T value);

    void wait_and_pop(T &value);

    bool try_pop(T &value);

    bool empty() const;
private:
    mutable std::mutex m_mtx;
    std::queue<T> m_dataQueue;
    std::condition_variable m_dataCond;
};

template<typename T>
void ThreadSafeQueue<T>::push(T value)
{
    std::lock_guard<std::mutex> lk(m_mtx);
    m_dataQueue.push(value);
    m_dataCond.notify_one();
}

template<typename T>
void ThreadSafeQueue<T>::wait_and_pop(T &value)
{
    std::unique_lock<std::mutex> lk(m_mtx);
    m_dataCond.wait(lk, [this] {return !m_dataQueue.empty();});
    value = m_dataQueue.front();
    m_dataQueue.pop();
}

template<typename T>
bool ThreadSafeQueue<T>::try_pop(T &value)
{
    std::lock_guard<std::mutex> lk(m_mtx);
    if (m_dataQueue.empty()) {
        return false;
    }

    value = m_dataQueue.front();
    m_dataQueue.pop();
    return true;
}

template<typename T>
bool ThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lk(m_mtx);
    return m_dataQueue.empty();
}

#endif //THE_CPLUSPLUS_PROGRAM_THREADSAFE_QUEUE_H
