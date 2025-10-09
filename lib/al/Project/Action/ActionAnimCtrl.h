#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class ActorResource;
struct ActionAnimCtrlInfo;

class ActionAnimCtrl {
public:
    static ActionAnimCtrl* tryCreate(LiveActor*, const ActorResource*, const char*, const char*);

    ActionAnimCtrl(LiveActor* actor);

    void init(const ActorResource*, const char*, const char*);
    bool start(const char*);
    bool trySetFrame(f32);
    bool isExistAction(const char*) const;
    bool isActionOneTime(const char*) const;
    bool isActionEnd() const;
    void sortCtrlInfo();
    const char* findAnimInfo(const char*) const;
    const char* getPlayingActionName() const;
    f32 getActionFrameMax(const char*) const;
    f32 getFrame() const;
    f32 getFrameRate() const;

private:
    LiveActor* mParentActor;
    const char* mPlayingAnimName = nullptr;
    s32 mInfoCount = 0;
    ActionAnimCtrlInfo* mInfos = nullptr;
    const char* mPlayingActionName = nullptr;
    s32 mPartIndex = -1;
};

}  // namespace al
