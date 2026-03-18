#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/Scene/ISceneObj.h"

class ScreenPointAnalyzer;

namespace al {
struct ActorInitInfo;
class CollisionDirector;
class ScreenPointer;
}  // namespace al

class TouchActionMessageDirector : public al::ISceneObj, public al::IUseCollision {
public:
    TouchActionMessageDirector();
    void init(const al::ActorInitInfo& info);
    void update();
    bool isTouchHold() const;
    f32 getRotateSpeedDegree() const;
    bool tryGetMoveVec(sead::Vector2f* out) const;
    bool tryGetSlideDir(sead::Vector2f* out) const;

    const char* getSceneObjName() const override;
    ~TouchActionMessageDirector() override = default;
    al::CollisionDirector* getCollisionDirector() const override;

private:
    al::CollisionDirector* mCollisionDirector = nullptr;
    ScreenPointAnalyzer* mScreenPointAnalyzer = nullptr;
    al::ScreenPointer* mScreenPointer = nullptr;
};
