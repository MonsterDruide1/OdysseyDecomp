#pragma once
#include "Library/HostIO/HioNode.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "basis/seadTypes.h"

namespace sead {
class Thread;
}  // namespace sead

namespace nn::account {
class UserHandle;
}  // namespace nn::account

namespace al {

// TODO: Find a better place for this.
class IUseNexNetworkStateHolder {};

class NetworkSystem : public IUseNexNetworkStateHolder, HioNode {
public:
    void allocNexMemory(u64);
    void freeNexMemory(void*);
    void initializeNex();
    void finalizeNex();
    NetworkSystem(nn::account::UserHandle*, bool);
    void execute(sead::Thread*, s64);
    void getNgsFacade() const;
    void endInit();
    ~NetworkSystem();
    void getNexNetworkStateHolder() const;
    void updateBeforeScene();
    void updateAfterScene();
    void requestSystemInitialize();
    bool isAvailable() const;

private:
    char filler[0x48];
};

}  // namespace al
