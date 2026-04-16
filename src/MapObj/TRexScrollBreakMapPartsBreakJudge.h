#pragma once

#include <math/seadMatrix.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
struct ActorInitInfo;
class LiveActorGroup;
}  // namespace al

class TRex;
class TRexScrollBreakMapParts;

class TRexScrollBreakMapPartsBreakJudge : public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_TRexScrollBreakMapPartsBreakJudge;

    TRexScrollBreakMapPartsBreakJudge(const TRex* tRex, const al::ActorInitInfo& info);

    const char* getSceneObjName() const override {
        return "Tレックススクロール破壊マップパーツの破壊判定";
    }

    ~TRexScrollBreakMapPartsBreakJudge() override = default;

    bool judgeBreak(const TRexScrollBreakMapParts* mapParts) const;
    void addBreakCountAndAppearBreakModel(const TRexScrollBreakMapParts* mapParts);

private:
    const TRex* mTRex = nullptr;
    s32 mBreakCount = 0;
    al::LiveActorGroup* mBreakModelGroupA = nullptr;
    al::LiveActorGroup* mBreakModelGroupB = nullptr;
    al::LiveActorGroup* mBreakModelGroupC = nullptr;
    sead::Matrix34f* mBreakModelMatricesA = nullptr;
    sead::Matrix34f* mBreakModelMatricesB = nullptr;
    sead::Matrix34f* mBreakModelMatricesC = nullptr;
};

static_assert(sizeof(TRexScrollBreakMapPartsBreakJudge) == 0x48);

namespace TRexFunction {
void createTRexScrollBreakMapPartsBreakJudge(const TRex* tRex, const al::ActorInitInfo& info);
}  // namespace TRexFunction
