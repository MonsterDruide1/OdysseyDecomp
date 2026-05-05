#include "MapObj/WorldMapKoopaShip.h"

#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorInitFunction.h"

#include "MapObj/KoopaShip.h"
#include "MapObj/PropellerRotateCtrl.h"

static const sead::Vector3f sFloatOffset = {0.0f, 110.0f, 0.0f};

WorldMapKoopaShip* WorldMapKoopaShip::create(const char* name, const al::ActorInitInfo& initInfo,
                                             const sead::Matrix34f* worldMtx) {
    WorldMapKoopaShip* koopaShip = new WorldMapKoopaShip(name);

    al::initActorSceneInfo(koopaShip, initInfo);
    initParts(koopaShip, "KoopaShip", initInfo, worldMtx, sead::Matrix34f::ident, "WorldMap");

    s32 propellerRotateInfoNum = KoopaShipFunction::getPropellerRotateInfoNum();
    koopaShip->mPropellerRotateCtrlNum = propellerRotateInfoNum;
    al::initJointControllerKeeper(koopaShip, propellerRotateInfoNum);

    koopaShip->mPropellerRotateCtrls = new PropellerRotateCtrl*[koopaShip->mPropellerRotateCtrlNum];

    for (s32 i = 0; i < koopaShip->mPropellerRotateCtrlNum; i++) {
        koopaShip->mPropellerRotateCtrls[i] =
            new PropellerRotateCtrl(koopaShip, *KoopaShipFunction::getPropellerRotateInfo(i));
    }

    return koopaShip;
}

WorldMapKoopaShip::WorldMapKoopaShip(const char* name)
    : WorldMapPartsFloat(name, sFloatOffset, 540, 10.0f) {}

void WorldMapKoopaShip::control() {
    WorldMapPartsFloat::control();

    for (s32 i = 0; i < mPropellerRotateCtrlNum; i++)
        mPropellerRotateCtrls[i]->update();
}
