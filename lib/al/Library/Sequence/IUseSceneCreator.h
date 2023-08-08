#pragma once

namespace al {
class SceneCreator;

class IUseSceneCreator {
public:
    virtual ~IUseSceneCreator();
    
    virtual void setSceneCreator(SceneCreator* creator) const = 0;
    virtual SceneCreator* getSceneCreator() const = 0;
};
}  // namespace al
