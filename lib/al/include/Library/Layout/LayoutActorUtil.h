#pragma once

#include <math/seadVector.h>

namespace nn::ui2d {
class TextureInfo;
}

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

void setPaneTexture(IUseLayout* layout, const char* paneName, const nn::ui2d::TextureInfo* texture);
void setPaneString(IUseLayout* layout, const char* paneName, const char16_t*, u16);
void setPaneStringFormat(IUseLayout* layout, char const* paneName, char const* format, ...);
}  // namespace al
