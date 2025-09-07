#pragma once

#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Effect/IUseEffectKeeper.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Layout/IUseLayout.h"
#include "Library/Layout/IUseLayoutAction.h"
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Nerve/IUseNerve.h"
#include "Library/Scene/IUseSceneObjHolder.h"

namespace al {
class NerveKeeper;
class LayoutKeeper;
class LayoutActionKeeper;
class LayoutTextPaneAnimator;
class EffectKeeper;
class AudioKeeper;
class LayoutExecuteInfo;
class HitReactionKeeper;
class LayoutSceneInfo;
class LayoutPartsActorKeeper;
class CameraDirector;
class SceneObjHolder;
class MessageSystem;
class Nerve;

class LayoutActor : public IUseHioNode,
                    public IUseNerve,
                    public IUseLayout,
                    public IUseLayoutAction,
                    public IUseMessageSystem,
                    public IUseCamera,
                    public IUseAudioKeeper,
                    public IUseEffectKeeper,
                    public IUseSceneObjHolder {
public:
    LayoutActor(const char* name);

    virtual void appear();
    virtual void kill();

    virtual void movement();
    virtual void calcAnim(bool recursive);

    virtual NerveKeeper* getNerveKeeper() const override { return mNerveKeeper; }

    virtual const char* getName() const override { return mName.cstr(); }

    virtual EffectKeeper* getEffectKeeper() const override { return mEffectKeeper; }

    virtual AudioKeeper* getAudioKeeper() const override { return mAudioKeeper; }

    virtual LayoutActionKeeper* getLayoutActionKeeper() const override {
        return mLayoutActionKeeper;
    }

    virtual LayoutKeeper* getLayoutKeeper() const override { return mLayoutKeeper; }

    CameraDirector* getCameraDirector() const override;

    SceneObjHolder* getSceneObjHolder() const override;

    const MessageSystem* getMessageSystem() const override;

    virtual void control() {}

    void initLayoutKeeper(LayoutKeeper* layoutKeeper);
    void initActionKeeper();
    void initTextPaneAnimator(LayoutTextPaneAnimator*);
    void initExecuteInfo(LayoutExecuteInfo*);
    void initHitReactionKeeper(HitReactionKeeper*);
    void initSceneInfo(LayoutSceneInfo*);
    void initLayoutPartsActorKeeper(s32 capacity);
    void initEffectKeeper(EffectKeeper*);
    void initAudioKeeper(AudioKeeper*);
    void initNerve(const Nerve* nerve, s32 maxStates = 0);
    void setMainGroupName(const char*);
    void syncAction();

    bool isAlive() const { return mIsAlive; }

    LayoutExecuteInfo* getExecuteInfo() const { return mExecuteInfo; }

    LayoutSceneInfo* getLayoutSceneInfo() const { return mLayoutSceneInfo; }

private:
    sead::FixedSafeString<0x80> mName;
    NerveKeeper* mNerveKeeper;
    LayoutKeeper* mLayoutKeeper;
    LayoutActionKeeper* mLayoutActionKeeper;
    LayoutTextPaneAnimator* mTextPaneAnimator;
    EffectKeeper* mEffectKeeper;
    AudioKeeper* mAudioKeeper;
    LayoutExecuteInfo* mExecuteInfo;
    HitReactionKeeper* mHitReactionKeeper;
    LayoutSceneInfo* mLayoutSceneInfo;
    LayoutPartsActorKeeper* mLayoutPartsActorKeeper;
    bool mIsAlive;
};
}  // namespace al
