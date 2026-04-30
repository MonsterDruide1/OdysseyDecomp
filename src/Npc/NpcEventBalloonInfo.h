#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class IUseMessageSystem;
class LiveActor;
class MessageTagDataHolder;
}  // namespace al

struct NpcEventBalloonRequestInfo {
    s32 priority = 0;
    sead::Vector3f localOffset = sead::Vector3f::zero;
    f32 scale = 1.0f;
    f32 collisionCheckOffsetRadius = 0.0f;
    bool isInvalidUiCollisionCheck = false;
};

class NpcEventBalloonInfo {
public:
    NpcEventBalloonInfo();

    void setupForMessageBalloon(const al::LiveActor* actor, const char16* message,
                                const al::MessageTagDataHolder* tagDataHolder);
    void reset();
    void setupForEmotionIconBalloon(const al::LiveActor* actor, const char* iconName);
    void setupForTalkIconBalloon(const al::LiveActor* actor, const char* iconName,
                                 bool isEnableButtonA);
    void setCommonParam(const NpcEventBalloonRequestInfo& requestInfo);

    void setInvalidUiCollisionCheck() { mIsInvalidUiCollisionCheck = true; }

    void setEntranceCamera() { mIsEntranceCamera = true; }

    void makeTextW(sead::WBufferedSafeString* out,
                   const al::IUseMessageSystem* messageSystem) const;
    bool isTypeMessage() const;
    bool isTypeEmotionIcon() const;
    bool isTypeTalkIcon() const;

private:
    const al::LiveActor* mActor;
    const char16* mMessage;
    s32 mType;
    const char* mIconName;
    sead::Vector3f mLocalOffset;
    f32 mScale;
    f32 mCollisionCheckOffsetRadius;
    s32 mPriority;
    bool mIsInvalidUiCollisionCheck;
    void* _40;
    bool mIsEnableButtonA;
    bool mIsEntranceCamera;
};

class NpcEventTalkInfo {
public:
    NpcEventTalkInfo();
    NpcEventTalkInfo(const al::LiveActor* actor, const char16* message,
                     const al::MessageTagDataHolder* tagDataHolder);

    void reset();

    void setEnableButtonA() { mIsEnableButtonA = true; }

    void setSystemMessage(s32 style) {
        mIsSystemMessage = true;
        mMessageStyle = style;
    }

private:
    const char16* mMessage;
    void* _8;
    const al::MessageTagDataHolder* mTagDataHolder;
    const al::LiveActor* mActor;
    bool mIsEnableButtonA = false;
    bool mIsSystemMessage = false;
    char _22[2] = {};
    s32 mMessageStyle = -1;
};

static_assert(sizeof(NpcEventBalloonRequestInfo) == 0x1c);
static_assert(sizeof(NpcEventBalloonInfo) == 0x50);
static_assert(sizeof(NpcEventTalkInfo) == 0x28);
