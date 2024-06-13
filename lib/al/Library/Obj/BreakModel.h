#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {

class BreakModel : public LiveActor {
public:
    BreakModel(const LiveActor* rootActor, const char* objName, const char* modelName,
               const char* fileSuffixName, const sead::Matrix34f* rootMtx,
               const char* breakActionName);
    void init(const ActorInitInfo&);
    void appear();
    void exeWait();
    void exeBreak();

private:
    const LiveActor* mParent;
    const sead::Matrix34f* mRootMtx;
    const char* mModelName;
    const char* mBreakAction;
    const char* mInitSuffix;
    f32 mActionFrameRate = 1.0f;
    bool mIsValid = false;
    s32 mStep = 0;
};
static_assert(sizeof(BreakModel) == 0x140, "al::BreakModel Size");

}  // namespace al
