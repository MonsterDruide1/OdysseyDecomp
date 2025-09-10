#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class CameraTicket;
class LiveActor;
class EventFlowEventData;
class IUseCamera;

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
};

}  // namespace al
