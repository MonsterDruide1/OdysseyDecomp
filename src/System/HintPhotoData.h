#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

class GameDataFile;

class HintPhotoData : public ByamlSave {
public:
    HintPhotoData(const GameDataFile*);
    void init();
    void saveHintPhoto(const char*);
    bool checkSavedHintPhoto(const char*) const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    GameDataFile* mGameDataFile;
    s32 mSize;
    const char** mNameList;
};

static_assert(sizeof(HintPhotoData) == 0x20);
