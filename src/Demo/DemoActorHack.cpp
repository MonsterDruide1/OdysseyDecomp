#include "Demo/DemoActorHack.h"

#include "Library/Base/StringUtil.h"
#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Placement/PlacementFunction.h"

#include "Player/PlayerHackStartShaderCtrl.h"

DemoActorHack::DemoActorHack(const char* name) : al::DemoActor(name) {}

void DemoActorHack::init(const al::ActorInitInfo& info) {
    al::DemoActor::init(info);

    mPlayerHackStartShaderParam = new PlayerHackStartShaderParam(false, -1.0f, 10, 20);
    al::tryGetArg(&mPlayerHackStartShaderParam->_8, info, "HackStartShaderStartFrame");
    al::tryGetArg(&mPlayerHackStartShaderParam->_c, info, "HackStartShaderFrame");

    mPlayerHackStartShaderCtrl = new PlayerHackStartShaderCtrl(this, mPlayerHackStartShaderParam);

    const char* modelName = al::getModelName(this);
    if (al::isEqualString(modelName, "DemoFrog")) {
        mHackStartActionName.format("DemoHackFirst");
    } else if (al::isEqualString(modelName, "KoopaHack")) {
        mHackStartActionName.format("DemoHackKoopa01");
        mIsKoopaHack = true;
        al::JointSpringControllerHolder::tryCreateAndInitJointControllerKeeper(
            al::getSubActor(this, "クッパの口ヒゲ"), "InitJointSpringCtrlDemo");
    } else {
        mHackStartActionName.format("");
    }
}

void DemoActorHack::control() {
    if (al::isActionPlaying(this, mHackStartActionName.cstr())) {
        s32 actionFrame = al::getActionFrame(this);
        s32 shaderEndFrame = mPlayerHackStartShaderParam->_8 + mPlayerHackStartShaderParam->_c;

        if (actionFrame == 0)
            mPlayerHackStartShaderCtrl->start();

        if (actionFrame <= shaderEndFrame) {
            mPlayerHackStartShaderCtrl->update();
            if (actionFrame == shaderEndFrame)
                mPlayerHackStartShaderCtrl->end();
        }
    }
}

void DemoActorHack::makeActorAlive() {
    al::LiveActor::makeActorAlive();

    if (mIsKoopaHack)
        al::getSubActor(this, "クッパの口ヒゲ")->makeActorAlive();
}

void DemoActorHack::makeActorDead() {
    al::LiveActor::makeActorDead();

    if (mIsKoopaHack)
        al::getSubActor(this, "クッパの口ヒゲ")->makeActorDead();
}
