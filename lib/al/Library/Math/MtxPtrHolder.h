#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

namespace al {

void makeBayerMatrix(s32* outMtx, s32 size);
u16 f32ToF16(f32 value);
f32 f16ToF32(u16 value);

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
