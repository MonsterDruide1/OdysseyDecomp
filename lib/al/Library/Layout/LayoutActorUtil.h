#pragma once

#include <math/seadVector.h>

namespace al {
class LayoutActor;
class IUseLayout;

bool isActive(LayoutActor* actor);
bool isDead(LayoutActor* actor);

void killLayoutIfActive(LayoutActor* actor);
void appearLayoutIfDead(LayoutActor* actor);

void calcTrans(sead::Vector3f* trans, const IUseLayout*);
const sead::Vector3f& getLocalTrans(const IUseLayout*);

void updateLayoutPaneRecursive(LayoutActor* actor);
}  // namespace al
