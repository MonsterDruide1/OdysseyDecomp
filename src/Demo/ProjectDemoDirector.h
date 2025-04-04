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

class ProjectDemoDirector : public al::DemoDirector {
public:
    enum class DemoType {
        RiseMapParts = 0x1,
        EndHack = 0x3,
        UNK5 = 0x4,
        UNK6 = 0x5,
        ShineGet = 0x6,
        UNK8 = 0x7,
        UNK9 = 0x8,
        UNK10 = 0x9,
    };

    ProjectDemoDirector(const al::PlayerHolder*, al::GraphicsSystemInfo*);

    void endInit(const al::ActorInitInfo&) override;
    void updateSystemOnly();
    void updateDemoSyncEventKeeper();
    void updateDemoActor(al::EffectSystem*) override;
    bool isActiveDemo(const char*);
    bool isActiveDemoWithPlayer();
    bool isActiveDemoWithCinemaCaption() const;
    bool requestStartDemo(const char*, ProjectDemoDirector::DemoType);
    void requestEndDemo(const char*, ProjectDemoDirector::DemoType);
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
    void* mCinemaCaption;
    al::WipeSimple* mDemoWipe;
    DemoHackFirstDirector* mDemoHackFirstDirector;
    void* _b0;
    bool mIsDemoEnvironmentChangeFlag;
    sead::PtrArray<DemoChangeEffectObj> mDemoChangeEffectObjArray;
    s32 _d0;
    void* _d8;
    u16 _e0;
    s32 _e4;
};
