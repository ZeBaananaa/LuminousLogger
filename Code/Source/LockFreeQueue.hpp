#pragma once

#include <atomic>
#include <optional>
#include <string>
#include <vector>

namespace Debug
{
    class LockFreeQueue
    {
    public:
        explicit LockFreeQueue(const size_t a_capacity) : m_capacity{a_capacity}, m_head{0}, m_tail{0}, m_data(a_capacity) {}

        bool TryEnqueue(const std::string& a_log);
        std::optional<std::string> PopLogFromQueue();

        size_t GetCapacity() const { return m_capacity; }
        bool IsEmpty() const { return m_head.load(std::memory_order_acquire) == m_tail.load(std::memory_order_acquire); }

    private:
        const size_t m_capacity{ };
        alignas(64) std::atomic<size_t> m_head{ };
        alignas(64)std::atomic<size_t> m_tail{ };
        std::vector<std::optional<std::string>> m_data{ };
    };

}
