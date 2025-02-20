#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class AreaObj;

class AreaObjGroup {
public:
    AreaObjGroup(const char* name, s32 size);
    void incrementCount();
    void createBuffer();
    void createBuffer(s32 size);
    void registerAreaObj(AreaObj*);
    AreaObj* getAreaObj(s32 index) const;
    AreaObj* getInVolumeAreaObj(const sead::Vector3f&) const;

private:
    const char* mName;
    AreaObj** mAreaObjEntries;
    s32 mSize;
    s32 mMaxSize;
};

static_assert(sizeof(AreaObjGroup) == 0x18);

}  // namespace al
