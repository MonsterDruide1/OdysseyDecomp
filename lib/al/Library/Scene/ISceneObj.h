#pragma once

namespace al {
struct ActorInitInfo;

class ISceneObj {
public:
    virtual const char* getSceneObjName() const { return ""; }

    virtual ~ISceneObj() = default;

    virtual void initAfterPlacementSceneObj(const ActorInitInfo& info) {}

    virtual void initSceneObj() {}
};

}  // namespace al
