#include "MapObj/TouchActionMessageDirector.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Screen/ScreenPointer.h"

#include "MapObj/ScreenPointAnalyzer.h"

TouchActionMessageDirector::TouchActionMessageDirector() {}

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
    return mScreenPointAnalyzer->mRotateSpeedDegree;
}

bool TouchActionMessageDirector::tryGetMoveVec(sead::Vector2f* out) const {
    if (!mScreenPointAnalyzer->isHold())
        return false;
    const sead::Vector2f* moveVec = &mScreenPointAnalyzer->mMoveVec;
    if (al::isNearZero(*moveVec, 0.001f))
        return false;
    out->e = moveVec->e;
    return true;
}

bool TouchActionMessageDirector::tryGetSlideDir(sead::Vector2f* out) const {
    if (!mScreenPointAnalyzer->isSlide())
        return false;
    out->e = mScreenPointAnalyzer->mSlideDir.e;
    return true;
}

const char* TouchActionMessageDirector::getSceneObjName() const {
    return "タッチアクションメッセージディレクター";
}

al::CollisionDirector* TouchActionMessageDirector::getCollisionDirector() const {
    return mCollisionDirector;
}

namespace rs {

bool isTimeHandled(const al::LiveActor* actor) {
    auto* obj = static_cast<TouchActionMessageDirector*>(al::getSceneObj(actor, 66));
    return obj->isTouchHold();
}

f32 getTimeHandleRotateSpeedDegree(const al::LiveActor* actor) {
    auto* obj = static_cast<TouchActionMessageDirector*>(al::getSceneObj(actor, 66));
    return obj->getRotateSpeedDegree();
}

bool tryGetTouchActionMoveVec(sead::Vector2f* out, const al::LiveActor* actor) {
    auto* obj = static_cast<TouchActionMessageDirector*>(al::getSceneObj(actor, 66));
    return obj->tryGetMoveVec(out);
}

bool tryGetTouchActionSlideDir(sead::Vector2f* out, const al::LiveActor* actor) {
    auto* obj = static_cast<TouchActionMessageDirector*>(al::getSceneObj(actor, 66));
    return obj->tryGetSlideDir(out);
}

}  // namespace rs
