#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace nn {
class Result;
}  // namespace nn

namespace sead {
class Thread;
}  // namespace sead

namespace al {

enum NfpApplicationCommand : u32;

class NfpDirector : public IUseHioNode {
public:
    NfpDirector(s32);

    void executeCommand(sead::Thread*, s64);
    void initialize();
    void enqueueCommand(NfpApplicationCommand);
    void finalize();
    virtual void update();
    void stop();
    void listDevices();
    void start();
    void clearCommand();
    void resetError();
    virtual void showError(const nn::Result& result);
    void executeCommandInitialize();
    void executeCommandFinalize();
    void executeCommandListDevices();
    void executeCommandStart();
    void executeCommandStop();

private:
    char filler[0xd4];
};

}  // namespace al
