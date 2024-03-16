#pragma once

namespace al {
class SceneCreator;

// code can be found in Sequence.cpp - required for inlining the dtor there
class IUseSceneCreator {
public:
    virtual ~IUseSceneCreator();
    
    virtual void setSceneCreator(SceneCreator* creator) = 0;
    virtual SceneCreator* getSceneCreator() const = 0;
};
}  // namespace al
