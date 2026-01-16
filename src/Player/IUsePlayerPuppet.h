#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class HitSensor;
}  // namespace al

class IUsePlayerPuppet {
public:
    virtual void start(al::HitSensor*, al::HitSensor*) = 0;
    virtual void end() = 0;
    virtual void cancel() = 0;
    virtual void setTrans(const sead::Vector3f&) = 0;
    virtual void setPose(const sead::Quatf&) = 0;
    virtual void setVelocity(const sead::Vector3f&) = 0;
    virtual void resetPosition(const sead::Vector3f&) = 0;
    virtual const sead::Vector3f& getTrans() const = 0;
    virtual const sead::Vector3f& getVelocity() const = 0;
    virtual const sead::Vector3f& getGravity() const = 0;
    virtual void calcFront(sead::Vector3f*) const = 0;
    virtual void calcUp(sead::Vector3f*) const = 0;
    virtual void startAction(const sead::SafeString&) const = 0;
    virtual bool isActionEnd() const = 0;
    virtual bool isActionPlaying(const char*) const = 0;
    virtual void setAnimRate(f32) const = 0;
    virtual f32 getAnimFrameMax() const = 0;
    virtual void hide() = 0;
    virtual void show() = 0;
    virtual bool isHidden() const = 0;
    virtual void hideSilhouette() = 0;
    virtual void showSilhouette() = 0;
    virtual void hideShadow() = 0;
    virtual void showShadow() = 0;
    virtual void requestDamage() = 0;
    virtual void clearRequestDamage() = 0;
    virtual bool isRequestDamage() const = 0;
    virtual void setBindEndOnGround() = 0;
    virtual bool isBindEndOnGround() const = 0;
    virtual void setBindEndJump(const sead::Vector3f&, s32) = 0;
    virtual bool isBindEndJump() const = 0;
    virtual void validateCollisionCheck() = 0;
    virtual void invalidateCollisionCheck() = 0;
    virtual bool isValidCollisionCheck() = 0;
    virtual bool isCollidedGround() = 0;
    virtual const sead::Vector3f& getCollidedGroundNormal() = 0;
    virtual void validateSensor() = 0;
    virtual void invalidateSensor() = 0;
};
