#include<iostream>
#include "sensors/dht22sensor.hpp"
#include "sensors/PIRsensor.hpp"
#include "core/messagebus.hpp"
// LIN bits:
#include "sensors/LinGatewaySensor.hpp"
#include "lin/LinBusSim.hpp"            // for now; replace with OpenLinBus later
#include "lin/LinSchedule.hpp"
#include "lin/LinNormalizer.hpp"

#include "lin/driver/SimLinDriver.hpp"
//#include "lin/driver/OpenLinDriver.hpp" // later, when we have real LIN

class sensorfactory
{
    public:
    static std::unique_ptr<ISensor> create(MessageBus& bus, const nlohmann::json& config)
    {
        auto type = config.at("type").get<std::string>();
        if(type == "DHT22Sensor")
        {
            std::cout<<"Creating DHT22Sensor\n";
            return std::make_unique<DHT22Sensor>(bus);
        }else if(type == "PIRsensor")
        {
            return std::make_unique<PIRsensor>(bus,config.at("interval_ms").get<int>());
        }else if (type == "LinGateway") {
            // channel & baud
            std::string channel = config.value("channel", "sim");//
            int baud = config.value("baudrate", 19200);//

            // schedule doing LIN frame IDs and periods in ms   
            std::vector<LinScheduleEntry> entries;
            if (config.contains("schedule")) {
                for (const auto& s : config["schedule"]) {
                    entries.push_back({ static_cast<uint8_t>(s.at("id").get<int>()),
                                        s.at("period_ms").get<int>(),
                                        {} });
                }
            } else {
                // default schedule for demo
                entries.push_back({0x10, 100, {}});
                entries.push_back({0x11, 200, {}});
            }

            LinScheduler scheduler{std::move(entries)};

            // signals mapping
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
                // demo mapping (aligns with simulator patterns)
                specs.push_back({0x10, "climate.temp_c", 0, 1, "u8", 1.0, 0.0, "little"});
                specs.push_back({0x10, "climate.humidity", 1, 1, "u8", 1.0, 0.0, "little"});
                specs.push_back({0x11, "seat.heater_on", 0, 1, "bool", 1.0, 0.0, "little"});
            }

            LinNormalizer normalizer{std::move(specs)};

            // Choose HAL: simulator for now  later can add real LIN bus e.g. via openlinbus,socketcan etc.

            std::unique_ptr<ILinBus> hal = std::make_unique<LinBusSim>();
            std::unique_ptr<ILinDriver> driver = std::make_unique<SimLinDriver>();

            return std::make_unique<LinGatewaySensor>(
                bus, std::move(hal), channel, baud, std::move(scheduler), std::move(normalizer),20,std::move(driver)
            );
        }
      
        throw std::runtime_error("Unknown sensor type: " + type);   
        return nullptr;
    }
};