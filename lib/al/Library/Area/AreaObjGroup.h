#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class AreaObj;

class AreaObjGroup {
public:
    AreaObjGroup(const char* groupName, const s32 capacity);
    void createBuffer();
<<<<<<< HEAD
    void createBuffer(const s32 capacity);
    AreaObj* getAreaObj(const s32 index) const;
    AreaObj* getInVolumeAreaObj(const sead::Vector3f& position) const;
    void incrementCount();
    void registerAreaObj(AreaObj* newAreaObj);

    const char* getName() const { return mGroupName; };

    s32 getSize() const { return mCount; }

=======
    void createBuffer(s32 size);
    void registerAreaObj(AreaObj*);
    AreaObj* getAreaObj(s32 index) const;
    AreaObj* getInVolumeAreaObj(const sead::Vector3f&) const;
    s32 getSize() const { return mSize; }
>>>>>>> cae8a58c (uploaded because need to rebase other pr)
private:
    const char* mGroupName;
    AreaObj** mBuffer = nullptr;
    s32 mCount = 0;
    s32 mCapacity;
};

static_assert(sizeof(AreaObjGroup) == 0x18);

}  // namespace al
