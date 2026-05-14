#pragma once

#include <container/seadPtrArray.h>

#include "Library/Scene/DemoDirector.h"

namespace al {
class PlayerHolder;
class GraphicsSystemInfo;
class WipeSimple;
}  // namespace al
class IUseDemoSkip;
class Shine;
class ShineTowerRocket;
class RiseMapPartsHolder;
class DemoChangeEffectObj;
class DemoHackFirstDirector;
class CaptionInfoHolder;
class BarrierField;

class ProjectDemoDirector : public al::DemoDirector {
public:
    enum class DemoType {
        Normal = 0x1,
        RiseMapParts = 0x1,
        NormalWithCinemaFrame = 0x2,
        WithPlayer = 0x3,
        WithPlayerCinemaFrame = 0x4,
        ShineGetDemo = 0x5,
        ShineGet = 0x6,
        WithPlayerKeepCarry = 0x7,
        KeepHackTalk = 0x8,
        SceneStartPlayerWalk = 0x9,
        WithPlayerKeepBindTalk = 0xA,
    };

    ProjectDemoDirector(const al::PlayerHolder*, al::GraphicsSystemInfo*);

    void endInit(const al::ActorInitInfo&) override;
    void updateSystemOnly();
    void updateDemoSyncEventKeeper();
    void updateDemoActor(al::EffectSystem*) override;
    bool isActiveDemo(const char*);
    bool isActiveDemoWithPlayer();
    bool isActiveDemoWithCinemaCaption() const;
    bool requestStartDemo(const char*, DemoType);
    void requestEndDemo(const char*, DemoType);
    bool requestStartDemoShineMainGet(Shine*, const char*);
    void requestEndDemoShineMainGet(const char*);
    bool requestStartDemoShineGrandGet(Shine*, const char*);
    void requestEndDemoShineGrandGet(const char*);
    void setShine(Shine*);
    void clearShine();
    bool requestStartDemoScenarioCamera(const char*);
    void requestEndDemoScenarioCamera(const char*);
    void addDemoAppearFromHomeToList(ShineTowerRocket*);
    bool requestStartDemoAppearFromHome(const char*);
    void requestEndDemoAppearFromHome(const char*);
    void addDemoReturnToHomeToList(ShineTowerRocket*);
    bool requestStartDemoReturnToHome(const char*);
    void requestEndDemoReturnToHome(const char*);
    void addDemoRiseMapPartsToList(RiseMapPartsHolder*);
    bool requestStartDemoRiseMapParts(const char*);
    void requestEndDemoRiseMapParts(const char*);
    bool isExistDemoWorldLandInList() const;
    bool isExistDemoAppearFromHomeInList() const;
    bool isExistDemoReturnToHome() const;
    bool isExistDemoRiseMapPartsInList() const;
    void startCloseDemoFade(s32);
    void startOpenDemoFade(s32);
    bool isDemoFadeCloseEnd() const;
    bool isDemoFadeOpenEnd() const;
    bool isWipeStateEnableOpenMenuOrSnapShotMode() const;
    DemoHackFirstDirector* tryCreateDemoHackFirstDirector(al::LiveActor*, s32,
                                                          const al::ActorInitInfo&);
    void noticeDemoStartToDemoHackFirstDirector();
    void setDemoEnvironmentChangeFlag(bool);
    void registerDemoChangeEffectObj(DemoChangeEffectObj*);
    bool startDemo(const char*) override;
    void endDemo(const char*) override;

    void setShineTowerRocket(ShineTowerRocket* shineTowerRocket) {
        mShineTowerRocket = shineTowerRocket;
    }

    ShineTowerRocket* getShineTowerRocket() const { return mShineTowerRocket; }

    void setDemoSkipRequester(IUseDemoSkip* demoSkipRequester) {
        mDemoSkipRequester = demoSkipRequester;
    }

    IUseDemoSkip* getDemoSkipRequester() const { return mDemoSkipRequester; }

    void setDemoSkipping(bool isDemoSkipping) { mIsDemoSkipping = isDemoSkipping; }

    bool isDemoSkipping() const { return mIsDemoSkipping; }

    void setCaptionInfoHolder(CaptionInfoHolder* captionInfoHolder) {
        mCaptionInfoHolder = captionInfoHolder;
    }

    CaptionInfoHolder* getCaptionInfoHolder() const { return mCaptionInfoHolder; }

    void setBossBarrierField(BarrierField* bossBarrierField) {
        mBossBarrierField = bossBarrierField;
    }

    BarrierField* getBossBarrierField() const { return mBossBarrierField; }

    bool isDemoEnvironmentChangeFlag() const { return mIsDemoEnvironmentChangeFlag; }

    s32 getRandomActionIndexDemoChangeWorld() const { return mRandomActionIndexDemoChangeWorld; }

    void setDisableUpdateCamera() { mIsDisableUpdateCamera = true; }

    bool isDisableUpdateCamera() const { return mIsDisableUpdateCamera; }

private:
    al::PlayerHolder* mPlayerHolder;
    al::GraphicsSystemInfo* mGfxSysInfo;
    ShineTowerRocket* mShineTowerRocket;
    RiseMapPartsHolder* mRiseMapPartsHolder;
    s32* mDemoList;
    Shine* mDemoShine;
    DemoType mDemoType;
    IUseDemoSkip* mDemoSkipRequester;
    bool mIsDemoSkipping;
    CaptionInfoHolder* mCaptionInfoHolder;
    al::WipeSimple* mDemoWipe;
    DemoHackFirstDirector* mDemoHackFirstDirector;
    BarrierField* mBossBarrierField;
    bool mIsDemoEnvironmentChangeFlag;
    sead::PtrArray<DemoChangeEffectObj> mDemoChangeEffectObjArray;
    s32 mRandomActionIndexDemoChangeWorld;
    void* _d8;
    bool mIsDisableUpdateCamera;
    bool _e1;
    s32 _e4;
};

static_assert(sizeof(ProjectDemoDirector) == 0xE8);
