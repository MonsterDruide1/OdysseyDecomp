#include "Library/LiveActor/ActorParamHolder.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {

static ActorParamS32 gParamS32;
static ActorParamF32 gParamF32;
static ActorParamMove gParamMove;
static ActorParamJump gParamJump;
static ActorParamSight gParamSight;
static ActorParamRebound gParamRebound;

ActorParamInfo::ActorParamInfo() = default;

const char* ActorParamHolder::getYamlName() {
    return "ActorParam";
}

ActorParamHolder* ActorParamHolder::tryCreate(LiveActor* actor, const Resource* resource,
                                              const char* suffix) {
    sead::FixedSafeString<0x80> fileName;
    tryGetActorInitFileName(&fileName, resource, "ActorParam", suffix);

    if (!isExistResourceYaml(resource, fileName.cstr(), nullptr))
        return nullptr;

    return new ActorParamHolder(actor, resource, suffix);
}

ActorParamHolder::ActorParamHolder(LiveActor* actor, const Resource* resource, const char* suffix) {
    ByamlIter actorParamIter;

    tryGetActorInitFileIter(&actorParamIter, resource, "ActorParam", suffix);
    mSize = actorParamIter.getSize();
    mInfoArray = new ActorParamInfo[mSize];

    for (s32 i = 0; i < mSize; ++i) {
        ActorParamInfo* info = &mInfoArray[i];
        ByamlIter iter;
        actorParamIter.tryGetIterByIndex(&iter, i);

        iter.tryGetStringByKey(&info->name, "ParamName");

        ByamlIter iterKey;
        if (iter.tryGetIterByKey(&iterKey, "S32")) {
            info->type = ActorParamType::S32;
            iterKey.tryGetIntByKey(&info->paramS32, "ParamS32");
            continue;
        }
        if (iter.tryGetIterByKey(&iterKey, "F32")) {
            info->type = ActorParamType::F32;
            iterKey.tryGetFloatByKey(&info->paramF32, "ParamF32");
            continue;
        }
        if (iter.tryGetIterByKey(&iterKey, "ActorParamMove")) {
            info->type = ActorParamType::Move;
            ActorParamMove* param = new ActorParamMove;
            iterKey.tryGetFloatByKey(&param->moveAccel, "MoveAccel");
            iterKey.tryGetFloatByKey(&param->gravity, "Gravity");
            iterKey.tryGetFloatByKey(&param->moveFriction, "MoveFriction");
            iterKey.tryGetFloatByKey(&param->turnSpeedDegree, "TurnSpeedDegree");
            info->paramMove = param;
            continue;
        }
        if (iter.tryGetIterByKey(&iterKey, "ActorParamJump")) {
            info->type = ActorParamType::Jump;
            ActorParamJump* param = new ActorParamJump;
            iterKey.tryGetFloatByKey(&param->speedFront, "SpeedFront");
            iterKey.tryGetFloatByKey(&param->speedUp, "SpeedUp");
            info->paramJump = param;
            continue;
        }
        if (iter.tryGetIterByKey(&iterKey, "ActorParamSight")) {
            info->type = ActorParamType::Sight;
            ActorParamSight* param = new ActorParamSight;
            iterKey.tryGetFloatByKey(&param->distance, "Distance");
            iterKey.tryGetFloatByKey(&param->degreeH, "DegreeH");
            iterKey.tryGetFloatByKey(&param->degreeV, "DegreeV");
            info->paramSight = param;
            continue;
        }
        if (iter.tryGetIterByKey(&iterKey, "ActorParamRebound")) {
            info->type = ActorParamType::Rebound;
            ActorParamRebound* param = new ActorParamRebound;
            iterKey.tryGetFloatByKey(&param->reboundRate, "ReboundRate");
            iterKey.tryGetFloatByKey(&param->speedMinToRebound, "SpeedMinToRebound");
            iterKey.tryGetFloatByKey(&param->frictionH, "FrictionH");
            info->paramRebound = param;
            continue;
        }
    }
}

ActorParamInfo* ActorParamHolder::tryFindParamInfoByName(const char* name) const {
    for (s32 i = 0; i < mSize; ++i) {
        ActorParamInfo* info = &mInfoArray[i];

        if (isEqualString(info->name, name))
            return info;
    }

    return nullptr;
}

ActorParamS32* ActorParamHolder::findParamS32(const char* name) const {
    ActorParamInfo* info = tryFindParamInfoByName(name);

    if (!info)
        return &gParamS32;

    return &info->paramS32;
}

ActorParamF32* ActorParamHolder::findParamF32(const char* name) const {
    ActorParamInfo* info = tryFindParamInfoByName(name);

    if (!info)
        return &gParamF32;

    return &info->paramF32;
}

ActorParamMove* ActorParamHolder::findParamMove(const char* name) const {
    ActorParamInfo* info = tryFindParamInfoByName(name);

    if (!info)
        return &gParamMove;

    return info->paramMove;
}

ActorParamJump* ActorParamHolder::findParamJump(const char* name) const {
    ActorParamInfo* info = tryFindParamInfoByName(name);

    if (!info)
        return &gParamJump;

    return info->paramJump;
}

ActorParamSight* ActorParamHolder::findParamSight(const char* name) const {
    ActorParamInfo* info = tryFindParamInfoByName(name);

    if (!info)
        return &gParamSight;

    return info->paramSight;
}

ActorParamRebound* ActorParamHolder::findParamRebound(const char* name) const {
    ActorParamInfo* info = tryFindParamInfoByName(name);

    if (!info)
        return &gParamRebound;

    return info->paramRebound;
}

}  // namespace al
