#include "Library/Light/DirectionParam.h"

#include <math/seadQuat.h>
#include <utility/aglParameter.h>
#include <utility/aglParameterObj.h>

#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Yaml/ParameterBase.h"
#include "Library/Yaml/ParameterObj.h"

namespace al {

void DirectionParam::initByArg(const ActorInitInfo& initInfo) {
    f32 longitudeDegree = 0.0f;
    f32 latitudeDegree = 0.0f;
    tryGetArg(&longitudeDegree, initInfo, "DirectionParamLongitudeDegree");
    tryGetArg(&latitudeDegree, initInfo, "DirectionParamLatitudeDegree");

    sead::Vector2f coordinate = {sead::Mathf::deg2rad(longitudeDegree),
                                 sead::Mathf::deg2rad(latitudeDegree)};
    mAglCoordinate = new agl::utl::Parameter<sead::Vector2f>(
        coordinate, "PlacementArg", "配置情報からのパラメータ", new agl::utl::ParameterObj);
    syncToDirection();
}

void DirectionParam::syncToDirection() {
    mDirection.y = -sead::Mathf::sin(getLatitudeRadian());
    f32 rho = -sead::Mathf::cos(getLatitudeRadian());

    mDirection.x = sead::Mathf::sin(getLongitudeRadian()) * rho;
    mDirection.z = sead::Mathf::cos(getLongitudeRadian()) * rho;
}

void DirectionParam::initializeDir(agl::utl::ParameterObj* parameterObj, const char* name,
                                   const char* label) {
    mAglCoordinate =
        new agl::utl::Parameter<sead::Vector2f>(sead::Vector2f::zero, name, label, parameterObj);
    syncToDirection();
}

void DirectionParam::initializeDir(const sead::Vector3f& direction,
                                   agl::utl::ParameterObj* parameterObj, const char* name,
                                   const char* label) {
    mAglCoordinate =
        new agl::utl::Parameter<sead::Vector2f>(sead::Vector2f::zero, name, label, parameterObj);
    syncFromDirection(direction);
}

void DirectionParam::syncFromDirection(const sead::Vector3f& direction) {
    mDirection.set(direction);
    syncFromDirection();
}

void DirectionParam::initializeDir(ParameterObj* parameterObj, const char* name,
                                   const char* label) {
    mAlCoordinate = new ParameterV2f(sead::Vector2f::zero, name, label, "", parameterObj, true);
    syncToDirection();
}

void DirectionParam::initializeDir(const sead::Vector3f& direction, ParameterObj* parameterObj,
                                   const char* name, const char* label) {
    mAlCoordinate = new ParameterV2f(sead::Vector2f::zero, name, label, "", parameterObj, true);
    syncFromDirection(direction);
}

f32 DirectionParam::getLongitudeRadian() const {
    if (mAlCoordinate)
        return (*mAlCoordinate)->x;
    if (mAglCoordinate)
        return (*mAglCoordinate)->x;
    return 0.0f;
}

f32 DirectionParam::getLatitudeRadian() const {
    if (mAlCoordinate)
        return (*mAlCoordinate)->y;
    if (mAglCoordinate)
        return (*mAglCoordinate)->y;
    return 0.0f;
}

f32* DirectionParam::getLongitudeRadianPtr() {
    if (mAlCoordinate)
        return &(*mAlCoordinate)->x;
    if (mAglCoordinate)
        return &(*mAglCoordinate)->x;
    return nullptr;
}

f32* DirectionParam::getLatitudeRadianPtr() {
    if (mAlCoordinate)
        return &(*mAlCoordinate)->y;
    if (mAglCoordinate)
        return &(*mAglCoordinate)->y;
    return nullptr;
}

void DirectionParam::setLongitudeRadian(f32 radian) {
    if (mAlCoordinate)
        (*mAlCoordinate)->x = radian;
    else if (mAglCoordinate)
        (*mAglCoordinate)->x = radian;
}

void DirectionParam::setLatitudeRadian(f32 radian) {
    if (mAlCoordinate)
        (*mAlCoordinate)->y = radian;
    else if (mAglCoordinate)
        (*mAglCoordinate)->y = radian;
}

void DirectionParam::syncFromDirection() {
    sead::Vector3f direction = mDirection;
    if (!(direction.normalize() > 0.0f))
        return;

    f32 length = sead::Mathf::sqrt(direction.z * direction.z + direction.x * direction.x);

    if (length > 0.0f) {
        f32 inv = 1.0f / length;
        f32 x = -direction.z * inv;
        f32 y = -direction.x * inv;
        setLongitudeRadian(sead::Mathf::atan2(y, x));
    }

    setLatitudeRadian(sead::Mathf::asin(sead::Mathf::clamp(-direction.y, -1.0f, 1.0f)));
}

void DirectionParam::syncFromRPYDegree(const sead::Vector3f& degreeRPY) {
    sead::Quatf quat;
    quat.setRPY(sead::Mathf::deg2rad(degreeRPY.x), sead::Mathf::deg2rad(degreeRPY.y),
                sead::Mathf::deg2rad(degreeRPY.z));

    sead::Vector3f direction;
    calcQuatUp(&direction, quat);
    syncFromDirection(-direction);
}

void DirectionParam::lerp(const DirectionParam& startDirection, const DirectionParam& endDirection,
                          f32 rate) {
    lerpVec(&mDirection, startDirection.getDirection(), endDirection.getDirection(), rate);
    if (!tryNormalizeOrZero(&mDirection))
        mDirection.set(endDirection.getDirection());
    syncFromDirection();
}

}  // namespace al
