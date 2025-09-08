#pragma once
#include "plugins/IPlugin.hpp"

class filesinkplugin : public IPlugin {
public:
    filesinkplugin(const std::string& fp = "logs/filesink.txt");
    std::string name() const override;
    void initialize(MessageBus& bus) override;
    void shutdown() override;
    ~filesinkplugin() override = default;
private:
    std::string filePath_;
};