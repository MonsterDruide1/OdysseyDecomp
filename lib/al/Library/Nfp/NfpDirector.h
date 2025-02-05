#pragma once

#include <basis/seadTypes.h>

namespace nn {
class Result;
}  // namespace nn

namespace sead {
class Thread;
}  // namespace sead

namespace al {

enum NfpApplicationCommand : u32;

class NfpDirector {
public:
    NfpDirector(s32);

    void executeCommand(sead::Thread*, s64);
    void initialize();
    void enqueueCommand(NfpApplicationCommand);
    void finalize();
    void update();
    void stop();
    void listDevices();
    void start();
    void clearCommand();
    void resetError();
    void showError(const nn::Result&);
    void executeCommandInitialize();
    void executeCommandFinalize();
    void executeCommandListDevices();
    void executeCommandStart();
    void executeCommandStop();

private:
    char filler[0xdc];
};

}  // namespace al
