#include "Util/ShadowUtil.h"

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Util/WaterSurfaceShadow.h"

namespace rs {

f32 setShadowDropLength(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                        const char* maskName) {
    f32 shadowLength = 1500.0f;
    if (!al::tryGetArg(&shadowLength, initInfo, "ShadowLength"))
        shadowLength = 1500.0f;
    bool isLongShadow = false;
    al::tryGetArg(&isLongShadow, initInfo, "IsLongShadow");
    if (isLongShadow)
        shadowLength = 10000.0f;
    if (!al::isNearZero(shadowLength, 0.001f))
        al::setShadowMaskDropLength(actor, shadowLength, maskName);
    else
        al::invalidateShadowMask(actor, maskName);
    return shadowLength;
}

// TODO: rename parameter f and f2
void calcShadowDropClippingCenter(sead::Vector3f* vec, const al::LiveActor* actor, f32 f, f32 f2) {
    vec->set(al::getTrans(actor) - (sead::Vector3f::ey * (f / 2.0f + f2 / 2.0f)));
}

void setShadowDropOffset(al::LiveActor* actor, const char* maskName, f32 distance) {
    al::setShadowMaskOffset(actor, sead::Vector3f::ey * distance * -0.5f, maskName);
}

WaterSurfaceShadow* tryCreateWaterSurfaceShadow(const al::ActorInitInfo& initInfo,
                                                const char* archiveName) {
    bool useWaterSurfaceShadow = false;
    al::tryGetArg(&useWaterSurfaceShadow, initInfo, "UseWaterSurfaceShadow");

    if (!useWaterSurfaceShadow)
        return nullptr;
    if (!archiveName)
        return new WaterSurfaceShadow(initInfo, "WaterSurfaceShadow");
    return new WaterSurfaceShadow(initInfo, archiveName);
}

WaterSurfaceShadow* tryCreateWaterSurfaceCoinShadow(const al::ActorInitInfo& initInfo) {
    return tryCreateWaterSurfaceShadow(initInfo, "WaterSurfaceShadowCoin");
}

void tryUpdateWaterSurfaceCoinShadow(WaterSurfaceShadow* shadow, al::LiveActor* actor,
                                     f32 distance) {
    if (shadow) {
        shadow->update(al::getTrans(actor), -al::getGravity(actor), distance);
        shadow->setQuat(al::getQuat(actor));
    }
}

void initHackShadow(al::LiveActor* actor) {
    invalidateDepthShadowMap(actor);
}

void setupHackShadow(al::LiveActor* actor) {
    al::invalidateShadowMask(actor);
    al::offDepthShadowModel(actor);
    al::validateDepthShadowMap(actor);
}

void endHackShadow(al::LiveActor* actor) {
    al::validateShadowMask(actor);
    al::onDepthShadowModel(actor);
    al::invalidateDepthShadowMap(actor);
}
}  // namespace rs
