#pragma once

namespace al {
class Resource;
class InitResourceDataAction;
class InitResourceDataAnim;

class ActorInitResourceData {
public:
    ActorInitResourceData(Resource*);

    InitResourceDataAction* getDataAction() const { return mResDataAction; }

private:
    Resource* mResource;
    InitResourceDataAnim* mResDataAnim = nullptr;
    InitResourceDataAction* mResDataAction = nullptr;
};
}  // namespace al
