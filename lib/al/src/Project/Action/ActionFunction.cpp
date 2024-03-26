#include "Project/Action/ActionFunction.h"

#include "Project/Action/ActionAnimCtrl.h"

namespace alActionFunction {
const char* getAnimName(const al::ActionAnimCtrlInfo* infoCtrl,
                        const al::ActionAnimDataInfo* infoData) {
    const char* animName = infoData->mName;
    if (animName == nullptr) {
        animName = infoCtrl->mActionName;
    }
    return animName;
}
}  // namespace alActionFunction
