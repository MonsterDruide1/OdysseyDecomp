#include "Stuff.h"

#include "Library/Collision/CollisionUtil.h"
#include "Library/Math/MathLengthUtil.h"

namespace rs {

bool isCollisionCodePress(al::HitInfo* info) {
    return al::isFloorCode(info, "Press");
}

bool calcExistCollisionBorder(const al::IUseCollision* a1, const sead::Vector3f& a2, const sead::Vector3f& a3) {
  float y; // s1
  float z; // s2
  float v6; // s4
  float v7; // s5
  float v8; // s0
  sead::Vector3f v10; // [xsp+0h] [xbp-50h] BYREF
  sead::Vector3f v11; // [xsp+10h] [xbp-40h] BYREF
  sead::Vector3f v12; // [xsp+20h] [xbp-30h] BYREF
  sead::Vector3f v13; // [xsp+30h] [xbp-20h] BYREF

  v13.x = 0.0;
  v13.y = 0.0;
  v13.z = 0.0;
  v12.x = 0.0;
  v12.y = 0.0;
  v12.z = 0.0;
  y = a3.y;
  z = a3.z;
  v6 = a2.y - y;
  v7 = a2.z - z;
  v8 = a3.x + a3.x;
  v11.x = a2.x - a3.x;
  v11.y = v6;
  v11.z = v7;
  v10.x = v8;
  v10.y = y + y;
  v10.z = z + z;
  return (alCollisionUtil::getHitPosAndNormalOnArrow(a1, &v13, &v12, v11, v10, 0LL, 0LL) & 1) != 0
      && al::isReverseDirection(v12, a3, 0.01);
}

}  // namespace rs
