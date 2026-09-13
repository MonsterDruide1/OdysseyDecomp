#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

namespace al {
class BgmRhythmDetector;
class IUseActiveBgmLine;

class BgmRhythmSyncDirector {
public:
    BgmRhythmSyncDirector(IUseActiveBgmLine* activeBgmLine);

    void update();
    bool tryRegistTargetResourceName(const char* resourceName);

    bool isEnableRhythmAnim() const { return mIsEnableRhythmAnim; }

    bool isChangeActiveResourceName() const { return mIsChangeActiveResourceName; }

    BgmRhythmDetector* getActiveRhythmDetector() const { return mActiveRhythmDetector; }

private:
    IUseActiveBgmLine* mActiveBgmLine = nullptr;
    void* mCurrentRhythmDetector = nullptr;
    s32 mTargetResourceIndex = -1;
    bool mIsEnableRhythmAnim = false;
    bool mIsChangeActiveResourceName = false;
    u8 _16[2] = {};
    sead::PtrArray<const char*>* mTargetResourceNames = nullptr;
    BgmRhythmDetector* mActiveRhythmDetector = nullptr;
};

static_assert(sizeof(BgmRhythmSyncDirector) == 0x28);

}  // namespace al
