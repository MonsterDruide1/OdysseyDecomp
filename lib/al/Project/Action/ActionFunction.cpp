#include "Project/Action/ActionFunction.h"

#include "Project/Action/ActionAnimCtrl.h"
#include "Project/Action/ActionAnimInfo.h"

namespace alActionFunction {
const char* getAnimName(const al::ActionAnimCtrlInfo* infoCtrl,
                        const al::ActionAnimDataInfo* infoData) {
    const char* animName = infoData->actionName;
    if (animName == nullptr)
        animName = infoCtrl->actionName;
    return animName;
}
}  // namespace alActionFunction
