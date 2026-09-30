#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {

class DynamicCollisionActor : public LiveActor {
public:
    DynamicCollisionActor(const ActorInitInfo& initInfo, s32 vertexNum,
                          const sead::Matrix34f* baseMtx, HitSensor* sensor,
                          const void* attributeByml);

    void create(s32 vertexNum);
    void begin();
    void vertex(const sead::Vector3f& position);
    void endData();
    void updateCollisionParts();
    void attribute(u16 attribute);
    void makePrism();
    void end();

private:
    u8 mDynamicCollisionActorStorage[0x70];
};

static_assert(sizeof(DynamicCollisionActor) == 0x178);

}  // namespace al
