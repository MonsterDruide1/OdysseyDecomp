#pragma once

namespace al {
class ActorInitInfo;
class SePlayObj {
public:
    void init(const ActorInitInfo& info);
};
}  // namespace al

class SePlayObjWithSave : public al::SePlayObj {
public:
    void init(const al::ActorInitInfo& info);
};
