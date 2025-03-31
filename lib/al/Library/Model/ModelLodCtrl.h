#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class IUseCamera;
class ByamlIter;
class GraphicsQualityInfo;

class ModelLodCtrl {
public:
    ModelLodCtrl(IUseCamera*, const sead::Vector3f*, const sead::Matrix34f*, const sead::Vector3f*,
                 const sead::BoundBox3f&, s32);
    void init(const ByamlIter&);
    bool isValidate() const;
    bool isEnableMaterialLod() const;
    s32 getModelLevel() const;
    s32 getModelLevelNoClamp() const;
    s32 getShadowLevel() const;
    s32 getMaterialLevel() const;
    void setDistanceScale(f32);
    void update();

private:
    void* padding[0x68 / 8];
};

static_assert(sizeof(ModelLodCtrl) == 0x68);

class ModelLodAllCtrl {
public:
    ModelLodAllCtrl(const GraphicsQualityInfo*);
    void registerLodCtrl(ModelLodCtrl*);
    void endInit();
    void updateLodAll();
    void update();
};

}  // namespace al
