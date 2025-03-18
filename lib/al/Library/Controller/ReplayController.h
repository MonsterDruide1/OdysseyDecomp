#pragma once

#include <basis/seadTypes.h>
#include <controller/seadControllerWrapper.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead {
class Controller;
}  // namespace sead

namespace al {
class IUsePadDataReader;
class IUsePadDataWriter;

class ReplayController : public sead::ControllerWrapper {
    SEAD_RTTI_OVERRIDE(ReplayController, sead::ControllerWrapper);

public:
    ReplayController(sead::Controller* controller);
    ~ReplayController() override;
    void unregist();
    void startReplay();
    void pauseReplay();
    void endReplay();
    bool isReplaying() const;
    bool isRecording() const;
    s32 getReplayRemainFrame() const;
    void calc(u32 prevHold, bool prevPointerOn) override;
    void startRecord();
    void endRecord();

    void setPadDataReader(IUsePadDataReader* reader) { mPadDataReader = reader; }

    void setPadDataWriter(IUsePadDataWriter* writer) { mPadDataWriter = writer; }

private:
    IUsePadDataReader* mPadDataReader = nullptr;
    bool mIsReplaying = false;
    IUsePadDataWriter* mPadDataWriter = nullptr;
    bool mIsRecording = false;
    bool mIsValidPadReplay = true;
    bool mIsReadPadReplayData = false;
};
}  // namespace al
