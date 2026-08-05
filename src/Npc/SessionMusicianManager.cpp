#include "Npc/SessionMusicianManager.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Npc/SessionMayorNpc.h"
#include "Npc/SessionMusicianBgmController.h"
#include "Npc/SessionMusicianLocalFunction.h"
#include "Npc/SessionMusicianNpc.h"
#include "Npc/SessionMusicianType.h"
#include "Npc/SessionMusicianWarpAgent.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_HOST_TYPE_IMPL(SessionMusicianManager, Wait);
NERVE_HOST_TYPE_IMPL(SessionMusicianManager, Complete);

NERVES_MAKE_NOSTRUCT(HostType, Wait, Complete);
}  // namespace

SessionMusicianManager::SessionMusicianManager(const char* name) : al::LiveActor(name) {
    mMusicianPtrArray.allocBuffer(7, nullptr);
}

void SessionMusicianManager::initAfterPlacementSceneObj(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorWithArchiveName(this, info, "SessionMusicianManager", nullptr);
    al::initNerve(this, &Wait, 0);
    al::initActorSeKeeperWithout3D(this, info, "SessionMusicianManager");
    if (mMayorNpc != nullptr) {
        mMayorNpc->initIntroductionCamera(info, &mMusicianPtrArray);
        mBgmController = new SessionMusicianBgmController(this, info, false);
    }
    makeActorAlive();
    return;
}

void SessionMusicianManager::entryMusician(SessionMusicianNpc* musician) {
    if (mMusicianPtrArray.isFull())
        return;

    mMusicianPtrArray.pushBack(musician);
}

bool SessionMusicianManager::isJoinedMusician() const {
    if (mMusicianPtrArray.size() == 0)
        return false;

    for (auto it = mMusicianPtrArray.begin(); it != mMusicianPtrArray.end(); ++it)
        if (it->isJoined())
            return true;

    return false;
}

SessionMusicianNpc* SessionMusicianManager::getJoinedMusician() const {
    if (mMusicianPtrArray.size() == 0)
        return nullptr;

    for (auto it = mMusicianPtrArray.begin(); it != mMusicianPtrArray.end(); ++it)
        if (it->isJoined())
            return &(*it);

    return nullptr;
}

bool SessionMusicianManager::isSubscribed(SessionMusicianType type) const {
    if (mMusicianPtrArray.size() == 0)
        return false;

    for (auto it = mMusicianPtrArray.begin(); it != mMusicianPtrArray.end(); ++it) {
        if (SessionMusicianLocalFunction::isMusicianType(&(*it), type)) {
            if (SessionMusicianLocalFunction::isAlreadySessionMember(&(*it)))
                return true;
        }
    }

    return false;
}

bool SessionMusicianManager::tryAppearPowerPlant() {
    SessionMusicianNpc* musician = findPowerPlant();

    if (!musician)
        return false;

    if (al::isAlive(musician) || GameDataFunction::getSessionEventProgress(this) <
                                     SessionEventProgress::WaitThePowerPlantWorks)
        return false;

    musician->appear();
    return true;
}

SessionMusicianNpc* SessionMusicianManager::findPowerPlant() const {
    if (mMusicianPtrArray.size() == 0)
        return nullptr;

    for (auto it = mMusicianPtrArray.begin(); it != mMusicianPtrArray.end(); ++it) {
        SessionMusicianType type = SessionMusicianLocalFunction::getMusicianType(&(*it));
        if (type == SessionMusicianType::Vocal)
            return &(*it);
    }

    return nullptr;
}

bool SessionMusicianManager::tryStartWarp(al::PlacementInfo* info) {
    if (mMusicianPtrArray.size() == 0)
        return false;

    for (auto it = mMusicianPtrArray.begin(); it != mMusicianPtrArray.end(); ++it) {
        if (it->isStateWarp()) {
            SessionMusicianWarpAgent* warpAgent = it->getWarpAgent();
            if (warpAgent->tryGetWarpTargetInfo(info)) {
                if (warpAgent->tryStartWarp()) {
                    it->doneWarp();
                    al::invalidateClipping(mMayorNpc);
                    return true;
                }
            }
        }
    }

    return false;
}

void SessionMusicianManager::addDemoAllMusicians() {
    if (mMusicianPtrArray.size() == 0)
        return;

    for (auto it = mMusicianPtrArray.begin(); it != mMusicianPtrArray.end(); ++it)
        rs::addDemoActor(&(*it), 1);
}

void SessionMusicianManager::exeWait() {
    if (mBgmController)
        mBgmController->updateNerve();

    if (!(SessionMusicianLocalFunction::getMemberMusicianNum(this) <= 4))
        al::setNerve(this, &Complete);
    else
        tryAppearPowerPlant();
}

void SessionMusicianManager::exeComplete() {
    if (mBgmController != nullptr)
        mBgmController->updateNerve();
}

namespace SessionMusicianLocalFunction {

void tryCreateSessionMusicianManager(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SessionMusicianManager::sSceneObjId))
        return;

    SessionMusicianManager* manager = new SessionMusicianManager("SessionMusicianManager");
    al::setSceneObj(holder, manager, SessionMusicianManager::sSceneObjId);
}

SessionMusicianManager* getSessionMusicianManager(const al::IUseSceneObjHolder* holder) {
    return al::tryGetSceneObj<SessionMusicianManager>(holder);
}

bool isExistSessionMusicianManager(const al::IUseSceneObjHolder* holder) {
    return al::isExistSceneObj(holder, SessionMusicianManager::sSceneObjId);
}

bool tryStartWarpToSessionMayor(const al::IUseSceneObjHolder* holder, al::PlacementInfo* info) {
    if (!al::isExistSceneObj<SessionMusicianManager>(holder))
        return false;

    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);

    return manager->tryStartWarp(info);
}

void entrySessionMayorToManager(SessionMayorNpc* mayor) {
    al::tryGetSceneObj<SessionMusicianManager>(mayor)->entryMayor(mayor);
    return;
}

bool isJoinedSessionMusician(const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (!manager)
        return false;

    if (manager->isJoinedMusician())
        return true;

    return false;
}

SessionMusicianNpc* tryGetJoinedSessionMusicanActor(const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (!manager)
        return nullptr;

    return manager->getJoinedMusician();
}

bool tryAddJoinedSessionMusicianDemoActor(const al::IUseSceneObjHolder* holder) {
    SessionMusicianNpc* musician = tryGetJoinedSessionMusicanActor(holder);
    if (!musician)
        return false;

    rs::addDemoActor(musician, true);
    musician->addDemoActors();
    return true;
}

bool tryGetSessionMoonGetDemoPlayerPos(sead::Vector3f* pos, const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (!manager)
        return false;

    SessionMusicianNpc* musician = manager->getJoinedMusician();
    if (!musician)
        return false;

    pos->set(musician->getMoonGetDemoPlayerPos());
    return true;
}

bool tryGetSessionMoonGetDemoPlayerPose(sead::Quatf* pose, const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (!manager)
        return false;

    SessionMusicianNpc* musician = manager->getJoinedMusician();
    if (!musician)
        return false;

    pose->set(musician->getMoonGetDemoPlayerPose());
    return true;
}

bool trySetJoinedSessionMusicianTransformForMoonGetDemo(const al::IUseSceneObjHolder* holder) {
    SessionMusicianManager* manager = al::tryGetSceneObj<SessionMusicianManager>(holder);
    if (!manager)
        return false;

    SessionMusicianNpc* musician = manager->getJoinedMusician();
    if (!musician)
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
