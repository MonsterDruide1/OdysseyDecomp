#pragma once

#include <math/seadVector.h>

namespace nn::g3d {
class ModelObj;
class ResSkeleton;
}  // namespace nn::g3d

namespace al {
class ByamlIter;

struct SklAnimRetargettingInfo {
    SklAnimRetargettingInfo(const nn::g3d::ModelObj*, const nn::g3d::ModelObj*,
                            const sead::Vector3f&);
    SklAnimRetargettingInfo(const nn::g3d::ResSkeleton*, const nn::g3d::ResSkeleton*,
                            const sead::Vector3f&);
    SklAnimRetargettingInfo(const nn::g3d::ResSkeleton*, const ByamlIter&, const char*,
                            const sead::Vector3f&);
    SklAnimRetargettingInfo(const ByamlIter&, const char*, const char*, const sead::Vector3f&);

private:
    void* _0;
    sead::Vector3f _8;
};

static_assert(sizeof(SklAnimRetargettingInfo) == 0x18);
}  // namespace al
