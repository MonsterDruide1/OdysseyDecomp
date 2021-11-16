#pragma once

namespace al {
class ActorInitInfo;

class ISceneObj {
public:
    virtual ~ISceneObj();
    virtual const char* getSceneObjName() const;
    virtual void initAfterPlacementSceneObj(const ActorInitInfo&);
    virtual void initSceneObj();
};

}  // namespace al
