#pragma once

class IPlayerModelChanger {
public:
    virtual bool isFireFlower() const = 0;
    virtual bool isMini() const = 0;
    virtual bool isChange() const = 0;
    virtual bool is2DModel() const = 0;
    virtual bool isHiddenModel() const = 0;
    virtual bool isHiddenShadowMask() const = 0;
    virtual void resetPosition() = 0;
    virtual void hideModel() = 0;
    virtual void hideSilhouette() = 0;
    virtual void hideShadowMask() = 0;
    virtual void showModel() = 0;
    virtual void showSilhouette() = 0;
    virtual void showShadowMask() = 0;
};
