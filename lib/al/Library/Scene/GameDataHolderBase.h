#pragma once

#include "Library/HostIO/HioNode.h"
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Scene/ISceneObj.h"

namespace al {

class GameDataHolderBase : public ISceneObj, public HioNode, public IUseMessageSystem {};
}  // namespace al
