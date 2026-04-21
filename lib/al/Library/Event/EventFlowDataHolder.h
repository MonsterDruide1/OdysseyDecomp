#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class CameraTicket;
class BalloonOrderGroup;
class EventFlowActorTerritory;
struct EventFlowActorParam;
class LiveActor;
class EventFlowEventData;
class EventFlowMovement;
class EventFlowRequestInfo;
class EventFlowScareCtrlBase;
class EventFlowWatchParam;
class IEventFlowEventReceiver;
class IUseCamera;
struct EventFlowExecutorState;

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

    EventFlowRequestInfo* getRequestInfo() { return mRequestInfo; }

    const EventFlowRequestInfo* getRequestInfo() const { return mRequestInfo; }

    EventFlowExecutorState* getExecutorState() { return mExecutorState; }

    const EventFlowExecutorState* getExecutorState() const { return mExecutorState; }

    const EventFlowActorParam* getDefaultActorParam() const { return mDefaultActorParam; }

    EventFlowWatchParam* getWatchParam() const { return mWatchParam; }

    const sead::Vector3f& getActorFront() const { return mActorFront; }

    bool isStopMovementByNode() const { return mIsStopMovementByNode; }

    EventFlowActorTerritory* getTerritory() { return mTerritory; }

    const EventFlowActorTerritory* getTerritory() const { return mTerritory; }

    BalloonOrderGroup* getBalloonOrderGroup() const { return mBalloonOrderGroup; }

    EventFlowScareCtrlBase* getScareCtrl() const { return mScareCtrl; }

    void clearTalkSubActorName() { mTalkSubActorName = nullptr; }

private:
    EventFlowRequestInfo* mRequestInfo = nullptr;
    void* _08 = nullptr;
    EventFlowExecutorState* mExecutorState = nullptr;
    EventFlowActorParam* mDefaultActorParam = nullptr;
    EventFlowWatchParam* mWatchParam = nullptr;
    sead::Vector3f mActorFront = sead::Vector3f::ez;
    bool mIsStopMovementByNode = false;
    u16 _36 = 0;
    s32 mItemType = 0;
    void* _40 = nullptr;
    void* _48 = nullptr;
    EventFlowActorTerritory* mTerritory = nullptr;
    IEventFlowEventReceiver* mEventReceiver = nullptr;
    EventFlowMovement* mMovement = nullptr;
    EventFlowScareCtrlBase* mScareCtrl = nullptr;
    void* mQueryJudge = nullptr;
    void* mActionNameConverter = nullptr;
    void* _80 = nullptr;
    BalloonOrderGroup* mBalloonOrderGroup = nullptr;
    sead::WFixedSafeString<32> mCharacterName = {u""};
    void* _e8 = nullptr;
    const char* mTalkSubActorName = nullptr;
    void* _f8 = nullptr;
    void* _100 = nullptr;
};

static_assert(sizeof(EventFlowDataHolder) == 0x108);
}  // namespace al
