#pragma once
#include <vector>
#include <chrono>
#include <cstdint>
//LinScheduler: maintains schedule entries for deterministic polling.
struct LinScheduleEntry {
    uint8_t frame_id;
    int period_ms;
    std::chrono::steady_clock::time_point next_due;
};

class LinScheduler {
public:
    explicit LinScheduler(std::vector<LinScheduleEntry> entries) : entries_(std::move(entries)) {
        auto now = std::chrono::steady_clock::now();
        for (auto& e : entries_) e.next_due = now;
    }

    // Returns list of frame IDs due at 'now', and reschedules them
    std::vector<uint8_t> dueFrames(std::chrono::steady_clock::time_point now) {
        std::vector<uint8_t> out;
        for (auto& e : entries_) {
            if (now >= e.next_due) {
                out.push_back(e.frame_id);
                e.next_due = now + std::chrono::milliseconds(e.period_ms);
            }
        }
        return out;
    }

private:
    std::vector<LinScheduleEntry> entries_;
};
