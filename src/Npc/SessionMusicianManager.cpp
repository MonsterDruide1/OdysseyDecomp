#include "Npc/SessionMusicianManager.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Npc/SessionEventProgress.h"
#include "Npc/SessionMayorNpc.h"
#include "Npc/SessionMusicianBgmController.h"
#include "Npc/SessionMusicianLocalFunction.h"
#include "Npc/SessionMusicianNpc.h"
#include "Npc/SessionMusicianWarpAgent.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_IMPL(SessionMusicianManager, Wait);
NERVE_IMPL(SessionMusicianManager, Complete);

NERVES_MAKE_NOSTRUCT(SessionMusicianManager, Wait, Complete);
}  // namespace

SessionMusicianManager::SessionMusicianManager(const char* name) : al::LiveActor(name) {
    mMusicians.allocBuffer(7, nullptr);
}

void SessionMusicianManager::initAfterPlacementSceneObj(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorWithArchiveName(this, info, "SessionMusicianManager", nullptr);
    al::initNerve(this, &Wait, 0);
    al::initActorSeKeeperWithout3D(this, info, "SessionMusicianManager");

    if (mMayor != nullptr) {
        mMayor->initIntroductionCamera(info, &mMusicians);
        mBgmController = new SessionMusicianBgmController(this, info, false);
    }

    makeActorAlive();
}

void SessionMusicianManager::entryMusician(SessionMusicianNpc* musician) {
    mMusicians.pushBack(musician);
}

bool SessionMusicianManager::isJoinedMusician() const {
    if (mMusicians.size() == 0)
        return false;

    SessionMusicianNpc** musician = mMusicians.data();
    while (true) {
        if ((*musician++)->isJoined())
            return true;

        if (musician == &mMusicians.data()[mMusicians.size()])
            break;
    }

    return false;
}

SessionMusicianNpc* SessionMusicianManager::getJoinedMusician() const {
    if (mMusicians.size() == 0)
        return nullptr;

    SessionMusicianNpc** musician = mMusicians.data();
    while (true) {
        if ((*musician)->isJoined())
            return *musician;

        musician++;
        if (musician == &mMusicians.data()[mMusicians.size()])
            break;
    }

    return nullptr;
}

bool SessionMusicianManager::isSubscribed(SessionMusicianType type) const {
    if (mMusicians.size() == 0)
        return false;

    SessionMusicianNpc** musician = mMusicians.data();
    while (true) {
        if (SessionMusicianLocalFunction::isMusicianType(*musician, type) &&
            SessionMusicianLocalFunction::isAlreadySessionMember(*musician))
            return true;

        musician++;
        if (musician == &mMusicians.data()[mMusicians.size()])
            break;
    }

    return false;
}

bool SessionMusicianManager::tryAppearPowerPlant() {
    SessionMusicianNpc* powerPlant = findPowerPlant();
    if (powerPlant == nullptr || al::isAlive(powerPlant))
        return false;

    GameDataHolderAccessor accessor(this);
    if (GameDataFunction::getSessionEventProgress(accessor).value() <
        SessionEventProgress::WaitThePowerPlantWorks)
        return false;

    powerPlant->appear();
    return true;
}

SessionMusicianNpc* SessionMusicianManager::findPowerPlant() const {
    if (mMusicians.size() == 0)
        return nullptr;

    SessionMusicianNpc** musician = mMusicians.data();
    while (true) {
        if (SessionMusicianLocalFunction::getMusicianType(*musician) == SessionMusicianType::Vocal)
            return *musician;

        musician++;
        if (musician == &mMusicians.data()[mMusicians.size()])
            break;
    }

    return nullptr;
}

bool SessionMusicianManager::tryStartWarp(al::PlacementInfo* placementInfo) {
    if (mMusicians.size() == 0)
        return false;

    SessionMusicianNpc** musician = mMusicians.data();
    while (true) {
        if ((*musician)->isStateWarp()) {
            SessionMusicianWarpAgent* warpAgent = (*musician)->getWarpAgent();
            if (warpAgent->tryGetWarpTargetInfo(placementInfo) && warpAgent->tryStartWarp()) {
                (*musician)->doneWarp();
                al::invalidateClipping(mMayor);
                return true;
            }
        }

        musician++;
        if (musician == &mMusicians.data()[mMusicians.size()])
            break;
    }

    return false;
}

void SessionMusicianManager::addDemoAllMusicians() {
    if (mMusicians.size() == 0)
        return;

    SessionMusicianNpc** musician = mMusicians.data();
    while (true) {
        rs::addDemoActor(*musician++, true);

        if (musician == &mMusicians.data()[mMusicians.size()])
            break;
    }
}

void SessionMusicianManager::exeWait() {
    if (mBgmController != nullptr)
        mBgmController->updateNerve();

    if (!(SessionMusicianLocalFunction::getMemberMusicianNum(this) < 5))
        return al::setNerve(this, &Complete);

    SessionMusicianNpc* powerPlant = findPowerPlant();
    if (powerPlant != nullptr && !al::isAlive(powerPlant)) {
        GameDataHolderAccessor accessor(this);
        if (GameDataFunction::getSessionEventProgress(accessor).value() >=
            SessionEventProgress::WaitThePowerPlantWorks)
            powerPlant->appear();
    }
    return;
}

void SessionMusicianManager::exeComplete() {
    if (mBgmController != nullptr)
        mBgmController->updateNerve();
}

