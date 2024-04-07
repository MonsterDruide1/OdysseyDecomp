#include "Library/Math/VectorUtil.h"

namespace al {

void parallelizeVec(sead::Vector3f* out, const sead::Vector3f& parallel, const sead::Vector3f& vec) {
    f32 factor = parallel.dot(vec);
    out->x = parallel.x * factor;
    out->y = parallel.y * factor;
    out->z = parallel.z * factor;
}

void calcVectorSeparateHV(sead::Vector3f* out, const sead::Vector3f& a1, const sead::Vector3f& a2, float a3, float a4) {
    f32 factor = a2.dot(a1);
    sead::Vector3f x2;
    x2.x = a1.x - (a2.x * factor);
    x2.y = a1.y - (a2.y * factor);
    x2.z = a1.z - (a2.z * factor);

    if(x2.squaredLength() < 0.000001f) {
        x2.x = 0;
        x2.y = 0;
        x2.z = 0;
    } else {
        x2.normalize();
    }
    *out = (x2 * a3) + (a2 * a4);
}

void separateVectorParallelVertical(sead::Vector3f* out1, sead::Vector3f* out2, const sead::Vector3f& a1, const sead::Vector3f& a2) {
    f32 factor = a1.dot(a2);
    out1->x = a1.x * factor;
    out1->y = a1.y * factor;
    out1->z = a1.z * factor;
    out2->x = a2.x - out1->x;
    out2->y = a2.y - out1->y;
    out2->z = a2.z - out1->z;
}

void alongVectorNormalH(sead::Vector3f* out, const sead::Vector3f& a2p, const sead::Vector3f& a3, const sead::Vector3f& a4) {
    const sead::Vector3f a2 = a2p;
    sead::Vector3f x1 = a3.cross(a4);
    f32 sqLenX1 = x1.squaredLength();
    if (sqLenX1 < 0.000001f) {
        f32 factor = a4.dot(a2p);
        out->x = a2p.x - (a4.x * factor);
        out->y = a2p.y - (a4.y * factor);
        out->z = a2p.z - (a4.z * factor);
        //sead::Vector3f nul;
        //separateVectorParallelVertical(&nul, out, a4, a2p);
        return;
    }

    sead::Vector3f nul;
    sead::Vector3f x2;
    separateVectorParallelVertical(&nul, &x2, a2, a3);
    //sead::Vector3f x2 = a2 - (a3 * a2.dot(a3));

    x1.normalize();

    f32 v35 = a3.dot(a4);
    f32 v39 = sead::Mathf::atan2(a3.cross(a4).length(), v35) * 0.5f;

    f32 cos = sead::Mathf::cos(v39);
    f32 sin = sead::Mathf::sin(v39);
    sead::Vector3f x5 = sin*x1;

    sead::Vector3f x6 = (cos*x2) + x2.cross(x5);
    sead::Vector3f x8 = (cos*x6) + x6.cross(x5);
    sead::Vector3f x7;
    parallelizeVec(&x7, -x5, x2);
    *out = x7 + x8;
}

}
