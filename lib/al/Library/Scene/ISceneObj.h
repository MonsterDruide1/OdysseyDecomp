#pragma once

namespace al {
class ActorInitInfo;

class ISceneObj {
public:
    virtual const char* getSceneObjName() const { return ""; }

    virtual ~ISceneObj() = default;

    virtual void initAfterPlacementSceneObj(const ActorInitInfo&) {}

    virtual void initSceneObj() {}
};

}  // namespace al
