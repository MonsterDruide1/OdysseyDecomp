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
    void update();
    void showError(const nn::Result& result);
    al::NfpInfo* tryGetTriggerTouchNfpInfo();

private:
    bool _dc;
};
