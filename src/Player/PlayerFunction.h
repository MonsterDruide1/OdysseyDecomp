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
class PlayerBodyCostumeInfo;
class PlayerJointControlPartsDynamics;

class PlayerFunction {
public:
    static u32 getPlayerInputPort(const al::LiveActor*);
    static const sead::Matrix34f& getPlayerViewMtx(const al::LiveActor*);
    static bool tryActivateAmiiboPreventDamage(const al::LiveActor*);
    static bool isPlayerDeadStatus(const al::LiveActor* player);
    static void syncBodyHairVisibility(al::LiveActor*, al::LiveActor*);
    static void syncMarioFaceBeardVisibility(al::LiveActor*, al::LiveActor*);
    static void syncMarioHeadStrapVisibility(al::LiveActor*);
    static bool isNeedHairControl(const PlayerBodyCostumeInfo*, const char*);
    static bool isInvisibleCap(const PlayerCostumeInfo*);
    static void hideHairVisibility(al::LiveActor*);

    static PlayerConst* createMarioConst(const char*);
    static void createCapModelName(sead::BufferedSafeStringBase<char>*, const char*);

    static void initMarioModelActor2D(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                      const char* model2DName, bool isInvisCap);
    static al::Resource* initCapModelActor(al::LiveActor*, const al::ActorInitInfo&, const char*);
    static al::Resource* initCapModelActorDemo(al::LiveActor*, const al::ActorInitInfo&,
                                               const char*);
    static PlayerCostumeInfo* initMarioModelActor(al::LiveActor* player,
                                                  const al::ActorInitInfo& initInfo,
                                                  const char* modelName, const char* capType,
                                                  al::AudioKeeper* keeper, bool isCloset);
    static PlayerCostumeInfo*
    initMarioModelActorDemo(PlayerJointControlPartsDynamics** jointCtrlPtr, al::LiveActor* player,
                            const al::ActorInitInfo& initInfo, const char* bodyName,
                            const char* capName, const PlayerConst* pConst,
                            sead::Vector3f* noseScale, sead::Vector3f* earScale, bool isCloset);
};
