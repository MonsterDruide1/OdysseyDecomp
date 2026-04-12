#include "MapObj/TouchActionMessageDirector.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Screen/ScreenPointer.h"

#include "MapObj/ScreenPointAnalyzer.h"

TouchActionMessageDirector::TouchActionMessageDirector() = default;

void TouchActionMessageDirector::init(const al::ActorInitInfo& info) {
    mCollisionDirector = al::getCollisionDirectorFromInfo(info);
    mScreenPointAnalyzer = new ScreenPointAnalyzer();
    mScreenPointer = new al::ScreenPointer(info, "No Owner");
}

void TouchActionMessageDirector::update() {
    mScreenPointAnalyzer->update();
}

bool TouchActionMessageDirector::isTouchHold() const {
    return mScreenPointAnalyzer->isHold();
}

f32 TouchActionMessageDirector::getRotateSpeedDegree() const {
    return mScreenPointAnalyzer->getRotateSpeedDegree();
}

bool TouchActionMessageDirector::tryGetMoveVec(sead::Vector2f* out) const {
    if (!isTouchHold())
        return false;
    const sead::Vector2f& moveVec = mScreenPointAnalyzer->getMoveVec();
    if (al::isNearZero(moveVec))
        return false;

    out->set(moveVec);
    return true;
}

bool TouchActionMessageDirector::tryGetSlideDir(sead::Vector2f* out) const {
    if (!mScreenPointAnalyzer->isSlide())
        return false;
    out->set(mScreenPointAnalyzer->getSlideDir());
    return true;
}

namespace rs {

bool isTimeHandled(const al::LiveActor* actor) {
    TouchActionMessageDirector* director = al::getSceneObj<TouchActionMessageDirector>(actor);
    return director->isTouchHold();
}

f32 getTimeHandleRotateSpeedDegree(const al::LiveActor* actor) {
    TouchActionMessageDirector* director = al::getSceneObj<TouchActionMessageDirector>(actor);
    return director->getRotateSpeedDegree();
}

bool tryGetTouchActionMoveVec(sead::Vector2f* out, const al::LiveActor* actor) {
    TouchActionMessageDirector* director = al::getSceneObj<TouchActionMessageDirector>(actor);
    return director->tryGetMoveVec(out);
}

bool tryGetTouchActionSlideDir(sead::Vector2f* out, const al::LiveActor* actor) {
    TouchActionMessageDirector* director = al::getSceneObj<TouchActionMessageDirector>(actor);
    return director->tryGetSlideDir(out);
}

}  // namespace rs
