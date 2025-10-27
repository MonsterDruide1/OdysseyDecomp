#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
struct ActionAnimCtrlInfo;
struct ActionAnimDataInfo;
class HitSensorKeeper;
class LiveActor;

enum class CtrlFlag : s16 {
    NotFound = 0,
    ValidOff = 2,
    ValidOn = 3,
};

struct ActionSensorCtrlInfo {
    ActionSensorCtrlInfo();

    const char* name = nullptr;
    CtrlFlag state = CtrlFlag::NotFound;
    s16 startFrame = -1;
    s16 endFrame = -1;
};

static_assert(sizeof(ActionSensorCtrlInfo) == 0x10);

struct ActionFlagCtrlInfo {
    ActionFlagCtrlInfo();

    const char* actionName = nullptr;
    CtrlFlag* ctrlFlags = nullptr;
    ActionSensorCtrlInfo* sensorCtrlInfoArray = nullptr;
};

static_assert(sizeof(ActionFlagCtrlInfo) == 0x18);

class ActionFlagCtrl {
public:
    static ActionFlagCtrl* tryCreate(LiveActor* actor, const char* name);

    ActionFlagCtrl(LiveActor* actor, const char* name);

    void initPost();
    void start(const char* name);
    ActionFlagCtrlInfo* findFlagInfo(const char* name) const;
    void startCtrlFlag();
    void startCtrlSensor();
    void update(f32 frame, f32 frameRateMax, f32 frameRate, bool isStop);
    void updateCtrlSensor(f32 frame, f32 frameRateMax, f32 frameRate, bool isStop);
    bool isFlagValidOn(s32 index, bool isEnabled) const;
    bool isFlagValidOff(s32 index, bool isEnabled) const;

private:
    LiveActor* mParentActor;
    const char* mName;
    HitSensorKeeper* mHitSensorKeeper = nullptr;
    s32 mInfoCount = 0;
    ActionFlagCtrlInfo** mCtrlInfoArray = nullptr;
    ActionFlagCtrlInfo* mLastCtrlInfo = nullptr;
    bool mIsUpdateNeeded = false;
};

static_assert(sizeof(ActionFlagCtrl) == 0x38);
}  // namespace al