namespace SessionMusicianLocalFunction {

void tryCreateSessionMusicianManager(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_SessionMusicianManager))
        return;

    al::setSceneObj(holder, new SessionMusicianManager("セッション[マネージャー]"));
}

SessionMusicianManager* getSessionMusicianManager(const al::IUseSceneObjHolder* holder) {
    return al::tryGetSceneObj<SessionMusicianManager>(holder);
}

bool isExistSessionMusicianManager(const al::IUseSceneObjHolder* holder) {
    return al::isExistSceneObj(holder, SceneObjID_SessionMusicianManager);
}

bool tryStartWarpToSessionMayor(const al::IUseSceneObjHolder* holder,
                                al::PlacementInfo* placementInfo) {
    if (!al::isExistSceneObj(holder, SceneObjID_SessionMusicianManager))
        return false;

    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    return manager->tryStartWarp(placementInfo);
}

void entrySessionMayorToManager(SessionMayorNpc* mayor) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(mayor);
    manager->setSessionMayor(mayor);
}

bool isJoinedSessionMusician(const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (manager == nullptr)
        return false;

    const sead::PtrArray<SessionMusicianNpc>& musicians = manager->getMusicians();
    if (musicians.size() == 0)
        return false;

    SessionMusicianNpc** musician = musicians.data();
    while (true) {
        if ((*musician++)->isJoined())
            return true;

        if (musician == &musicians.data()[musicians.size()])
            break;
    }

    return false;
}

SessionMusicianNpc* tryGetJoinedSessionMusicanActor(const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (manager == nullptr)
        return nullptr;

    const sead::PtrArray<SessionMusicianNpc>& musicians = manager->getMusicians();
    if (musicians.size() == 0)
        return nullptr;

    SessionMusicianNpc** musician = musicians.data();
    while (true) {
        if ((*musician)->isJoined())
            return *musician;

        musician++;
        if (musician == &musicians.data()[musicians.size()])
            break;
    }

    return nullptr;
}

bool tryAddJoinedSessionMusicianDemoActor(const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    bool result = false;
    if (manager == nullptr)
        return result;

    const sead::PtrArray<SessionMusicianNpc>& musicians = manager->getMusicians();
    if (musicians.size() == 0)
        return false;

    SessionMusicianNpc** musicianPtr = musicians.data();
    while (true) {
        if ((*musicianPtr)->isJoined())
            break;

        musicianPtr++;
        if (musicianPtr == &musicians.data()[musicians.size()])
            return false;
    }

    SessionMusicianNpc* musician = *musicianPtr;
    if (musician == nullptr)
        return false;

    rs::addDemoActor(musician, true);
    result = true;
    if (musician->getDemoActorNum() < 1)
        return result;

    s64 demoActorIndex = 0;
    result = true;
    do {
        rs::addDemoActor(musician->tryGetDemoActor(demoActorIndex), true);
        demoActorIndex++;
    } while (demoActorIndex < musician->getDemoActorNum());

    return result;
}

bool tryGetSessionMoonGetDemoPlayerPos(sead::Vector3f* out, const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (manager == nullptr)
        return false;

    const sead::PtrArray<SessionMusicianNpc>& musicians = manager->getMusicians();
    if (musicians.size() == 0)
        return false;

    SessionMusicianNpc** musicianPtr = musicians.data();
    while (true) {
        if ((*musicianPtr)->isJoined())
            break;

        musicianPtr++;
        if (musicianPtr == &musicians.data()[musicians.size()])
            return false;
    }

    SessionMusicianNpc* musician = *musicianPtr;
    if (musician == nullptr)
        return false;

    new (out) sead::Vector3f(musician->getMoonGetDemoPlayerPos());
    return true;
}

bool tryGetSessionMoonGetDemoPlayerPose(sead::Quatf* out, const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (manager == nullptr)
        return false;

    const sead::PtrArray<SessionMusicianNpc>& musicians = manager->getMusicians();
    if (musicians.size() == 0)
        return false;

    SessionMusicianNpc** musicianPtr = musicians.data();
    while (true) {
        if ((*musicianPtr)->isJoined())
            break;

        musicianPtr++;
        if (musicianPtr == &musicians.data()[musicians.size()])
            return false;
    }

    SessionMusicianNpc* musician = *musicianPtr;
    if (musician == nullptr)
        return false;

    new (out) sead::Quatf(musician->getMoonGetDemoPlayerPose());
    return true;
}

bool trySetJoinedSessionMusicianTransformForMoonGetDemo(const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (manager == nullptr)
        return false;

    const sead::PtrArray<SessionMusicianNpc>& musicians = manager->getMusicians();
    if (musicians.size() == 0)
        return false;

    SessionMusicianNpc** musicianPtr = musicians.data();
    while (true) {
        if ((*musicianPtr)->isJoined())
            break;

        musicianPtr++;
        if (musicianPtr == &musicians.data()[musicians.size()])
            return false;
    }

    SessionMusicianNpc* musician = *musicianPtr;
    if (musician == nullptr)
        return false;

    sead::Vector3f pos = musician->getMoonGetDemoPlayerPos();
    al::faceToTarget(musician, pos);
    return true;
}

void addDemoAllMusicians(const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    manager->addDemoAllMusicians();
}

}  // namespace SessionMusicianLocalFunction
