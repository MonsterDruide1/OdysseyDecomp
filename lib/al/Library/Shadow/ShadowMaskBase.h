#pragma once

#include <math/seadMatrix.h>

namespace al {
class LiveActor;
class ByamlIter;
class MtxConnector;
enum class ShadowMaskType;
enum class ShadowMaskDrawCategory;

class ShadowMaskBase {
public:
    ShadowMaskBase(const char*);

    virtual ~ShadowMaskBase();
    virtual void declare(ShadowMaskDrawCategory);
    virtual void update();
    virtual void initAfterPlacement();
    virtual void calcShadowMatrix(sead::Matrix34f*);
    virtual void createMtxConnector();
    virtual void readParam(const ByamlIter&);
    virtual void updateMulti();
    virtual void addMulti();
    virtual ShadowMaskType getShadowMaskType() const = 0;

    void setHost(const LiveActor*);
    void setDrawCategory(const char*);

    MtxConnector* getMtxConnector() const {
        return mMtxConnector;
    }

    ShadowMaskDrawCategory get_5c() const {
        return _5c;
    }

private:
    void* _8[0x8/8];
    MtxConnector* mMtxConnector;
    void* _18[0x40/8];
    s32 _58;
    ShadowMaskDrawCategory _5c;
    void* _60[0x90/8];
};

static_assert(sizeof(ShadowMaskBase) == 0xf0);

}  // namespace al
