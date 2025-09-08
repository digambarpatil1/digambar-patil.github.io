#include "plugins/filesinkplugin.hpp"
#include <fstream>


filesinkplugin::filesinkplugin(const std::string &fp)
{
    filePath_ = fp;
}

std::string filesinkplugin::name() const
{
    return std::string();
}

void filesinkplugin::initialize(MessageBus &bus)
{
    bus.subscribe([this](const Message& msg) {
        std::ofstream ofs(filePath_, std::ios::app);
        if (ofs.is_open()) {
            ofs << msg.sensorid << ": " << msg.payload.dump() << "\n";
        }
    });

}

void filesinkplugin::shutdown()
{
    // Nothing to clean up for now
}
