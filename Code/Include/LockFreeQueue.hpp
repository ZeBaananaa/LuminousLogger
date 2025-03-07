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
        explicit LockFreeQueue(const size_t a_capacity) :
            m_capacity{a_capacity}, m_head{0}, m_tail{0}, m_data(a_capacity) { m_data.resize(a_capacity); }

        bool PushLogToQueue(const std::string& a_log);
        std::optional<std::string> PopLogFromQueue();

        size_t GetCapacity() const { return m_capacity; }

    private:
        size_t m_capacity{};
        std::atomic<size_t> m_head{};
        std::atomic<size_t> m_tail{};
        std::vector<std::string> m_data{};
    };

}
