#pragma once

class CollisionShapeInfoBase;

class CollidedShapeResult {
public:
    CollidedShapeResult(const CollisionShapeInfoBase*);

    void operator=(const CollidedShapeResult&);
private:
    void* size[0x1e8/8];
};
static_assert(sizeof(CollidedShapeResult) == 0x1e8);
