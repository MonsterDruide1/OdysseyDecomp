#include "MapObj/HintRouteGuidePoint.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/RouteGuideDirector.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(HintRouteGuidePoint, Start)
NERVE_IMPL(HintRouteGuidePoint, End)
NERVE_IMPL(HintRouteGuidePoint, HintNpc)
NERVE_IMPL(HintRouteGuidePoint, Home)
NERVE_IMPL(HintRouteGuidePoint, Dedication)
NERVE_IMPL(HintRouteGuidePoint, NearHint)

NERVES_MAKE_NOSTRUCT(HintRouteGuidePoint, Start, End, HintNpc, Home, Dedication, NearHint)
}  // namespace

HintRouteGuidePoint::HintRouteGuidePoint(const char* name)
    : al::LiveActor(name), RouteGuidePoint() {}

void HintRouteGuidePoint::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);

    if (rs::isKidsMode(this)) {
        rs::addRouteGuidePointBufferCount(this, 1);
        al::initExecutorWatchObj(this, info);
        al::initNerve(this, &Start, 0);
        makeActorAlive();
        return;
    }

    makeActorDead();
}

void HintRouteGuidePoint::initAfterPlacement() {
    rs::registerRouteGuidePoint(this, this);
}

const sead::Vector3f& HintRouteGuidePoint::getGuideTrans() const {
    return mGuideTrans;
}

bool HintRouteGuidePoint::isValidateGuide() const {
    if (al::isNerve(this, &Start))
        return false;

    if (al::isNerve(this, &End))
        return false;

    return RouteGuidePoint::isValidateGuide();
}

bool HintRouteGuidePoint::isHorizontalGuide() const {
    return al::isNerve(this, &HintNpc) || al::isNerve(this, &Home) ||
           al::isNerve(this, &Dedication);
}

void HintRouteGuidePoint::exeStart() {
    if (tryGuideEnd())
        return;

    if (tryGuideHome())
        return;

    if (tryGuideNearHint())
        return;

    if (tryGuideDedication())
        return;

    al::setNerve(this, &HintNpc);
}

bool HintRouteGuidePoint::tryGuideEnd() {
    if (!GameDataFunction::isUnlockedNextWorld(GameDataHolderAccessor(this)))
        return false;

    al::setNerve(this, &End);
    return true;
}

bool HintRouteGuidePoint::tryGuideHome() {
    if (!GameDataFunction::isCollectShineForNextWorldAndNotUnlockNextWorld(this))
        return false;

    al::setNerve(this, &Home);
    return true;
}

bool HintRouteGuidePoint::tryGuideNearHint() {
    if (!GameDataFunction::checkExistHint(GameDataHolderAccessor(this)))
        return false;

    al::setNerve(this, &NearHint);
    return true;
}

bool HintRouteGuidePoint::tryGuideDedication() {
    if (GameDataFunction::getCurrentShineNum(GameDataHolderAccessor(this)) < 1)
        return false;

    al::setNerve(this, &Dedication);
    return true;
}

void HintRouteGuidePoint::exeEnd() {}

void HintRouteGuidePoint::exeHome() {
    u64 homeAccessorStorage;
    u64 nextWorldAccessorStorage;

    if (al::isFirstStep(this)) {
        GameDataHolderAccessor* accessor =
            new (&nextWorldAccessorStorage) GameDataHolderAccessor(this);
        mGuideTrans = GameDataFunction::getHomeTrans(*accessor);
    }

    GameDataHolderAccessor* accessor = new (&homeAccessorStorage) GameDataHolderAccessor(this);
    if (GameDataFunction::isUnlockedNextWorld(*accessor))
        al::setNerve(this, &End);
}

void HintRouteGuidePoint::exeNearHint() {
    if (tryGuideEnd())
        return;

    if (tryGuideHome())
        return;

    if (!updateNearHint())
        return;

    if (tryGuideDedication())
        return;

    al::setNerve(this, &HintNpc);
}

bool HintRouteGuidePoint::updateNearHint() {
    bool isNotFound = true;

    if (GameDataFunction::checkExistHint(GameDataHolderAccessor(this))) {
        isNotFound = !GameDataFunction::calcHintTransMostNear(
            &mGuideTrans, GameDataHolderAccessor(this), rs::getPlayerPos(this));
    }

    return isNotFound;
}

void HintRouteGuidePoint::exeDedication() {
    u64 laterAccessorStorage;
    u64 homeAccessorStorage;

    if (al::isFirstStep(this)) {
        GameDataHolderAccessor* accessor = new (&homeAccessorStorage) GameDataHolderAccessor(this);
        mGuideTrans = GameDataFunction::getHomeTrans(*accessor);
    }

    GameDataHolderAccessor* nextWorldAccessor =
        new (&laterAccessorStorage) GameDataHolderAccessor(this);
    if (GameDataFunction::isUnlockedNextWorld(*nextWorldAccessor)) {
        al::setNerve(this, &End);
        return;
    }

    if (GameDataFunction::isCollectShineForNextWorldAndNotUnlockNextWorld(this)) {
        al::setNerve(this, &Home);
        return;
    }

    GameDataHolderAccessor* hintAccessor = new (&laterAccessorStorage) GameDataHolderAccessor(this);
    if (GameDataFunction::checkExistHint(*hintAccessor)) {
        al::setNerve(this, &NearHint);
        return;
    }

    GameDataHolderAccessor* shineAccessor =
        new (&laterAccessorStorage) GameDataHolderAccessor(this);
    if (GameDataFunction::getCurrentShineNum(*shineAccessor) > 0)
        return;

    al::setNerve(this, &HintNpc);
}

bool HintRouteGuidePoint::isEndDedication() const {
    return GameDataFunction::getCurrentShineNum(GameDataHolderAccessor(this)) < 1;
}

void HintRouteGuidePoint::exeHintNpc() {
    u64 laterAccessorStorage;
    u64 hintNpcAccessorStorage;

    if (al::isFirstStep(this)) {
        GameDataHolderAccessor* accessor =
            new (&hintNpcAccessorStorage) GameDataHolderAccessor(this);
        mGuideTrans = GameDataFunction::getHintNpcTrans(*accessor);
    }

    GameDataHolderAccessor* nextWorldAccessor =
        new (&laterAccessorStorage) GameDataHolderAccessor(this);
    if (GameDataFunction::isUnlockedNextWorld(*nextWorldAccessor)) {
        al::setNerve(this, &End);
        return;
    }

    if (GameDataFunction::isCollectShineForNextWorldAndNotUnlockNextWorld(this)) {
        al::setNerve(this, &Home);
        return;
    }

    GameDataHolderAccessor* hintAccessor = new (&laterAccessorStorage) GameDataHolderAccessor(this);
    if (GameDataFunction::checkExistHint(*hintAccessor)) {
        al::setNerve(this, &NearHint);
        return;
    }

    GameDataHolderAccessor* shineAccessor =
        new (&laterAccessorStorage) GameDataHolderAccessor(this);
    if (GameDataFunction::getCurrentShineNum(*shineAccessor) >= 1) {
        al::setNerve(this, &Dedication);
        return;
    }
}
