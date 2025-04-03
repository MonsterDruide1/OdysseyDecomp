#pragma once

#include <basis/seadTypes.h>

namespace al {
struct PadDataPack;

class IUsePadDataReader {
public:
    virtual void read(PadDataPack* frameData) = 0;
    virtual bool isEnd() const = 0;
    virtual u32 getCursorFrame() const = 0;
    virtual s32 getRemainFrame() const = 0;

    virtual void close() {}
};
}  // namespace al
