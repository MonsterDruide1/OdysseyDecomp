#pragma once

namespace al {

class GraphicsSystemInfo;
class ShadowMaskCtrl;
class DepthShadowMapCtrl;

class ShadowKeeper {
public:
    ShadowKeeper();
    void initAfterPlacement(GraphicsSystemInfo* info);
    void update();
    bool isHide() const;
    void hide();
    void show();

    void setShadowMaskCtrl(ShadowMaskCtrl* ctrl) { mShadowMaskCtrl = ctrl; }

    ShadowMaskCtrl* getShadowMaskCtrl() const { return mShadowMaskCtrl; }

    void setDepthShadowMapCtrl(DepthShadowMapCtrl* ctrl) { mDepthShadowMapCtrl = ctrl; }

    DepthShadowMapCtrl* getDepthShadowMapCtrl() const { return mDepthShadowMapCtrl; }

private:
    ShadowMaskCtrl* mShadowMaskCtrl = nullptr;
    DepthShadowMapCtrl* mDepthShadowMapCtrl = nullptr;
};

static_assert(sizeof(ShadowKeeper) == 0x10);

}  // namespace al
