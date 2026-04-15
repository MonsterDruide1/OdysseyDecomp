#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SimpleCircleShadowXZ : public LiveActor {
public:
    SimpleCircleShadowXZ(const char* name);
    void initSimpleCircleShadow(LiveActor* rootActor, const ActorInitInfo& info,
                                const char* archiveName, const char* suffix);
    void makeActorAlive() override;
    void updatePose();
    void control() override;
    void syncHostVisible();
    void setOffsetWithInterpole(const sead::Vector3f& offset);
    void setScaleWithInterpole(const sead::Vector3f& scale);
    void setRotateWithInterpole(const sead::Vector3f& rotate);
    void setInterpoleFrame(s32 frame);

private:
    LiveActor* mRootActor;
    sead::Vector3f mOffsetInterpole;
    bool mIsChildHidden;
    bool mIsSyncHostVisible;
    sead::Vector3f mActorScale;
    sead::Vector3f mScale;
    sead::Vector3f field_138;
    sead::Vector3f mOffset;
    sead::Vector3f field_150;
    sead::Vector3f field_15c;
    sead::Vector3f mRotate;
    s32 mInterpoleStep;
    s32 mInterpoleFrame;
};

static_assert(sizeof(SimpleCircleShadowXZ) == 0x180);
}  // namespace al

class SimpleSignBoard : public al::LiveActor {
public:
    SimpleSignBoard(const char* name);
    void init(const al::ActorInitInfo& info) override;
};

namespace SimpleSignBoardFunction {
void startSignAimVisAnimFromModelName(al::LiveActor* actor, const al::ActorInitInfo& info);
}  // namespace SimpleSignBoardFunction

static_assert(sizeof(SimpleSignBoard) == 0x108);
