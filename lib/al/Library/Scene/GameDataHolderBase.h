#pragma once

#include "al/Library/HostIO/HioNode.h"
#include "al/Library/Message/IUseMessageSystem.h"
#include "al/Library/Scene/ISceneObj.h"

namespace al {
class GameDataHolderBase : public ISceneObj, HioNode, IUseMessageSystem {};
}  // namespace al
