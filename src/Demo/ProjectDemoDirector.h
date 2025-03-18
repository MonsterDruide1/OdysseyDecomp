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

    void endInit(const al::ActorInitInfo&);
    void updateSystemOnly();
    void updateDemoSyncEventKeeper();
    void updateDemoActor(al::EffectSystem*);
    void isActiveDemo(const char*);
    void isActiveDemoWithPlayer();
    void isActiveDemoWithCinemaCaption() const;
    void requestStartDemo(const char*, ProjectDemoDirector::DemoType);
    void requestEndDemo(const char*, ProjectDemoDirector::DemoType);
    void requestStartDemoShineMainGet(Shine*, const char*);
    void requestEndDemoShineMainGet(const char*);
    void requestStartDemoShineGrandGet(Shine*, const char*);
    void requestEndDemoShineGrandGet(const char*);
    void setShine(Shine*);
    void clearShine();
    void requestStartDemoScenarioCamera(const char*);
    void requestEndDemoScenarioCamera(const char*);
    void addDemoAppearFromHomeToList(ShineTowerRocket*);
    void requestStartDemoAppearFromHome(const char*);
    void requestEndDemoAppearFromHome(const char*);
    void addDemoReturnToHomeToList(ShineTowerRocket*);
    void requestStartDemoReturnToHome(const char*);
    void requestEndDemoReturnToHome(const char*);
    void addDemoRiseMapPartsToList(RiseMapPartsHolder*);
    void requestStartDemoRiseMapParts(const char*);
    void requestEndDemoRiseMapParts(const char*);
    void isExistDemoWorldLandInList() const;
    void isExistDemoAppearFromHomeInList() const;
    void isExistDemoReturnToHome() const;
    void isExistDemoRiseMapPartsInList() const;
    void startCloseDemoFade(s32);
    void startOpenDemoFade(s32);
    void isDemoFadeCloseEnd() const;
    void isDemoFadeOpenEnd() const;
    void isWipeStateEnableOpenMenuOrSnapShotMode() const;
    void tryCreateDemoHackFirstDirector(al::LiveActor*, s32, const al::ActorInitInfo&);
    void noticeDemoStartToDemoHackFirstDirector();
    void setDemoEnvironmentChangeFlag(bool);
    void registerDemoChangeEffectObj(DemoChangeEffectObj*);
    void startDemo(const char*);
    void endDemo(const char*);

private:
    al::PlayerHolder* mPlayerHolder;
    al::GraphicsSystemInfo* mGfxSysInfo;
    void* _60;
    void* _68;
    void* mDemoList;
    Shine* mDemoShine;
    s32 mDemoType;
    IUseDemoSkip* mDemoSkipRequester;
    bool mIsDemoSkipping;
    void* mCinemaCaption;
    al::WipeSimple* mDemoWipe;
    void* _a8;
    void* _b0;
    bool mIsDemoEnvironmentChangeFlag;
    sead::PtrArrayImpl mDemoChangeEffectObjArray;
    s32 _d0;
    void* _d8;
    u16 _e0;
    s32 _e4;
};
