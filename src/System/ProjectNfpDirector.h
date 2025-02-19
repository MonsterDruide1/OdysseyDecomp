#pragma once

#include "Library/Nfp/NfpDirector.h"

namespace al {
struct NfpInfo;
}  // namespace al

namespace nn {
class Result;
}  // namespace nn

class ProjectNfpDirector : public al::NfpDirector {
public:
    ProjectNfpDirector();
    void update() override;
    void showError(const nn::Result& result) override;
    al::NfpInfo* tryGetTriggerTouchNfpInfo();

    bool isNfpErrorHandled() { return mIsNfpErrorHandled; }

private:
    bool mIsNfpErrorHandled;
};

static_assert(sizeof(ProjectNfpDirector) == 0xe0, "ProjectNfpDirector Size");
