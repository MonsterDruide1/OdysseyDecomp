#include "MapObj/SimpleSignBoard.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Placement/PlacementFunction.h"

SimpleSignBoard::SimpleSignBoard(const char* name) : al::LiveActor(name) {}

void SimpleSignBoard::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    SimpleSignBoardFunction::startSignAimVisAnimFromModelName(this, info);
    makeActorAlive();
}

namespace SimpleSignBoardFunction {
void startSignAimVisAnimFromModelName(al::LiveActor* actor, const al::ActorInitInfo& info) {
    const char* modelName = nullptr;
    alPlacementFunction::getModelName(&modelName, info);

    f32 frame;
    if (al::isEqualSubString(modelName, "LeftDown"))
        frame = 7.0f;
    else if (al::isEqualSubString(modelName, "LeftUp"))
        frame = 6.0f;
    else if (al::isEqualSubString(modelName, "RightDown"))
        frame = 5.0f;
    else if (al::isEqualSubString(modelName, "RightUp"))
        frame = 4.0f;
    else if (al::isEqualSubString(modelName, "Right"))
        frame = 3.0f;
    else if (al::isEqualSubString(modelName, "Left"))
        frame = 2.0f;
    else if (al::isEqualSubString(modelName, "Down"))
        frame = 1.0f;
    else if (al::isEqualSubString(modelName, "Up"))
        frame = 0.0f;
    else
        frame = -1.0f;

    al::startVisAnimAndSetFrameAndStop(actor, "SignAim", frame);
}
}  // namespace SimpleSignBoardFunction
