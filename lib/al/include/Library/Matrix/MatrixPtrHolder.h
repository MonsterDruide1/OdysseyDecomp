#pragma once

#include <math/seadMatrix.h>

namespace al {
void makeBayerMatrix(s32*, s32);
u32 f32ToF16(f32);
f32 f16ToF32(u16);

class MtxPtrHolder {
public:
    MtxPtrHolder();

    void init(s32);
    void setMtxPtrAndName(s32, const char*, const sead::Matrix34f*);
    void setMtxPtr(const char*, const sead::Matrix34f*);
    s32 findIndex(const char*) const;
    sead::Matrix34f* findMtxPtr(const char*) const;
    sead::Matrix34f* tryFindMtxPtr(const char*) const;
    s32 tryFindIndex(const char*) const;
};
}  // namespace al
