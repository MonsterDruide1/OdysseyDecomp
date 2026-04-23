#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

class ScreenPointAnalyzer;

namespace al {
struct ActorInitInfo;
class CollisionDirector;
class LiveActor;
class ScreenPointer;
}  // namespace al

class TouchActionMessageDirector : public al::HioNode,
                                   public al::ISceneObj,
                                   public al::IUseCollision {
public:
    static constexpr s32 sSceneObjId = SceneObjID_TouchActionMessageDirector;

    TouchActionMessageDirector();
    void init(const al::ActorInitInfo& info);
    void update();
    bool isTouchHold() const;
    f32 getRotateSpeedDegree() const;
    bool tryGetMoveVec(sead::Vector2f* out) const;
    bool tryGetSlideDir(sead::Vector2f* out) const;

    const char* getSceneObjName() const override {
        return "タッチアクションメッセージディレクター";
    }

    al::CollisionDirector* getCollisionDirector() const override { return mCollisionDirector; }

private:
    al::CollisionDirector* mCollisionDirector = nullptr;
    ScreenPointAnalyzer* mScreenPointAnalyzer = nullptr;
    al::ScreenPointer* mScreenPointer = nullptr;
};

namespace rs {
bool isTimeHandled(const al::LiveActor* actor);
f32 getTimeHandleRotateSpeedDegree(const al::LiveActor* actor);
bool tryGetTouchActionMoveVec(sead::Vector2f* out, const al::LiveActor* actor);
bool tryGetTouchActionSlideDir(sead::Vector2f* out, const al::LiveActor* actor);
}  // namespace rs
