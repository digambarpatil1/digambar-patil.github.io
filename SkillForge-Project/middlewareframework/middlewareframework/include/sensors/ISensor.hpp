#pragma once

class ISensor {
public:
    virtual ~ISensor() = default;

    // Start the sensor (polling, reading, publishing)
    virtual void start() = 0;

    // Stop the sensor
    virtual void stop() = 0;
};
