#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class BalloonOrderGroup;
class CameraTicket;
class EventFlowEventData;
class EventFlowScareCtrlBase;
class IEventFlowActionNameConverter;
class IEventFlowQueryJudge;
class IUseCamera;
class LiveActor;
class MessageTagDataHolder;
class EventFlowRequestInfo;

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

    EventFlowRequestInfo* getRequestInfo() const { return mRequestInfo; }

    void setEventFlowActorParam(const void* actorParam) { mEventFlowActorParam = actorParam; }

    void setEventFlowActor(const LiveActor* actor) { mEventFlowActor = actor; }

    void setEventQueryJudge(const IEventFlowQueryJudge* queryJudge) {
        mEventQueryJudge = queryJudge;
    }

    void setEventActionNameConverter(const IEventFlowActionNameConverter* converter) {
        mEventActionNameConverter = converter;
    }

    void setMessageTagDataHolder(const MessageTagDataHolder* messageTagDataHolder) {
        mMessageTagDataHolder = messageTagDataHolder;
    }

    const MessageTagDataHolder* getMessageTagDataHolder() const { return mMessageTagDataHolder; }

    void setBalloonOrderGroup(BalloonOrderGroup* group) { mBalloonOrderGroup = group; }

    void swapCharacterName(const sead::WBufferedSafeString* name) { mSwappedCharacterName = name; }

    void resetCharacterName() { mSwappedCharacterName = nullptr; }

    const char* getTalkSubActorName() const { return mTalkSubActorName; }

    s32 getItemTypeCount() const { return mItemTypeCount; }

    const char* getItemType(s64 index) const { return mItemTypes[index]; }

private:
    EventFlowRequestInfo* mRequestInfo;
    void* _8;
    void* _10;
    const void* mEventFlowActorParam;
    const LiveActor* mEventFlowActor;
    void* _28;
    void* _30;
    void* _38;
    void* _40;
    void* _48;
    void* _50;
    void* _58;
    void* _60;
    EventFlowScareCtrlBase* mScareCtrl;
    const IEventFlowQueryJudge* mEventQueryJudge;
    const IEventFlowActionNameConverter* mEventActionNameConverter;
    const MessageTagDataHolder* mMessageTagDataHolder;
    BalloonOrderGroup* mBalloonOrderGroup;
    sead::WFixedSafeString<32> _90;
    const sead::WBufferedSafeString* mSwappedCharacterName;
    const char* mTalkSubActorName;
    s32 mItemTypeCapacity;
    s32 mItemTypeCount;
    const char** mItemTypes;
};

static_assert(sizeof(EventFlowDataHolder) == 0x108);
}  // namespace al
