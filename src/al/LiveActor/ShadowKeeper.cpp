#include "al/LiveActor/ShadowKeeper.h"

#include "al/LiveActor/DepthShadowMapCtrl.h"
#include "al/LiveActor/ShadowMaskCtrl.h"

namespace al {

    ShadowKeeper::ShadowKeeper() = default;

    void ShadowKeeper::initAfterPlacement(al::GraphicsSystemInfo* info){
        if(mShadowMaskCtrl)
            mShadowMaskCtrl->initAfterPlacement();
        if(mDepthShadowMapCtrl)
            mDepthShadowMapCtrl->initAfterPlacement(info);
    }

    void ShadowKeeper::update() {
        if(mDepthShadowMapCtrl)
            mDepthShadowMapCtrl->update();
    }

    bool ShadowKeeper::isHide() const {
        return false;
    }

    void ShadowKeeper::hide() {
        if(mShadowMaskCtrl)
            mShadowMaskCtrl->hide();
        if(mDepthShadowMapCtrl)
            mDepthShadowMapCtrl->hide();
    }

    void ShadowKeeper::show() {
        if(mShadowMaskCtrl)
            mShadowMaskCtrl->show();
        if(mDepthShadowMapCtrl)
            mDepthShadowMapCtrl->show();
    }

}
