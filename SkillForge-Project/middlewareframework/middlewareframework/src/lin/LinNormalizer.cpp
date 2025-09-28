#include "lin/LinNormalizer.hpp"
#include <chrono>

/**
 * @brief Read a signed 16-bit little-endian integer from a byte buffer.
 * 
 * @param p Pointer to the first byte.
 * @return int16_t Interpreted signed value.
 */
static int16_t read_i16_le(const uint8_t *p) {
    return (int16_t)((p[1] << 8) | p[0]);
}

/**
 * @brief Read an unsigned 16-bit little-endian integer from a byte buffer.
 * 
 * @param p Pointer to the first byte.
 * @return uint16_t Interpreted unsigned value.
 */
static uint16_t read_u16_le(const uint8_t *p) {
    return (uint16_t)((p[1] << 8) | p[0]);
}

/**
 * @brief Normalize a LIN frame into structured JSON.
 *
 * Iterates through the configured signal specifications (`specs_`), 
 * extracts values from the frame data, applies scaling and offset, 
 * and outputs them in JSON format with metadata.
 *
 * Supported types: `bool`, `u8`, `u16`, `i16`.
 *
 * Example output:
 * @code{.json}
 * {
 *   "source": "LIN",
 *   "frame_id": 34,
 *   "signals": {
 *       "speed": 123.4,
 *       "door_open": 1
 *   },
 *   "timestamp": 169876543210
 * }
 * @endcode
 *
 * @param f The LIN frame containing raw data.
 * @return nlohmann::json Structured JSON with decoded signals.
 */
nlohmann::json LinNormalizer::normalize(const LinFrame &f) const {
    nlohmann::json signals = nlohmann::json::object();

    for (const auto &s : specs_) {
        // Match only signals belonging to this frame ID
        if (s.frame_id != f.id)
            continue;

        // Ensure data length is valid
        if (s.byte + s.len > f.dlc)
            continue;

        const uint8_t *p = &f.data[s.byte];
        double val = 0.0;

        // Decode raw value based on signal type
        if (s.type == "bool") {
            val = (*p != 0) ? 1.0 : 0.0;
        }
        else if (s.type == "u8") {
            val = static_cast<double>(*p);
        }
        else if (s.type == "u16") {
            uint16_t raw = read_u16_le(p);
            val = static_cast<double>(raw);
        }
        else if (s.type == "i16") {
            int16_t raw = read_i16_le(p);
            val = static_cast<double>(raw);
        }
        else {
            continue; // Unsupported type – extend as needed
        }

        // Apply scaling and offset
        val = val * s.scale + s.offset;

        // Store normalized value
        signals[s.name] = val;
    }

    // Wrap signals with metadata
    nlohmann::json out = {
        {"source", "LIN"},
        {"frame_id", f.id},
        {"signals", signals},
        {"timestamp", std::chrono::duration_cast<std::chrono::milliseconds>(
                          f.ts.time_since_epoch())
                          .count()}};

    return out;
}
