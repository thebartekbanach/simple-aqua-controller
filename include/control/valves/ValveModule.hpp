#pragma once

#include "devices.hpp"

class ValveModule {
    public:
        virtual bool open(unsigned short valveId) = 0;
        virtual bool close(unsigned short valveId) = 0;
        virtual bool set(unsigned short valveId, bool state) = 0;
        virtual bool isClosed(unsigned short valveId) = 0;
        virtual bool areRemotesAvailable() = 0;
};
