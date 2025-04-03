#pragma once

#include <basis/seadTypes.h>

namespace al {
struct PadDataPack;

class IUsePadDataWriter {
public:
    virtual void write(const PadDataPack& frameData) = 0;
    virtual void open() = 0;
    virtual void close() = 0;
};
}  // namespace al
