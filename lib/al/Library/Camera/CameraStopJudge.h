#pragma once

#include "Library/Area/IUseAreaObj.h"
#include "Library/Camera/CameraTargetBase.h"

namespace al {
class AreaObjDirector;

class CameraStopJudge : public HioNode, public IUseAreaObj {
public:
    void init();
    CameraStopJudge();
    bool isStop() const;
    void update(const sead::Vector3f&);
    AreaObjDirector* getAreaObjDirector() const override;

private:
    bool mIsInObjArea = false;
    bool _9 = false;
    bool mIsInvalidStopJudgeByDemo = false;
    AreaObjDirector* mAreaObjDirector = nullptr;
};
}  // namespace al
