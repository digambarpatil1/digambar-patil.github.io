#pragma once

class ISensor {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~ISensor() = default;
};