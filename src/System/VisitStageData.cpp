#include "System/VisitStageData.h"

#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

namespace {
constexpr s32 cVisitStageMax = 300;
}  // namespace

VisitStageData::VisitStageData() {
    mStageNames = new sead::FixedSafeString<128>[cVisitStageMax];
}

void VisitStageData::init() {
    mVisitStageNum = 0;

    for (s32 i = 0; i < cVisitStageMax; i++)
        mStageNames[i].clear();
}

bool VisitStageData::checkAlreadyVisit(const char* stageName) const {
    for (s32 i = 0; i < mVisitStageNum; i++)
        if (al::isEqualString(stageName, mStageNames[i].cstr()))
            return true;

    return false;
}

void VisitStageData::visit(const char* stageName) {
    for (s32 i = 0; i < mVisitStageNum; i++)
        if (al::isEqualString(stageName, mStageNames[i].cstr()))
            return;

    mStageNames[mVisitStageNum].format("%s", stageName);
    mVisitStageNum++;
}

void VisitStageData::write(al::ByamlWriter* writer) {
    writer->pushArray("VisitStageData");

    for (s32 i = 0; i < mVisitStageNum; i++)
        writer->addString(mStageNames[i].cstr());

    writer->pop();
}

void VisitStageData::read(const al::ByamlIter& save) {
    mVisitStageNum = 0;

    for (s32 i = 0; i < cVisitStageMax; i++)
        mStageNames[i].clear();

    al::ByamlIter visitStageData;
    save.tryGetIterByKey(&visitStageData, "VisitStageData");

    for (s32 i = 0; i < visitStageData.getSize(); i++) {
        const char* stageName = nullptr;
        visitStageData.tryGetStringByIndex(&stageName, i);
        mStageNames[i].format("%s", stageName);
    }

    mVisitStageNum = visitStageData.getSize();
}
