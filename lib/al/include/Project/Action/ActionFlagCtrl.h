#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
class ActionAnimCtrlInfo;
class ActionAnimDataInfo;
class HitSensorKeeper;
class LiveActor;

struct ActionFlagCtrlInfo {
    const char* mName;
};

class ActionFlagCtrl {
public:
    static LiveActor** tryCreate(LiveActor*, const char*);

    ActionFlagCtrl(LiveActor*, const char*);

    void initPost();
    void start(const char*);
    void startCtrlFlag();
    void startCtrlSensor();
    void update(float, float, float, bool);
    void updateCtrlSensor(float, float, float, bool);
    bool isFlagValidOn(int, bool);
    bool isFlagValidOff(int, bool);
    const char* findFlagInfo(const char*);

private:
    LiveActor* mParentActor;
    const char* mName;
    HitSensorKeeper* mHitSensor;
    int mStackSize;
    ActionFlagCtrlInfo* mFlagStack;
};
}  // namespace al
