#pragma once

#include <math/seadVector.h>

#include "Library/Area/IUseAreaObj.h"
#include "Library/HostIO/HioNode.h"

namespace al {

class CameraStopJudge : public HioNode, public IUseAreaObj {
public:
    CameraStopJudge();
    bool isStop() const;
    void update(const sead::Vector3f& position);

    AreaObjDirector* getAreaObjDirector() const override { return mAreaObjDirector; }

private:
    bool mIsInCameraStopArea = false;
    bool _9 = false;
    bool mIsInvalidStopJudgeByDemo = false;
    AreaObjDirector* mAreaObjDirector = nullptr;
};
}  // namespace al
