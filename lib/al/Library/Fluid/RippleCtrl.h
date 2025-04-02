#pragma once

#include <math/seadQuat.h>

#include "Library/Fluid/IUseFluidSurface.h"

namespace al {
class LiveActor;
struct ActorInitInfo;

// TODO: Finish this
class RippleCtrl : public IUseFluidSurface {
public:
    RippleCtrl(LiveActor* parent);

    static RippleCtrl* tryCreate(LiveActor* parent);

    void init(const ActorInitInfo& info);
    void calcPosQuat(sead::Quatf* quat) const;
    void calcOffsetedTrans(sead::Vector3f* trans) const;
    void tryCreateFlowMap();
    void initAfterPlacement();
    void update();
    bool calcIsInArea(const sead::Vector3f&) const override;
    bool isCollide(s32, s32) const;
    void calcPos(sead::Vector3f*, const sead::Vector3f&) const override;
    const sead::Vector3f& getCollidePos(s32, s32) const;
    void calcNormal(sead::Vector3f*, const sead::Vector3f&) const override;
    bool tryAddRipple(const sead::Vector3f&, f32, f32) override;
    bool tryAddRippleWithRange(const sead::Vector3f&, f32, f32, f32, f32) override;
    bool tryAddQuadRipple(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                          const sead::Vector3f&, f32) override;
    void setFieldScale(f32) override;
    const char* getTypeName() const override;
    void forceResetCount();

private:
    unsigned char padding[0x128 - sizeof(IUseFluidSurface)];
};
}  // namespace al
