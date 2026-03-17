#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "System/ByamlSave.h"

class WorldList;

class SequenceDemoSkipData : public ByamlSave {
public:
    SequenceDemoSkipData(const WorldList* worldList);

    void init();
    void showWorldIntroCamera(s32 worldId);
    void showScenarioStartCamera(s32 worldId, s32 scenario);
    bool isAlreadyShowWorldIntroCamera(s32 worldId) const;
    bool isAlreadyShowScenarioStartCamera(s32 worldId, s32 scenario) const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    struct WorldData {
        void init();

        bool isFirstWorldIntroCamera;
        bool isFirstScenarioStartCamera[10];
    };

    const WorldList* mWorldList = nullptr;
    sead::PtrArray<WorldData> mWorldData;
    bool mIsShowDemoWorldWarpHole = false;
};

static_assert(sizeof(SequenceDemoSkipData) == 0x28);
