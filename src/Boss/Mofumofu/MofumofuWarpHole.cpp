#include "Boss/Mofumofu/MofumofuWarpHole.h"

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerUtil.h"

namespace {

NERVE_IMPL(MofumofuWarpHole, Close);
NERVE_IMPL(MofumofuWarpHole, Disappear);
NERVE_IMPL(MofumofuWarpHole, Appear);
NERVE_IMPL(MofumofuWarpHole, HideMove);
NERVE_IMPL(MofumofuWarpHole, HideWait);
NERVE_IMPL(MofumofuWarpHole, Wait);
NERVE_IMPL(MofumofuWarpHole, DashSign);
NERVE_IMPL(MofumofuWarpHole, DashSignEnd);
NERVE_IMPL_(MofumofuWarpHole, CloseAndDisappear, Close);

NERVES_MAKE_NOSTRUCT(MofumofuWarpHole, Close, Disappear, Appear, HideMove, HideWait, Wait, DashSign,
                     DashSignEnd, CloseAndDisappear);

}  // namespace

MofumofuWarpHole::MofumofuWarpHole(const char* name)
    : al::LiveActor(name) {}  // TODO minor mismatch about storing `gap`

void MofumofuWarpHole::init(const al::ActorInitInfo& actorInitInfo) {
    al::initActorWithArchiveName(this, actorInitInfo, "MofumofuWarpHole", nullptr);
    al::initNerve(this, &Appear, 0);
    al::initJointControllerKeeper(this, 1);
    al::initJointGlobalQuatController(this, &gap, "DashSign");
    makeActorDead();
}

void MofumofuWarpHole::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &Appear);
}

void MofumofuWarpHole::disappear() {
    al::setNerve(this, &Disappear);
}

void MofumofuWarpHole::close() {
    al::setNerve(this, &Close);
}

void MofumofuWarpHole::closeAndDisappear() {
    al::setNerve(this, &CloseAndDisappear);
}

void MofumofuWarpHole::open() {
    al::setNerve(this, &Appear);
}

void MofumofuWarpHole::startHideMove() {
    al::setNerve(this, &HideMove);
}

void MofumofuWarpHole::startDashSign() {
    al::setNerve(this, &DashSign);
}

bool MofumofuWarpHole::isWait() const {
    return al::isNerve(this, &Wait);
}

bool MofumofuWarpHole::isHideWait() const {
    return al::isNerve(this, &HideWait);
}

void MofumofuWarpHole::calcDashSignFront(sead::Vector3f* front) const {
    al::calcJointFrontDir(front, this, "DashSign");
}

void MofumofuWarpHole::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");
    al::setNerveAtActionEnd(this, &Wait);
}

void MofumofuWarpHole::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void MofumofuWarpHole::exeDisappear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Disappear");
    if (!al::isEffectEmitting(this, "Disappear"))
        kill();
}

void MofumofuWarpHole::exeClose() {
    if (al::isFirstStep(this))
        al::startAction(this, "Disappear");

    if (al::isActionEnd(this)) {
        if (al::isNerve(this, &CloseAndDisappear))
            al::setNerve(this, &Disappear);
        else
            al::setNerve(this, &HideWait);
    }
}

void MofumofuWarpHole::exeHideWait() {}

void MofumofuWarpHole::exeHideMove() {
    if (al::isFirstStep(this))
        al::startAction(this, "Move");
}

void MofumofuWarpHole::exeDashSign() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "DashSign");
        al::calcQuat(&gap, this);
    }

    const sead::Vector3f& playerPos = rs::getPlayerPos(this);
    const sead::Vector3f& trans = al::getTrans(this);

    sead::Vector3f a3 = (playerPos) - (trans);
    al::verticalizeVec(&a3, al::getGravity(this), a3);
    if (!al::tryNormalizeOrZero(&a3))
        a3.set(al::getFront(this));

    al::turnVecToVecDegree(&a3, al::getFront(this), a3, 55);
    al::normalize(&a3);

    sead::Vector3f v21{0, 0, 0};
    al::calcQuatFront(&v21, gap);

    f32 v13 = al::calcNerveEaseInRate(this, 100, 120);
    f32 v14 = al::lerpValue(0.3, 0.05, v13);
    al::turnVecToVecRate(&a3, v21, a3, v14);
    al::normalize(&a3);
    al::turnVecToVecRate(&v21, v21, a3, 0.15);
    al::normalize(&v21);

    const sead::Vector3f& v15 = al::getGravity(this);
    sead::Vector3f v20{-v15.x, -v15.y, -v15.z};
    al::makeQuatFrontUp(&gap, v21, v20);
    al::setNerveAtGreaterEqualStep(this, &DashSignEnd, 120);
}

void MofumofuWarpHole::exeDashSignEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "DashSignEnd");
    al::setNerveAtActionEnd(this, &Wait);
}
