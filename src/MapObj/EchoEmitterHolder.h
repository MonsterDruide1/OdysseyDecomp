#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"
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

class EchoEmitterHolder : public al::HioNode, public al::IUseExecutor, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_EchoEmitterHolder;

    EchoEmitterHolder();

    void initAfterPlacementSceneObj(const al::ActorInitInfo& info) override;
    void execute() override;

    bool emit(const sead::Vector3f&, f32, s32);
    void emitKeep(const void*, const sead::Vector3f&, f32, s32);
    void setToUbo(agl::DrawContext*, s32, const sead::Matrix34f&, const sead::Matrix44f&) const;
    EchoEmitter* tryGetUnUseEmitter() const;
    void* tryGetUnUseEmitterKeeper(const void*) const;

    const char* getSceneObjName() const override;

private:
    char _10[0x58];
};

static_assert(sizeof(EchoEmitterHolder) == 0x68);

namespace rs {
EchoEmitterHolder* tryGetEmitterHolder(const al::IUseSceneObjHolder*);
void initEchoEmitterHolder(const al::LiveActor*);
bool emitEcho(const al::LiveActor*, const sead::Vector3f&, f32, s32);
void emitKeepEcho(const al::LiveActor*, const sead::Vector3f&, f32, s32);
}  // namespace rs
