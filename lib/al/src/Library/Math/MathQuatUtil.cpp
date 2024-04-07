#include "Library/Math/MathQuatUtil.h"

namespace al {

sead::Vector3f someSubOperation(const sead::Quatf& a1, const sead::Vector3f& a2) {
    return a1.vector().cross(a2) + a2*a1.scalar();
}

sead::Vector3f someOperation(const sead::Quatf& a1, const sead::Vector3f& a2) {
    sead::Vector3f x1 = someSubOperation(a1, a2);
    float x2 = a1.vector().dot(-a2);
    return someSubOperation(a1, x1) - x2*a1.vector();
}

bool turnQuatFrontToDirDegreeH(sead::Quatf* a1, const sead::Vector3f& a2, float a3) {
    sead::Vector3f x0 = a2;
    x0.y = 0.0f;
    if(x0.squaredLength() < 0.000001f) {
        x0 = {0.0f, 0.0f, 0.0f};
        return true;
    }

    x0.normalize();
    sead::Vector3f x3 = someOperation(*a1, sead::Vector3f::ez);

    if(!(
        x3.dot(a2) >= 0.0f ||
        sead::Mathf::abs(x3.cross(a2).x) > 0.01f ||
        sead::Mathf::abs(x3.cross(a2).y) > 0.01f ||
        sead::Mathf::abs(x3.cross(a2).z) > 0.01f
    )) {
        sead::Vector3f x6 = someOperation(*a1, sead::Vector3f::ex);
        x0 = a2 + x6 * 0.01f;
    }
    bool check = turnQuat(a1, *a1, x3, x0, sead::Mathf::deg2rad(a3));
    turnQuatYDirRate(a1, *a1, {0.0f, 1.0f, 0.0f}, 0.2f);
    return check;
}

}
