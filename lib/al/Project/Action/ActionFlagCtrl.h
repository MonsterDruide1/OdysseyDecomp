#pragma once

#include <basis/seadTypes.h>

namespace al {
class HitSensorKeeper;
class LiveActor;

struct ActionFlagCtrlInfo {
    const char* name;
    void* _8;
    void* _10;
};

class ActionFlagCtrl {
public:
    static ActionFlagCtrl* tryCreate(LiveActor*, const char*);

    ActionFlagCtrl(LiveActor*, const char*);

    ActionFlagCtrlInfo* findFlagInfo(const char*) const;
    void initPost();
    void start(const char*);
    void startCtrlFlag();
    void startCtrlSensor();
    void update(f32, f32, f32, bool);
    void updateCtrlSensor(f32, f32, f32, bool);
    bool isFlagValidOn(s32, bool);
    bool isFlagValidOff(s32, bool);

private:
    LiveActor* mParentActor;
    const char* mName;
    HitSensorKeeper* mHitSensorKeeper = nullptr;
    s32 mInfoCount = 0;
    ActionFlagCtrlInfo** mInfos = nullptr;
    ActionFlagCtrlInfo* mLastFlag = nullptr;
    bool mIsBool = false;
};
}  // namespace al
