#include "Layout/MapSeaOfTreeIcon.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(MapSeaOfTreeIcon, Wait);
NERVE_IMPL(MapSeaOfTreeIcon, Appear);
NERVE_IMPL(MapSeaOfTreeIcon, End);
NERVE_IMPL(MapSeaOfTreeIcon, Dead);

NERVES_MAKE_NOSTRUCT(MapSeaOfTreeIcon, Wait, Appear, End, Dead);

inline void preserveIconTypeGuardOrder(s32& nextIconType, s32& iconTypeNext) {
    asm("" : "+r"(nextIconType), "+r"(iconTypeNext));
}
}  // namespace

MapSeaOfTreeIcon::MapSeaOfTreeIcon(const al::LayoutInitInfo& initInfo) : al::LayoutActor("[") {
    initNerve(&Wait);
    al::initLayoutActor(this, initInfo, "MapIconUnclear");
    kill();
}

void MapSeaOfTreeIcon::appearHint0Mode() {
    appear();
    al::startAction(this, "Hint2", "State");
    al::setNerve(this, &Appear);
}

void MapSeaOfTreeIcon::appearHint1Mode() {
    appear();
    al::startAction(this, "Hint2", "State");
    al::setNerve(this, &Appear);
}

void MapSeaOfTreeIcon::appearHint2Mode() {
    appear();
    al::startAction(this, "Hint2", "State");
    al::setNerve(this, &Appear);
}

void MapSeaOfTreeIcon::end() {
    al::setNerve(this, &End);
}

bool MapSeaOfTreeIcon::trySetIconType(SeaOfTreeIconType iconType) {
    volatile s32 iconTypeLocal = iconType;
    s32 currentIconType = mCurrentIconType;
    s32 iconTypeCurrent = iconTypeLocal;

    if (currentIconType == iconTypeCurrent)
        return false;

    if (currentIconType == Value_2) {
        mCurrentIconType = iconTypeCurrent;
        return true;
    }

    s32 nextIconType = mNextIconType;
    s32 iconTypeNext = iconTypeLocal;

    preserveIconTypeGuardOrder(nextIconType, iconTypeNext);

    if (nextIconType == iconTypeNext)
        return false;

    if (nextIconType == Value_2) {
        mCurrentIconType = iconTypeNext;
        return true;
    }

    return false;
}

void MapSeaOfTreeIcon::resetIconType() {
    mCurrentIconType = Value_2;
    mNextIconType = Value_2;
}

void MapSeaOfTreeIcon::getHint1LayoutPos(sead::Vector3f* trans) const {
    *trans = al::getPaneLocalTrans(this, "Hint00");
}

void MapSeaOfTreeIcon::getHint2LayoutPos(sead::Vector3f* trans) const {
    *trans = al::getPaneLocalTrans(this, "Hint01");
}

void MapSeaOfTreeIcon::getPlayerLayoutPos(sead::Vector3f* trans) const {
    *trans = al::getPaneLocalTrans(this, "Player");
}

void MapSeaOfTreeIcon::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void MapSeaOfTreeIcon::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void MapSeaOfTreeIcon::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End");
    if (al::isActionEnd(this))
        al::setNerve(this, &Dead);
}

void MapSeaOfTreeIcon::exeDead() {
    kill();
}
