#pragma once

#include <math/seadVector.h>

namespace al {

bool checkHitSegmentSphere(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                           float, sead::Vector3f*, sead::Vector3f*);

}
