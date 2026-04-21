#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace agl {
class DrawContext;
}  // namespace agl

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
class LiveActor;
}  // namespace al

class EchoEmitter;
class EchoEmitterHolder;

class EchoEmitterHolder : public al::IUseExecutor, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_EchoEmitterHolder;

    EchoEmitterHolder();

    void initAfterPlacementSceneObj(const al::ActorInitInfo& info) override;
    void execute() override;

    void emit(const sead::Vector3f& trans, f32 scale, s32 life);
    void emitKeep(const void* keeper, const sead::Vector3f& trans, f32 scale, s32 life);
    void setToUbo(agl::DrawContext* context, s32 index, const sead::Matrix34f& viewMtx,
                  const sead::Matrix44f& projMtx) const;
    EchoEmitter* tryGetUnUseEmitter() const;
    void* tryGetUnUseEmitterKeeper(const void* keeper) const;

    const char* getSceneObjName() const override;

private:
    char _10[0x58];
};

static_assert(sizeof(EchoEmitterHolder) == 0x68);

namespace rs {
EchoEmitterHolder* tryGetEmitterHolder(const al::IUseSceneObjHolder*);
void initEchoEmitterHolder(const al::LiveActor*);
void emitEcho(const al::LiveActor*, const sead::Vector3f&, f32, s32);
void emitKeepEcho(const al::LiveActor*, const sead::Vector3f&, f32, s32);
}  // namespace rs
