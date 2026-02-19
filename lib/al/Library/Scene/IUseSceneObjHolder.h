#pragma once

namespace al {
class SceneObjHolder;

class IUseSceneObjHolder {
public:
    virtual SceneObjHolder* getSceneObjHolder() const = 0;

private:
    static const char* sSceneObjName;
};
}  // namespace al
