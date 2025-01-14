#pragma once

namespace al {
class ActorInitInfo;

class ISceneObj {
public:
    virtual ~ISceneObj() = default;

    virtual const char* getSceneObjName() const { return ""; }

    virtual void initAfterPlacementSceneObj(const ActorInitInfo&) {}

    virtual void initSceneObj() {}
};

}  // namespace al
