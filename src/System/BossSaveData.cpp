#include "System/BossSaveData.h"

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

static const s32 sGKWorldIndexTable[] = {0, 1, 2};

BossSaveData::BossSaveData() {
    mIsShowDemoBossBattleStartLv1 = new bool[sBossBattleStartWorldNum];
    mIsShowDemoBossBattleStartLv2 = new bool[sBossBattleStartWorldNum];
    mIsAlreadyDeadGKLv1 = new bool[sDeadGKNum];
    mIsAlreadyDeadGKLv2 = new bool[sDeadGKNum];
    mIsAlreadyDeadGKLv3 = new bool[sDeadGKNum];
}

void BossSaveData::init() {
    for (s32 i = 0; i < (s32)sBossBattleStartWorldNum; i++) {
        mIsShowDemoBossBattleStartLv1[i] = false;
        mIsShowDemoBossBattleStartLv2[i] = false;
    }

    for (s32 i = 0; i < (s32)sDeadGKNum; i++) {
        mIsAlreadyDeadGKLv1[i] = false;
        mIsAlreadyDeadGKLv2[i] = false;
        mIsAlreadyDeadGKLv3[i] = false;
    }

    mIsShowDemoBattleEndKoopaLv2 = false;
    mIsShowDemoMoonBasementCollapse = false;
}

bool BossSaveData::isAlreadyShowDemoBossBattleStart(s32 world, s32 lv) const {
    if (lv == 1)
        return mIsShowDemoBossBattleStartLv1[world];
    if (lv == 2)
        return mIsShowDemoBossBattleStartLv2[world];
    if (world == 6 && lv == 3)
        return mIsShowDemoBossBattleStartLv2[6];
    if ((u32)world < sDeadGKNum && lv == 3)
        return mIsShowDemoBossBattleStartLv2[world];
    return false;
}

void BossSaveData::showDemoBossBattleStart(s32 world, s32 lv) {
    if (lv == 1)
        mIsShowDemoBossBattleStartLv1[world] = true;
    if (lv == 2)
        mIsShowDemoBossBattleStartLv2[world] = true;
}

bool BossSaveData::isAlreadyDeadGK(s32 world, s32 lv) const {
    s32 worldIdx = (u32)(world - 1) <= 2u ? sGKWorldIndexTable[world - 1] : 0;

    if (lv == 1)
        return mIsAlreadyDeadGKLv1[worldIdx];
    if (lv == 2)
        return mIsAlreadyDeadGKLv2[worldIdx];
    if (lv == 3)
        return mIsAlreadyDeadGKLv3[worldIdx];
    return false;
}

void BossSaveData::onAlreadyDeadGK(s32 world, s32 lv) {
    s32 worldIdx = (u32)(world - 1) <= 2u ? sGKWorldIndexTable[world - 1] : 0;

    if (lv == 1)
        mIsAlreadyDeadGKLv1[worldIdx] = true;
    if (lv == 2)
        mIsAlreadyDeadGKLv2[worldIdx] = true;
    if (lv == 3)
        mIsAlreadyDeadGKLv3[worldIdx] = true;
}

bool BossSaveData::isAlreadyShowDemoBattleEndKoopaLv2() const {
    return mIsShowDemoBattleEndKoopaLv2;
}

void BossSaveData::saveDemoBattleEndKoopaLv2() {
    mIsShowDemoBattleEndKoopaLv2 = true;
}

void BossSaveData::resetLv3Data() {
    for (s32 i = 0; i < (s32)sDeadGKNum; i++)
        mIsAlreadyDeadGKLv3[i] = false;
}

void BossSaveData::write(al::ByamlWriter* writer) {
    writer->pushHash("BossSaveData");

    writer->pushArray("IsShowDemoBossBattleStartLv1");
    for (s32 i = 0; i < (s32)sBossBattleStartWorldNum; i++)
        writer->addBool(mIsShowDemoBossBattleStartLv1[i]);
    writer->pop();

    writer->pushArray("IsShowDemoBossBattleStartLv2");
    for (s32 i = 0; i < (s32)sBossBattleStartWorldNum; i++)
        writer->addBool(mIsShowDemoBossBattleStartLv2[i]);
    writer->pop();

    writer->pushArray("IsAlreadyDeadGK");
    for (s32 i = 0; i < (s32)sDeadGKNum; i++) {
        writer->addBool(mIsAlreadyDeadGKLv1[i]);
        writer->addBool(mIsAlreadyDeadGKLv2[i]);
    }
    writer->pop();

    writer->addBool("IsShowDemoBattleEndKoopaLv2", mIsShowDemoBattleEndKoopaLv2);
    writer->addBool("IsShowDemoMoonBasementCollapse", mIsShowDemoMoonBasementCollapse);

    writer->pop();
}

void BossSaveData::read(const al::ByamlIter& save) {
    init();

    al::ByamlIter bossIter{};
    save.tryGetIterByKey(&bossIter, "BossSaveData");

    al::ByamlIter lv1Iter{};
    bossIter.tryGetIterByKey(&lv1Iter, "IsShowDemoBossBattleStartLv1");

    al::ByamlIter lv2Iter{};
    bossIter.tryGetIterByKey(&lv2Iter, "IsShowDemoBossBattleStartLv2");

    for (s32 i = 0; i < (s32)sBossBattleStartWorldNum; i++) {
        lv1Iter.tryGetBoolByIndex(&mIsShowDemoBossBattleStartLv1[i], i);
        lv2Iter.tryGetBoolByIndex(&mIsShowDemoBossBattleStartLv2[i], i);
    }

    al::ByamlIter gkIter{};
    bossIter.tryGetIterByKey(&gkIter, "IsAlreadyDeadGK");

    for (s32 i = 0; i < (s32)sDeadGKNum; i++) {
        gkIter.tryGetBoolByIndex(&mIsAlreadyDeadGKLv1[i], i * 2 + 0);
        gkIter.tryGetBoolByIndex(&mIsAlreadyDeadGKLv2[i], i * 2 + 1);
    }

    bossIter.tryGetBoolByKey(&mIsShowDemoBattleEndKoopaLv2, "IsShowDemoBattleEndKoopaLv2");
    bossIter.tryGetBoolByKey(&mIsShowDemoMoonBasementCollapse, "IsShowDemoMoonBasementCollapse");
}
