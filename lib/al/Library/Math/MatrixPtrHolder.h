#pragma once

#include <math/seadMatrix.h>

namespace al {

class MtxPtrHolder {
public:
    MtxPtrHolder();
    void init(s32 size);
    void setMtxPtrAndName(s32 index, const char* name, const sead::Matrix34f* mtx);
    void setMtxPtr(const char* name, const sead::Matrix34f* mtx);
    s32 findIndex(const char* name) const;
    const sead::Matrix34f* findMtxPtr(const char* name) const;
    const sead::Matrix34f* tryFindMtxPtr(const char* name) const;
    s32 tryFindIndex(const char* name) const;

private:
    const char** mNames = nullptr;
    const sead::Matrix34f** mMtxArray = nullptr;
    s32 mSize = 0;
};

static_assert(sizeof(MtxPtrHolder) == 0x18);

}  // namespace al
