#pragma once

namespace al {
class ActorOcclusionKeeper {
public:
    void appear(bool isModelHidden);
    void requestKill();
    void updateAndRequest();
    void hideModel();
};
}  // namespace al
