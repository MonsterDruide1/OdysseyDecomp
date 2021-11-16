#pragma once

class IPlayerModelChanger {
public:
    virtual bool isFireFlower();
    virtual bool isMini();
    virtual bool isChange();
    virtual bool is2DModel();
    virtual bool isHiddenModel();
    virtual bool isHiddenShadowMask();
    virtual void resetPosition();
    virtual void hideModel();
    virtual void hideSilhouette();
    virtual void hideShadowMask();
    virtual void showModel();
    virtual void showSilhouette();
    virtual void showShadowMask();
};
