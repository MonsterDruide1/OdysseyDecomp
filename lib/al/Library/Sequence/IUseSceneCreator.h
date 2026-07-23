#pragma once

namespace al {
class SceneCreator;

class IUseSceneCreator {
public:
    virtual ~IUseSceneCreator() = default;

    virtual void setSceneCreator(SceneCreator* creator) = 0;
    virtual SceneCreator* getSceneCreator() const = 0;
};
}  // namespace al
