#pragma once

#include <container/seadObjArray.h>

namespace al {
struct ArrowHitInfo;
struct SphereHitInfo;
struct DiskHitInfo;

using ArrowHitResultBuffer = sead::ObjArray<ArrowHitInfo>;
using SphereHitResultBuffer = sead::ObjArray<SphereHitInfo>;
using DiskHitResultBuffer = sead::ObjArray<DiskHitInfo>;

}  // namespace al
