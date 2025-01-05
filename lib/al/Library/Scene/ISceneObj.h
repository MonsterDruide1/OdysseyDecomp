#pragma once

namespace al {
class ActorInitInfo;

class ISceneObj {
public:
    virtual ~ISceneObj();
    virtual const char* getSceneObjName() const = 0;
    virtual void initAfterPlacementSceneObj(const ActorInitInfo&) = 0;
    virtual void initSceneObj() = 0;
};

}  // namespace al
