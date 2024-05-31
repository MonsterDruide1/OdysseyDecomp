#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/IUseCollision.h"
#include "CUSTOM/CollisionPartsKeeper.h"

namespace al {
class CollisionPartsFilterBase;
class CollisionParts;
class KCPrismData;
class KCPrismHeader;
}
class CollisionShapeKeeper;

class CollisionPartsKeeper;

namespace game {
class CollisionMultiShape {
public:
    //CollisionMultiShape(const CollisionPartsKeeper* keeper) : mPartsKeeper(keeper) {}
    CollisionMultiShape(const al::IUseCollision* part, int) : mPartsKeeper((game::CollisionPartsKeeper*)part->getCollisionDirector()->mActivePartsKeeper) {}

    bool check(CollisionShapeKeeper *,sead::Matrix34f const*,float,sead::Vector3f const&,al::CollisionPartsFilterBase const*);
    void callbackFromParts(al::CollisionParts*);
    void callbackFromServer(al::KCPrismData const*,al::KCPrismHeader const*);

private:
    const game::CollisionPartsKeeper* mPartsKeeper;
    sead::Vector3f mCheckPos = {0.0f, 0.0f, 0.0f};
    CollisionShapeKeeper* mCollisionShapeKeeper;
    int mCurrentShapeIndex;
    al::CollisionParts* mParts;
    f32 unk6;
};
}

class CollisionMultiShape : public game::CollisionMultiShape {
public:
    CollisionMultiShape(const al::IUseCollision* part, int s) : game::CollisionMultiShape(part, s) {}
};
