#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class CameraTicket;
class LiveActor;
class EventFlowEventData;
class IUseCamera;
class EventFlowRequestInfo;
class EventFlowScareCtrlBase;
class BalloonOrderGroup;

class EventFlowDataHolder {
public:
    EventFlowDataHolder();

    bool isExistCameraInfo(const char*) const;
    const char* getCameraName(s32) const;
    void initCamera(const char*, CameraTicket*);
    void initCameraSubTargetAfterChart(const LiveActor*);
    void stopMovementByNode();
    void restartMovementByNode();
    void initCharacterName(const char16*);
    void initItemType(s32);
    void addItemType(const char*);
    void setActorFront(const sead::Vector3f&);
    const char* tryGetCharacterName() const;
    void convertActionName(sead::BufferedSafeString*, const char*) const;
    s32 sendEvent(const EventFlowEventData*);
    const char* judgeQuery(const char*) const;
    void setTalkSubActorName(const char*);
    void tryCreateCameraInfoBySystem(const char*);
    void startEventCamera(IUseCamera*, const char*, s32);
    void startEventAnimCamera(IUseCamera*, const char*, const char*, s32);
    void endEventCamera(IUseCamera*, const char*, s32, bool);
    bool tryEndEventCameraIfPlaying(IUseCamera*, const char*, s32, bool);
    void endAllEventCamera(IUseCamera*);
    bool isEndInterpoleCamera(const IUseCamera*, const char*) const;
    bool isPlayingEventAnimCamera(const char*) const;

private:
    EventFlowRequestInfo* mRequestInfo;
    void* filler_8[10];
    void* _58;
    void* filler_60[1];
    EventFlowScareCtrlBase* mScareCtrl;
    void* _70;
    void* filler_78[2];
    BalloonOrderGroup* mBalloonOrderGroup;
    sead::WFixedSafeString<32> _90;
    void* filler_e8[1];
    const char* mTalkSubActorName;
    s32 mItemTypeCapacity;
    s32 mItemTypeCount;
    const char** mItemTypes;
};

static_assert(sizeof(EventFlowDataHolder) == 0x108);

}  // namespace al
