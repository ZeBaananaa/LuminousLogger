#include "LockFreeQueue.hpp"

namespace Debug
{
    bool LockFreeQueue::PushLogToQueue(const std::string& a_log)
    {
        size_t l_currentTail{};
        size_t l_nextTail{};
        do
        {
            l_currentTail = m_tail.load(std::memory_order_relaxed);
            l_nextTail = (l_currentTail + 1) & (m_capacity - 1);

            if (l_nextTail == m_head.load(std::memory_order_acquire))
                return false;
        }
        while (!m_tail.compare_exchange_weak(l_currentTail, l_nextTail, std::memory_order_release,
                                             std::memory_order_relaxed));

        m_data[l_currentTail] = a_log;
        return true;
    }

    std::optional<std::string> LockFreeQueue::PopLogFromQueue()
    {
        size_t l_currentHead{};
        do
        {
            l_currentHead = m_head.load(std::memory_order_relaxed);

            if (l_currentHead == m_tail.load(std::memory_order_acquire))
                return std::nullopt;
        }
        // Bitwise operation used for better optimization than the '%' sign (only when queue capacity is a power of 2)
        while (!m_head.compare_exchange_weak(l_currentHead, (l_currentHead + 1) & (m_capacity - 1), std::memory_order_release, std::memory_order_relaxed));

        return m_data[l_currentHead];;
    }
}
