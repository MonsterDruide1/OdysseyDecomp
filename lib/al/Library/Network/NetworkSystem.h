#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace sead {
class Thread;
}  // namespace sead

namespace nn::account {
class UserHandle;
}  // namespace nn::account

namespace nn::nex {

class NgsFacade;

}  // namespace nn::nex

namespace al {

class NexNetworkStateHolder;

// TODO: Find a better place for this.
class IUseNexNetworkStateHolder {
public:
    virtual NexNetworkStateHolder* getNexNetworkStateHolder() const;
    virtual ~IUseNexNetworkStateHolder();
};

class NetworkSystem : public IUseNexNetworkStateHolder, public HioNode {
public:
    void allocNexMemory(u64);
    void freeNexMemory(void*);
    void initializeNex();
    void finalizeNex();
    NetworkSystem(nn::account::UserHandle*, bool);
    void execute(sead::Thread*, s64);
    nn::nex::NgsFacade* getNgsFacade() const;
    void endInit();
    ~NetworkSystem() override;
    NexNetworkStateHolder* getNexNetworkStateHolder() const override;
    void updateBeforeScene();
    void updateAfterScene();
    void requestSystemInitialize();
    bool isAvailable() const;

private:
    char filler[0x40];
};

static_assert(sizeof(NetworkSystem) == 0x48);

}  // namespace al
