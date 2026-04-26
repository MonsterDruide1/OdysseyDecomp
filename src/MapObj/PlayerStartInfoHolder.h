#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Camera/IUseCamera.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

namespace al {
struct ActorInitInfo;
class AreaObjGroup;
class CameraTicket;
class IUseSceneObjHolder;
class LiveActor;
class PlacementInfo;
class Scene;
}  // namespace al
class PlayerStartInfo;
class PlayerStartObj;

class PlayerStartInfoHolder : public al::ISceneObj, public al::IUseCamera, public al::HioNode {
public:
    PlayerStartInfoHolder();

    void init(al::CameraDirector*, s32);
    void registerPlayerStartInfo(const al::PlacementInfo*, const char*, al::CameraTicket*,
                                 PlayerStartObj*, const sead::Vector3f*, const sead::Quatf*);
    bool isRegistered(const al::PlacementInfo*, const char*) const;
    void registerChangeStageAreaGroup(al::AreaObjGroup*, const al::ActorInitInfo&);
    void registerWarpAreaGroup(al::AreaObjGroup*, const al::ActorInitInfo&);
    void registerRestartAreaGroup(al::AreaObjGroup*);
    PlayerStartInfo* tryFindInitInfoByStartId(const char*) const;
    PlayerStartInfo* tryFindInitInfoByPlacementIdString(const char*) const;

    const char* getSceneObjName() const override;

    al::CameraDirector* getCameraDirector() const override;

private:
    sead::PtrArray<PlayerStartInfo> mPlayerStartInfoList;
    al::CameraDirector* mCameraDirector;
};

static_assert(sizeof(PlayerStartInfoHolder) == 0x28);

namespace rs {
void registerLinkedPlayerStartInfoToHolder(const al::IUseSceneObjHolder*, const al::ActorInitInfo&,
                                           const char*, const sead::Vector3f*, const sead::Quatf*);
void registerPlayerStartInfoToHolder(const al::IUseSceneObjHolder*, const al::PlacementInfo&,
                                     const char*, PlayerStartObj*, const sead::Vector3f*,
                                     const sead::Quatf*);
void registerPlayerStartInfoToHolder(const al::IUseSceneObjHolder*, const al::ActorInitInfo&,
                                     const char*, PlayerStartObj*, const sead::Vector3f*,
                                     const sead::Quatf*);
void registerPlayerStartInfoToHolderWithCamera(al::CameraTicket*, const al::IUseSceneObjHolder*,
                                               const al::ActorInitInfo&, const char*,
                                               PlayerStartObj*, const sead::Vector3f*,
                                               const sead::Quatf*);
void registerPlayerStartInfoToHolderForTimeBalloon(const al::IUseSceneObjHolder*,
                                                   const al::ActorInitInfo&);
void registerPlayerStartInfoToHolderForCloset(const al::IUseSceneObjHolder*,
                                              const al::ActorInitInfo&, const sead::Vector3f&,
                                              const sead::Quatf&);
void tryStartEntranceCamera(const al::Scene*, const char*);
bool isPlayerStartCloset(const al::LiveActor*);
}  // namespace rs
