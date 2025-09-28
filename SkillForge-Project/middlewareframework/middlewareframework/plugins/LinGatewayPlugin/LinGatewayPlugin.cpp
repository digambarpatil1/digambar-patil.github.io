#include "LinGatewayPlugin.hpp"

/**
 * @brief Create a LIN gateway sensor instance.
 *
 * Parses the provided JSON configuration to extract:
 * - polling interval
 * - frame schedule
 * - signal mapping for normalization
 *
 * If signals are not provided, a default demo mapping is used.
 *
 * @param bus Reference to the application's message bus.
 * @param config JSON configuration for scheduler and signals.
 * @return std::unique_ptr<ISensor> Constructed LinGatewaySensor instance.
 */
std::unique_ptr<ISensor> LinGatewayPlugin::createSensor(
    MessageBus& bus,
    const nlohmann::json& config) {

    // Extract polling interval (default 1000 ms)
    int poll_ms = config.value("poll_interval_ms", 1000);

    // Build frame schedule from config
    std::vector<LinScheduleEntry> schedule;
    if(config.contains("schedule")) {
        for(auto& e : config["schedule"]) {
            schedule.push_back({ 
                static_cast<uint8_t>(e.at("id").get<int>()),
                e.at("period_ms").get<int>(),
                {} // payload is handled internally in LinBusSim
            });
        }
    }
    LinScheduler scheduler(schedule);

    // Build signal mapping for normalization
    std::vector<SignalSpec> specs;
    if (config.contains("signals")) {
        for (const auto& sig : config["signals"]) {
            SignalSpec sp;
            sp.frame_id = static_cast<uint8_t>(sig.at("frame_id").get<int>());
            sp.name     = sig.at("name").get<std::string>();
            sp.byte     = sig.at("byte").get<size_t>();
            sp.len      = sig.at("len").get<size_t>();
            sp.type     = sig.at("type").get<std::string>();
            sp.scale    = sig.value("scale", 1.0);
            sp.offset   = sig.value("offset", 0.0);
            sp.endian   = sig.value("endian", "little");
            specs.push_back(sp);
        }
    } else {
        // Default demo mapping for simulator
        specs.push_back({0x10, "climate.temp_c", 0, 1, "u8", 1.0, 0.0, "little"});
        specs.push_back({0x10, "climate.humidity", 1, 1, "u8", 1.0, 0.0, "little"});
        specs.push_back({0x11, "seat.heater_on", 0, 1, "bool", 1.0, 0.0, "little"});
    }
    LinNormalizer normalizer{std::move(specs)};

    // Create simulated LIN bus and driver
    auto linBus = std::make_unique<LinBusSim>();
    auto driver = std::make_unique<SimLinDriver>();

    // Construct and return the LIN gateway sensor
    return std::make_unique<LinGatewaySensor>(
        bus, std::move(linBus), std::move(driver),
        scheduler, normalizer, poll_ms);
}

/**
 * @brief Factory function for plugin system.
 * 
 * @return IPlugin* Newly created LinGatewayPlugin instance.
 */
extern "C" IPlugin* create_plugin() {
    return new LinGatewayPlugin();
}

/**
 * @brief Destroy plugin instance.
 * 
 * @param p Pointer to plugin instance to delete.
 */
extern "C" void destroy_plugin(IPlugin* p) {
    delete p;
}
