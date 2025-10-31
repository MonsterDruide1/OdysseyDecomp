#pragma once

#include <gfx/seadCamera.h>
#include <math/seadVector.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/Nerve/NerveExecutor.h"

namespace al {
class CameraPoser;
class CollisionDirector;
class CollisionParts;
class LookAtCamera;

class CameraArrowCollider : public NerveExecutor, public IUseCollision {
public:
    CameraArrowCollider(CollisionDirector* director);

    CollisionDirector* getCollisionDirector() const override;

    void start();
    void update(const sead::Vector3f& vec0, const sead::Vector3f& vec1, const sead::Vector3f& vec2);
    void pushBackCollisionParts(CollisionParts* parts);
    void makeLookAtCamera(sead::LookAtCamera* cam) const;

    void exeKeep();
    void exeShrink();

    bool isShrink() const;
    f32 getPushLength() const;

    void set_48c(bool idx) { _48c = idx; }
private:
    char buffer[0x474];
    bool _48c;
    char buffer2[0x3];
};

static_assert(sizeof(CameraArrowCollider) == 0x490);
}  // namespace al
