#pragma once

#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/ISceneObj.h"

#include "Npc/SessionMusicianNpc.h"
#include "Scene/SceneObjFactory.h"

namespace al {
class ISceneObj;
class LiveActor;
class PlacementInfo;
}  // namespace al

class SessionMayorNpc;
class SessionMusicianBgmController;
class SessionMusicianType;

class SessionMusicianManager : public al::LiveActor, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_SessionMusicianManager;

    SessionMusicianManager(const char* name);
    ~SessionMusicianManager() {};

    void initAfterPlacementSceneObj(const al::ActorInitInfo& info);

    void entryMayor(SessionMayorNpc* mayor) { mMayorNpc = mayor; };

    void entryMusician(SessionMusicianNpc* musician);
    bool isJoinedMusician() const;
    SessionMusicianNpc* getJoinedMusician() const;
    bool isSubscribed(SessionMusicianType type) const;

    bool tryAppearPowerPlant();
    SessionMusicianNpc* findPowerPlant() const;

    bool tryStartWarp(al::PlacementInfo* info);

    void addDemoAllMusicians();

    void exeWait();
    void exeComplete();

    sead::PtrArray<SessionMusicianNpc> getMusicians() const { return mMusicianPtrArray; }

private:
    sead::PtrArray<SessionMusicianNpc> mMusicianPtrArray;
    SessionMusicianBgmController* mBgmController = nullptr;
    SessionMayorNpc* mMayorNpc = nullptr;
};
