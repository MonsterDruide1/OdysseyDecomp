#pragma once

#include <container/seadPtrArray.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/ISceneObj.h"

#include "Npc/SessionMusicianType.h"
#include "Scene/SceneObjFactory.h"

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
class PlacementInfo;
}  // namespace al

class SessionMayorNpc;
class SessionMusicianBgmController;
class SessionMusicianNpc;

class SessionMusicianManager : public al::LiveActor, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_SessionMusicianManager;

    SessionMusicianManager(const char* name);

    void initAfterPlacementSceneObj(const al::ActorInitInfo& info) override;
    void entryMusician(SessionMusicianNpc* musician);
    bool isJoinedMusician() const;
    SessionMusicianNpc* getJoinedMusician() const;
    bool isSubscribed(SessionMusicianType type) const;
    bool tryAppearPowerPlant();
    SessionMusicianNpc* findPowerPlant() const;
    bool tryStartWarp(al::PlacementInfo* placementInfo);
    void addDemoAllMusicians();
    void exeWait();
    void exeComplete();

    const sead::PtrArray<SessionMusicianNpc>& getMusicians() const { return mMusicians; }

    void setSessionMayor(SessionMayorNpc* mayor) { mMayor = mayor; }

private:
    sead::PtrArray<SessionMusicianNpc> mMusicians;
    SessionMusicianBgmController* mBgmController = nullptr;
    SessionMayorNpc* mMayor = nullptr;
};

static_assert(sizeof(SessionMusicianManager) == 0x130);

namespace SessionMusicianLocalFunction {
void tryCreateSessionMusicianManager(const al::IUseSceneObjHolder* holder);
SessionMusicianManager* getSessionMusicianManager(const al::IUseSceneObjHolder* holder);
bool isExistSessionMusicianManager(const al::IUseSceneObjHolder* holder);
bool tryStartWarpToSessionMayor(const al::IUseSceneObjHolder* holder,
                                al::PlacementInfo* placementInfo);
void entrySessionMayorToManager(SessionMayorNpc* mayor);
bool isJoinedSessionMusician(const al::IUseSceneObjHolder* holder);
SessionMusicianNpc* tryGetJoinedSessionMusicanActor(const al::IUseSceneObjHolder* holder);
bool tryAddJoinedSessionMusicianDemoActor(const al::IUseSceneObjHolder* holder);
bool tryGetSessionMoonGetDemoPlayerPos(sead::Vector3f* out, const al::IUseSceneObjHolder* holder);
bool tryGetSessionMoonGetDemoPlayerPose(sead::Quatf* out, const al::IUseSceneObjHolder* holder);
bool trySetJoinedSessionMusicianTransformForMoonGetDemo(const al::IUseSceneObjHolder* holder);
void addDemoAllMusicians(const al::IUseSceneObjHolder* holder);
}  // namespace SessionMusicianLocalFunction
