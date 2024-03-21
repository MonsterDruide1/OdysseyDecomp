#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
class HitSensorKeeper;

struct ActionFlagCtrlInfo {
    const char** mName;
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
    char* findFlatInfo(const char*);
    bool isFlagValidOn(int, bool);
    bool isFlagValidOff(int, bool);

private:
    LiveActor* mParentActor;
    sead::SafeString* mActionName;
    HitSensorKeeper* mHitSensor;
    int mInfoMax;
    ActionFlagCtrlInfo* mInfoArr;
};
}  // namespace al
