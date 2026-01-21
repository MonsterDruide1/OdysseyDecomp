#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace agl::utl {
template <typename T>
class Parameter;
class ParameterObj;
}  // namespace agl::utl

namespace al {
class ActorInitInfo;
class ParameterObj;
class ParameterV2f;

class DirectionParam {
public:
    DirectionParam() = default;

    void initByArg(const ActorInitInfo& initInfo);
    void syncToDirection();
    void initializeDir(agl::utl::ParameterObj* parameterObj, const char* name, const char* label);
    void initializeDir(const sead::Vector3f& direction, agl::utl::ParameterObj* parameterObj,
                       const char* name, const char* label);
    void syncFromDirection(const sead::Vector3f& direction);
    void initializeDir(ParameterObj* parameterObj, const char* name, const char* label);
    void initializeDir(const sead::Vector3f& direction, ParameterObj* parameterObj,
                       const char* name, const char* label);
    f32 getLongitudeRadian() const;
    f32 getLatitudeRadian() const;
    f32* getLongitudeRadianPtr();
    f32* getLatitudeRadianPtr();
    void setLongitudeRadian(f32 radian);
    void setLatitudeRadian(f32 radian);
    void syncFromDirection();
    void syncFromRPYDegree(const sead::Vector3f& degreeRPY);
    void lerp(const DirectionParam& startDirection, const DirectionParam& endDirection, f32 rate);

    const sead::Vector3f& getDirection() const { return mDirection; }

private:
    agl::utl::Parameter<sead::Vector2f>* mAglCoordinate = nullptr;
    ParameterV2f* mAlCoordinate = nullptr;
    sead::Vector3f mDirection = -sead::Vector3f::ey;
};

static_assert(sizeof(DirectionParam) == 0x20);

}  // namespace al
