#include "lin/LinNormalizer.hpp"
#include <chrono>

static int16_t read_i16_le(const uint8_t *p) { return (int16_t)((p[1] << 8) | p[0]); }
static uint16_t read_u16_le(const uint8_t *p) { return (uint16_t)((p[1] << 8) | p[0]); }

nlohmann::json LinNormalizer::normalize(const LinFrame &f) const
{
    nlohmann::json signals = nlohmann::json::object();

    for (const auto &s : specs_)
    {
        if (s.frame_id != f.id)
            continue;
        if (s.byte + s.len > f.dlc)
            continue;

        const uint8_t *p = &f.data[s.byte];
        double val = 0.0;

        if (s.type == "bool")
        {
            val = (*p != 0) ? 1.0 : 0.0;
        }
        else if (s.type == "u8")
        {
            val = static_cast<double>(*p);
        }
        else if (s.type == "u16")
        {
            uint16_t raw = read_u16_le(p);
            val = static_cast<double>(raw);
        }
        else if (s.type == "i16")
        {
            int16_t raw = read_i16_le(p);
            val = static_cast<double>(raw);
        }
        else
        {
            continue; // unsupported – can extend later
        }

        val = val * s.scale + s.offset;
        signals[s.name] = val;
    }

    nlohmann::json out = {
        {"source", "LIN"},
        {"frame_id", f.id},
        {"signals", signals},
        {"timestamp", std::chrono::duration_cast<std::chrono::milliseconds>(
                          f.ts.time_since_epoch())
                          .count()}};
    return out;
}
