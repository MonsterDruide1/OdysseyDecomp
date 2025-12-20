#pragma once

#include <math/seadMatrix.h>
#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
struct ActorInitInfo;
class Resource;
class AudioKeeper;
}  // namespace al

class PlayerConst;
class PlayerCostumeInfo;
struct PlayerBodyCostumeInfo;
class PlayerJointControlPartsDynamics;

namespace PlayerFunction {
u32 getPlayerInputPort(const al::LiveActor*);
const sead::Matrix34f& getPlayerViewMtx(const al::LiveActor*);
bool tryActivateAmiiboPreventDamage(const al::LiveActor*);
bool isPlayerDeadStatus(const al::LiveActor* actor);
void syncBodyHairVisibility(al::LiveActor*, al::LiveActor*);
void syncMarioFaceBeardVisibility(al::LiveActor*, al::LiveActor*);
void syncMarioHeadStrapVisibility(al::LiveActor*);
bool isNeedHairControl(const PlayerBodyCostumeInfo*, const char*);
bool isInvisibleCap(const PlayerCostumeInfo*);
void hideHairVisibility(al::LiveActor*);

PlayerConst* createMarioConst(const char*);
void createCapModelName(sead::BufferedSafeStringBase<char>*, const char*);

void initMarioModelActor2D(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                           const char* model2DName, bool isInvisCap);
al::Resource* initCapModelActor(al::LiveActor*, const al::ActorInitInfo&, const char*);
al::Resource* initCapModelActorDemo(al::LiveActor*, const al::ActorInitInfo&, const char*);
PlayerCostumeInfo* initMarioModelActor(al::LiveActor* player, const al::ActorInitInfo& initInfo,
                                       const char* modelName, const char* capType,
                                       al::AudioKeeper* keeper, bool isCloset);
PlayerCostumeInfo* initMarioModelActorDemo(PlayerJointControlPartsDynamics** jointCtrlPtr,
                                           al::LiveActor* player, const al::ActorInitInfo& initInfo,
                                           const char* bodyName, const char* capName,
                                           const PlayerConst* pConst, sead::Vector3f* noseScale,
                                           sead::Vector3f* earScale, bool isCloset);
}  // namespace PlayerFunction
