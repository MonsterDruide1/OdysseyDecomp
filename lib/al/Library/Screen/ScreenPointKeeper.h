#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class Resource;
struct ActorInitInfo;
class ScreenPointTarget;
class ParameterIo;
class ParameterArray;
class ParameterObj;
class ParameterS32;

class ScreenPointKeeper {
public:
    static bool isExistFile(const Resource* resource, const char* fileName);
    ScreenPointKeeper();
    void initByYaml(LiveActor* actor, const Resource* resource, const ActorInitInfo& initInfo,
                    const char* name);
    void initArray(s32 size);

    // TODO: Add proper parameter names for va and vb
    ScreenPointTarget* addTarget(LiveActor* actor, const ActorInitInfo& initInfo,
                                 const char* targetName, f32 radius, const sead::Vector3f* va,
                                 const char* jointName, const sead::Vector3f& vb);
    void update();
    void validate();
    const ScreenPointTarget* getTarget(s32 index) const;
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    const ScreenPointTarget* getTarget(const char* targetName) const;
    bool isExistTarget(const char* targetName) const;

    bool isTargetArrayFull() {
        return mScreenPointTargets.size() >= mScreenPointTargets.capacity();
    }

private:
    sead::PtrArray<ScreenPointTarget> mScreenPointTargets;
    ParameterIo* mParameterIo;
    ParameterArray* mTargets;
    ParameterObj* mOptions;
    ParameterS32* mAddTargetNum;
};
}  // namespace al
