#include "Util/ScenePlayerCapFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/Controller/InputFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"

#include "Player/CapTargetInfo.h"
#include "Player/HackCap.h"
#include "Player/PlayerActorBase.h"
#include "Player/PlayerHackKeeper.h"
#include "Player/PlayerInfo.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataHolderWriter.h"
#include "Util/PlayerCollisionUtil.h"

namespace al {
PlayerActorBase* getPlayerActor(const LiveActor* actor, s32 index);
}

namespace {
const char* sHackTypeNames[] = {
    "",
    "RigidBodyBox",
    "Kuribo",
    "Statue",
    "HammerBros",
    "Bull",
    "KaronWing",
    "",
    "Megane",
    "",
    "",
    "",
    "Frog",
    "",
    "ElectricWire",
    "RigidBodyBox",
    "CityManFlying",
    "TRex",
    "Pukupuku",
    "Kakku",
    "Fukankun",
    "Hosui",
    "Yukimaru",
    "Tsukkun",
    "KuriboWing",
    "BazookaElectric",
    "Bubble",
    "Tank",
    "KillerMagnum",
    "JugemFishing",
    "Radicon",
    "Senobi",
    "Yoshi",
    "Imomu",
    "Fastener",
    "Koopa",
    "PukupukuSnow",
};
}  // namespace

namespace PlayerCapFunction {

bool tryCalcHackCapThrowInputNormal(sead::Vector3f* out, const al::LiveActor* actor) {
    PlayerHackKeeper* hackKeeper = al::getPlayerActor(actor, 0)->getPlayerHackKeeper();
    if (hackKeeper && hackKeeper->getCurrentHackName()) {
        hackKeeper = al::getPlayerActor(actor, 0)->getPlayerHackKeeper();
        if (!hackKeeper)
            return false;

        if (hackKeeper->getHackSensor()) {
            if (hackKeeper->isHackUsePlayerCollision()) {
                if (rs::isCollidedGround(hackKeeper->getPlayerCollision())) {
                    out->e = rs::getCollidedGroundNormal(hackKeeper->getPlayerCollision()).e;
                    return true;
                }
            } else {
                const al::LiveActor* hackActor = hackKeeper->getHack();
                if (al::isExistActorCollider(hackActor) && al::isCollidedGround(hackActor)) {
                    out->e = al::getCollidedGroundNormal(hackActor).e;
                    return true;
                }
            }
        }
    } else {
        const IUsePlayerCollision* collision = al::getPlayerActor(actor, 0)->getPlayerCollision();
        if (!collision)
            return false;

        if (rs::isCollidedGround(collision)) {
            out->e = rs::getCollidedGroundNormal(collision).e;
            return true;
        }
    }

    return false;
}

s32 getCapPadRumblePort(const al::LiveActor* actor) {
    PlayerInfo* playerInfo = al::getPlayerActor(actor, 0)->getPlayerInfo();
    if (playerInfo) {
        HackCap* hackCap = playerInfo->getHackCap();
        if (hackCap)
            return hackCap->getPadRumblePort();
    }

    return al::getPlayerControllerPort(0);
}

bool isEnableBirdLandPlayerCapOn(const al::LiveActor* actor) {
    PlayerInfo* playerInfo = al::getPlayerActor(actor, 0)->getPlayerInfo();
    if (!playerInfo)
        return false;

    HackCap* hackCap = playerInfo->getHackCap();
    if (!hackCap)
        return false;

    if (hackCap->isEnableThrowSeparate())
        return false;

    return hackCap->isPutOn();
}

}  // namespace PlayerCapFunction

namespace InformationWindowFunction {

bool isShowPlayerHackHackTutorial(const al::LiveActor* actor, const char* suffix) {
    GameDataHolderAccessor accessor(actor);
    PlayerHackKeeper* hackKeeper = al::getPlayerActor(actor, 0)->getPlayerHackKeeper();
    const char* hackName = nullptr;
    if (hackKeeper)
        hackName = hackKeeper->getCurrentHackName();

    return GameDataFunction::isShowHackTutorial(accessor, hackName, suffix);
}

bool isShowPlayerBindTutorial(const al::LiveActor* actor, const BindInfo& bindInfo) {
    GameDataHolderAccessor accessor(actor);
    return GameDataFunction::isShowBindTutorial(accessor, bindInfo);
}

void setShowPlayerHackHackTutorial(const al::LiveActor* actor, const char* suffix) {
    GameDataHolderWriter writer(actor);
    PlayerHackKeeper* hackKeeper = al::getPlayerActor(actor, 0)->getPlayerHackKeeper();
    const char* hackName = nullptr;
    if (hackKeeper)
        hackName = hackKeeper->getCurrentHackName();

    GameDataFunction::setShowHackTutorial(writer, hackName, suffix);
}

}  // namespace InformationWindowFunction

namespace EventFlowFunction {

bool isCapTargetHackType(const CapTargetInfo* capTargetInfo, s32 hackType) {
    const char* hackName = capTargetInfo->getHackName();
    if (!hackName)
        return false;

    return al::isEqualString(sHackTypeNames[hackType], hackName);
}

}  // namespace EventFlowFunction
