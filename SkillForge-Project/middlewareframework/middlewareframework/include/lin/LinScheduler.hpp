#pragma once
#include <vector>
#include<chrono>
#include <cstdint>
//LinScheduler: maintains schedule entries for deterministic polling.
struct LinScheduleEntry {
    uint8_t id;
    int period_ms;
    //std::vector<uint8_t> payload; // optional default payload
    std::chrono::steady_clock::time_point next_due;
};

class LinScheduler {
public:
    LinScheduler() = default;
    LinScheduler(std::vector<LinScheduleEntry> entries)
        : entries_(std::move(entries)) {
            auto now = std::chrono::steady_clock::now();
             for (auto& e : entries_) e.next_due = now;
        }

    const std::vector<LinScheduleEntry>& getSchedule() const { return entries_; }
    // Returns list of frame IDs due at 'now', and reschedules them
    std::vector<uint8_t> dueFrames(std::chrono::steady_clock::time_point now) {
        std::vector<uint8_t> out;
        for (auto& e : entries_) {
            if (now >= e.next_due) {
                out.push_back(e.id);
                e.next_due = now + std::chrono::milliseconds(e.period_ms);
            }
        }
        return out;
    }
    int getPeriodMs(uint8_t id) const {
        for (const auto& entry : entries_) {
            if (entry.id == id) return entry.period_ms;
        }
        return 1000; // default fallback
    }


private:
    std::vector<LinScheduleEntry> entries_;
};
