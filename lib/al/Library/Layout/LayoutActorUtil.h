#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace nn::ui2d {
class TextureInfo;
}

namespace sead {
class Color4u8;
}

namespace agl {
class TextureData;
}

namespace al {
class LayoutActor;
class LayoutPaneGroup;
class IUseLayout;
class MessageHolder;

template <typename T>
class Matrix43;

typedef Matrix43<f32> Matrix43f;

bool killLayoutIfActive(LayoutActor*);
bool appearLayoutIfDead(LayoutActor*);
bool isActive(const LayoutActor*);
bool isDead(const LayoutActor*);
void calcTrans(sead::Vector3f*, const IUseLayout*);
sead::Vector2f getLocalTrans(const IUseLayout*);
sead::Vector2f* getLocalTransPtr(const IUseLayout*);
void calcScale(sead::Vector3f*, const IUseLayout*);
f32 getLocalScale(const IUseLayout*);
void setLocalTrans(IUseLayout*, const sead::Vector3f&);
void setLocalTrans(IUseLayout*, const sead::Vector2f&);
void setLocalScale(IUseLayout*, f32);
void setLocalScale(IUseLayout*, const sead::Vector2f&);
void setLocalAlpha(IUseLayout*, f32);
void calcPaneTrans(sead::Vector3f*, const IUseLayout*, const char*);
void calcPaneMtx(sead::Matrix34f*, const IUseLayout*, const char*);
void calcPaneTrans(sead::Vector2f*, const IUseLayout*, const char*);
void calcPaneScale(sead::Vector3f*, const IUseLayout*, const char*);
void calcPaneSize(sead::Vector3f*, const IUseLayout*, const char*);
const Matrix43f& getPaneMtx(const IUseLayout*, const char*);
const Matrix43f* getPaneMtxRaw(const IUseLayout*, const char*);
f32 getGlobalAlpha(const IUseLayout*, const char*);
void setPaneLocalTrans(IUseLayout*, const char*, const sead::Vector2f&);
void setPaneLocalTrans(IUseLayout*, const char*, const sead::Vector3f&);
void setPaneLocalRotate(IUseLayout*, const char*, const sead::Vector3f&);
void setPaneLocalScale(IUseLayout*, const char*, const sead::Vector2f&);
void setPaneLocalSize(IUseLayout*, const char*, const sead::Vector2f&);
void setPaneLocalAlpha(IUseLayout*, const char*, f32);
sead::Vector3f getPaneLocalTrans(const IUseLayout*, const char*);
void getPaneLocalSize(sead::Vector2f*, const IUseLayout*, const char*);
const sead::Vector3f& getPaneLocalRotate(const IUseLayout*, const char*);
const sead::Vector3f& getPaneLocalScale(const IUseLayout*, const char*);
const sead::Vector3f& getTextBoxDrawRectSize(const IUseLayout*, const char*);
void showPane(IUseLayout*, const char*);
void hidePane(IUseLayout*, const char*);
void showPaneNoRecursive(IUseLayout*, const char*);
void hidePaneNoRecursive(IUseLayout*, const char*);
bool isHidePane(const IUseLayout*, const char*);
void showPaneRoot(IUseLayout*);
void hidePaneRoot(IUseLayout*);
void showPaneRootNoRecursive(IUseLayout*);
void hidePaneRootNoRecursive(IUseLayout*);
bool isHidePaneRoot(const IUseLayout*);
bool isExistPane(const IUseLayout*, const char*);
bool isContainPointPane(const IUseLayout*, const char*, const sead::Vector2f&);
void findHitPaneFromLayoutPos(const IUseLayout*, const sead::Vector2f&);
bool isExistHitPaneFromLayoutPos(const IUseLayout*, const sead::Vector2f&);
void findHitPaneFromScreenPos(const IUseLayout*, const sead::Vector2f&);
bool isExistHitPaneFromScreenPos(const IUseLayout*, const sead::Vector2f&);
bool isTouchPosInPane(const IUseLayout*, const char*);
void setCursorPanePos(IUseLayout*, const IUseLayout*);
void setPaneVtxColor(const IUseLayout*, const char*, const sead::Color4u8&);
bool isTriggerTouchPane(const IUseLayout*, const char*);
bool isHoldTouchPane(const IUseLayout*, const char*);
bool isReleaseTouchPane(const IUseLayout*, const char*);
s32 getPaneChildNum(const IUseLayout*, const char*);
const char* getPaneChildName(const IUseLayout*, const char*, s32);
void setPaneStringLength(IUseLayout*, const char*, const char16*, u16, u16);
void setPaneString(IUseLayout*, const char*, const char16*, u16);
void setPaneCounterDigit1(IUseLayout*, const char*, s32, u16);
void setPaneCounterDigit2(IUseLayout*, const char*, s32, u16);
void setPaneCounterDigit3(IUseLayout*, const char*, s32, u16);
void setPaneCounterDigit4(IUseLayout*, const char*, s32, u16);
void setPaneCounterDigit5(IUseLayout*, const char*, s32, u16);
void setPaneCounterDigit6(IUseLayout*, const char*, s32, u16);
void setPaneNumberDigit1(IUseLayout*, const char*, s32, u16);
void setPaneNumberDigit2(IUseLayout*, const char*, s32, u16);
void setPaneNumberDigit3(IUseLayout*, const char*, s32, u16);
void setPaneNumberDigit4(IUseLayout*, const char*, s32, u16);
void setPaneNumberDigit5(IUseLayout*, const char*, s32, u16);
void setPaneStringFormat(IUseLayout*, const char*, const char*, ...);
void setTextPositionCenterH(IUseLayout*, const char*);
void initPaneMessage(IUseLayout*, const char*, const MessageHolder*, const char*, u32);
void setPaneSystemMessage(LayoutActor*, const char*, const char*, const char*);
void setPaneStageMessage(LayoutActor*, const char*, const char*, const char*);
const char16* getPaneStringBuffer(const IUseLayout*, const char*);
s32 getPaneStringBufferLength(const IUseLayout*, const char*);
void setTextBoxPaneFont(const LayoutActor*, const char*, const char*);
void adjustPaneSizeToTextSizeAll(const LayoutActor*);
void requestCaptureRecursive(const LayoutActor*);
void setRubyScale(const LayoutActor*, f32);
nn::ui2d::TextureInfo* createTextureInfo();
nn::ui2d::TextureInfo* createTextureInfo(const agl::TextureData&);
nn::ui2d::TextureInfo* createTextureInfo(const IUseLayout*, const char*);
void getPaneTextureInfo(nn::ui2d::TextureInfo*, const IUseLayout*, const char*);
nn::ui2d::TextureInfo* createTextureInfo(const char*, const char*, const char*);
void updateTextureInfo(nn::ui2d::TextureInfo*, const agl::TextureData&);
void setPaneTexture(IUseLayout*, const char*, const nn::ui2d::TextureInfo*);
void registerLayoutPartsActor(LayoutActor*, LayoutActor*);
void updateLayoutPaneRecursive(LayoutActor*);
s32 getLayoutPaneGroupNum(LayoutActor*);
LayoutPaneGroup* getLayoutPaneGroup(LayoutActor*, s32);
}  // namespace al
