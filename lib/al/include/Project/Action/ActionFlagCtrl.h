#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
struct ActionAnimCtrlInfo;
struct ActionAnimDataInfo;
class HitSensorKeeper;
class LiveActor;

struct ActionFlagCtrlInfo {
    const char* mName;
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
    void update(float, float, float, bool);
    void updateCtrlSensor(float, float, float, bool);
    bool isFlagValidOn(int, bool);
    bool isFlagValidOff(int, bool);

private:
    LiveActor* mParentActor;
    const char* mName;
    HitSensorKeeper* mHitSensorKeeper = nullptr;
    s32 mInfoCount = 0;
    ActionFlagCtrlInfo** mInfoTable = nullptr;
    ActionFlagCtrlInfo* mLastFlag = nullptr;
    bool mBool = false;
};
}  // namespace al
