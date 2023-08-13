#pragma once

namespace al {

class GraphicsSystemInfo;
class ShadowMaskCtrl;
class DepthShadowMapCtrl;

class ShadowKeeper {
public:
    ShadowKeeper();
    void initAfterPlacement(al::GraphicsSystemInfo*);
    void update();
    bool isHide() const;
    void hide();
    void show();

private:
    ShadowMaskCtrl* mShadowMaskCtrl = nullptr;
    DepthShadowMapCtrl* mDepthShadowMapCtrl = nullptr;
};
static_assert(sizeof(ShadowKeeper) == 0x10);

}  // namespace al