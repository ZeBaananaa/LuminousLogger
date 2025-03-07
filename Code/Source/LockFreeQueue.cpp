#include "LockFreeQueue.hpp"

namespace Debug
{
    bool LockFreeQueue::PushLogToQueue(const std::string& a_log)
    {
        const size_t l_currentTail = m_tail.load(std::memory_order_relaxed);
        const size_t l_nextTail = (l_currentTail + 1) % m_capacity;

        if (l_nextTail == m_head.load(std::memory_order_acquire))
            return false;

        m_data[l_currentTail] = a_log;
        m_tail.store(l_nextTail, std::memory_order_release);
        return true;
    }

    std::optional<std::string> LockFreeQueue::PopLogFromQueue()
    {
        const size_t l_currentHead = m_head.load(std::memory_order_relaxed);

        if (l_currentHead == m_tail.load(std::memory_order_acquire))
            return std::nullopt; // nullopt == nullptr for optional object

        std::string l_log = m_data[l_currentHead];
        m_head.store((l_currentHead + 1) % m_capacity, std::memory_order_release);
        return l_log;

    }
}
