#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class Resource;
struct ActorInitInfo;
class ScreenPointTarget;

class ScreenPointKeeper {
public:
    static bool isExistFile(const Resource*, const char*);
    ScreenPointKeeper();
    void initByYaml(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
    void initArray(s32);
    ScreenPointTarget* addTarget(LiveActor*, const ActorInitInfo&, const char*, f32,
                                 const sead::Vector3f*, const char*, const sead::Vector3f&);
    void update();
    void validate();
    const ScreenPointTarget* getTarget(s32) const;
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    const ScreenPointTarget* getTarget(const char*) const;
    bool isExistTarget(const char*) const;
};
}  // namespace al

namespace alScreenPointFunction {
void updateScreenPointAll(al::LiveActor*);
}
