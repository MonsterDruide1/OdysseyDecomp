#include "Library/Execute/ExecutorListActorDraw.h"

#include "Library/Execute/ExecutorActorCalcAnim.h"
#include "Library/Execute/ExecutorActorCalcView.h"
#include "Library/Execute/ExecutorActorDraw.h"
#include "Library/Execute/ExecutorActorMovement.h"
#include "Library/Execute/ExecutorActorMovementCalcAnim.h"

namespace al {

ExecutorListActorDraw::ExecutorListActorDraw(const char* name, s32 size)
    : ExecutorListActorExecuteBase(name, size) {}

ExecutorActorExecuteBase* ExecutorListActorDraw::createExecutor(const char* name) const {
    return new ExecutorActorDraw(name);
}

ExecutorListActorMovement::ExecutorListActorMovement(const char* name, s32 size)
    : ExecutorListActorExecuteBase(name, size) {}

ExecutorActorExecuteBase* ExecutorListActorMovement::createExecutor(const char* name) const {
    return new ExecutorActorMovement(name);
}

ExecutorListActorCalcAnim::ExecutorListActorCalcAnim(const char* name, s32 size)
    : ExecutorListActorExecuteBase(name, size) {}

ExecutorActorExecuteBase* ExecutorListActorCalcAnim::createExecutor(const char* name) const {
    return new ExecutorActorCalcAnim(name);
}

ExecutorListActorMovementCalcAnim::ExecutorListActorMovementCalcAnim(const char* name, s32 size)
    : ExecutorListActorExecuteBase(name, size) {}

ExecutorActorExecuteBase*
ExecutorListActorMovementCalcAnim::createExecutor(const char* name) const {
    return new ExecutorActorMovementCalcAnim(name);
}

ExecutorListActorCalcView::ExecutorListActorCalcView(const char* name, s32 size)
    : ExecutorListActorExecuteBase(name, size) {}

ExecutorActorExecuteBase* ExecutorListActorCalcView::createExecutor(const char* name) const {
    return new ExecutorActorCalcView(name);
}

}  // namespace al
