#include "Library/Screen/ScreenPointerUtil.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/Screen/ScreenPointKeeper.h"

namespace alScreenPointFunction {

void updateScreenPointAll(al::LiveActor* actor) {
    actor->getScreenPointKeeper()->update();
}

}  // namespace alScreenPointFunction
