#include "Library/Collision/CollisionCheckInfo.h"

namespace al {

DiskCheckInfo::DiskCheckInfo(const sead::Vector3f& pos, f32 unk1, f32 unk2,
                             const sead::Vector3f& unk3)
    : CollisionCheckInfoBase(pos), unk1(unk1), unk2(unk2), unk3(unk3) {
    if (unk1 <= unk2)
        unk4 = sead::Mathf::sqrt(sead::Mathf::square(unk2) + sead::Mathf::square(unk1));
    else
        unk4 = unk1 * 1.4142f;
}

}
