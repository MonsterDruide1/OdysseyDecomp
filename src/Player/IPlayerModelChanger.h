#pragma once

class IPlayerModelChanger {
public:
    virtual bool isFireFlower() const;
    virtual bool isMini() const;
    virtual bool isChange() const;
    virtual bool is2DModel() const;
    virtual bool isHiddenModel() const;
    virtual bool isHiddenShadowMask() const;
    virtual void resetPosition();
    virtual void hideModel();
    virtual void hideSilhouette();
    virtual void hideShadowMask();
    virtual void showModel();
    virtual void showSilhouette();
    virtual void showShadowMask();
};
