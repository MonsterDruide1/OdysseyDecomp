#pragma once

#include <basis/seadTypes.h>

#include "Library/Controller/IUsePadDataReader.h"

namespace al {
struct PadDataPack;

class PadDataArcReader : public IUsePadDataReader {
public:
    PadDataArcReader(const char* path);
    PadDataArcReader(const char* path, const char* resourceName);
    void readResource(const char* resourceName);
    void checkEnd();
    void read(PadDataPack* frameData) override;

    bool isEnd() const override { return mIsEnd; }

    u32 getCursorFrame() const override { return mCursorFrame; }

    s32 getRemainFrame() const override { return mTotalFrame - mCursorFrame; }

private:
    PadDataPack* mDataFrames = nullptr;
    u32 mCursorFrame = 0;
    u32 mTotalFrame = 0;
    bool mIsEnd = false;
    const char* mPath = nullptr;
};
}  // namespace al
