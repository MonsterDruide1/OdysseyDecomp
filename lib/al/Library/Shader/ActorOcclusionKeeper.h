#pragma once

namespace al {
class Resource;
class GraphicsSystemInfo;
class LiveActor;

class ActorOcclusionKeeper {
public:
    static bool isExistFile(const Resource*, const char*);

    ActorOcclusionKeeper(const GraphicsSystemInfo*, const Resource*, const LiveActor*, const char*);
    void appear(bool isModelHidden);
    void requestKill();
    void updateAndRequest();
    void hideModel();

private:
    void* _0[0x298 / 8];
};

static_assert(sizeof(ActorOcclusionKeeper) == 0x298);

}  // namespace al
